
#include "BlockDataFile.hpp"
#include "BlockId.hpp"
#include "BlockDataFileExceptions.hpp"
#include "../BlockDataFile/ClassIdentifiers.hpp"
#include "../BlockDataFile/OusterDataIdentifiers.hpp"
#include "../BlockDataFile/WeatherDataIdentifiers.hpp"

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <cerrno>
#include <cstring>


namespace
{
    static const uint8_t  BLOCK_FILE_HEADER_SIZE = 8;
    static const uint8_t  BLOCK_FILE_HEADER[BLOCK_FILE_HEADER_SIZE] = { 137, 83, 80, 68, 82, 67, 77, 03 };
    static const uint16_t BLOCK_FILE_BOM = 0x55AA;

    uint32_t to_uint32(const cBlockID& blockID)
    {
        uint32_t id = static_cast<uint32_t>(blockID.classID()) << 16;
        id |= static_cast<uint16_t>(blockID.majorVersion()) << 8;
        id |= blockID.minorVersion();

        return id;
    }

    /* Table of CRCs of all 8-bit messages. */
    uint32_t crc_table[256];

    /* Flag: has the table been computed? Initially false. */
    bool crc_table_computed = false;

    /* Make the table for a fast CRC. */
    void make_crc_table()
    {
        for (int n = 0; n < 256; ++n)
        {
            std::uint32_t c = n;
            for (int k = 0; k < 8; ++k)
            {
                if (c & 1)
                    c = 0xEDB88320L ^ (c >> 1);
                else
                    c = c >> 1;
            }
            crc_table[n] = c;
        }

        crc_table_computed = true;
    }

    /* Update a running CRC with the bytes buf[0..len-1]--the CRC
       should be initialized to all 1's, and the transmitted value
       is the 1's complement of the final running CRC (see the
       crc() routine below). */

    uint32_t update_crc(uint32_t crc, const std::byte* buf, std::size_t len)
    {
        std::uint32_t c = crc;
        for (int n = 0; n < len; ++n)
        {
            c = crc_table[(c ^ static_cast<const unsigned char>(buf[n])) & 0xff] ^ (c >> 8);
        }
        
        return c;
    }

    /* Return the CRC of the block id and block bytes buf[0..len-1]. */
    uint32_t crc(const cBlockID& blockID, const std::byte* buf, std::size_t len)
    {
        uint32_t id = to_uint32(blockID);
        auto c = update_crc(0xffffffffUL, reinterpret_cast<const std::byte*>(&id), sizeof(id));
        auto dataID = blockID.dataID();
        c = update_crc(c, reinterpret_cast<const std::byte*>(&dataID), sizeof(dataID)); // ^ 0xffffffffUL;
        return update_crc(c, buf, len) ^ 0xffffffffUL;
    }

    /* Return the CRC of the block id.  Used in zero length data blocks */
    uint32_t crc(const cBlockID& blockID)
    {
        uint32_t id = to_uint32(blockID);
        auto c = update_crc(0xffffffffUL, reinterpret_cast<const std::byte*>(&id), sizeof(id));
        auto dataID = blockID.dataID();
        return update_crc(c, reinterpret_cast<const std::byte*>(&dataID), sizeof(dataID)) ^ 0xffffffffUL;
    }
}

/*******************************************************************
 * 
 * B L O C K   D A T A   F I L E   W R I T E R
 * 
 ******************************************************************/

cBlockDataFileWriter::cBlockDataFileWriter()
{
    if (!crc_table_computed)
        make_crc_table();
}

cBlockDataFileWriter::cBlockDataFileWriter(const std::string& filename)
    : cBlockDataFileWriter()
{
    if (!open(filename))
    {
        std::string msg = "Failed to open file: ";
        msg += filename;
        throw bdf::io_error(msg);
    }
}

cBlockDataFileWriter::~cBlockDataFileWriter()
{
    mFile.close();
}


bool cBlockDataFileWriter::open(const std::string& filename)
{
    mFile.open(filename, std::ios_base::binary | std::ios_base::trunc);
    if (mFile.good() && mFile.is_open())
    {
        mFile.write(reinterpret_cast<const char*>(BLOCK_FILE_HEADER), sizeof(BLOCK_FILE_HEADER));
        if (!mFile.good())
        {
            mFile.close();
            return false;
        }

        mFile.write(reinterpret_cast<const char*>(&BLOCK_FILE_BOM), sizeof(BLOCK_FILE_BOM));
        if (!mFile.good())
        {
            mFile.close();
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool cBlockDataFileWriter::isOpen() const
{
    return mFile.is_open();
}

void cBlockDataFileWriter::close()
{
    mFile.close();
}

bool cBlockDataFileWriter::fail() const
{
    return mFile.fail();
}

bool cBlockDataFileWriter::bad() const
{
    return mFile.bad();
}

bool cBlockDataFileWriter::good() const
{
    return mFile.good();
}

bool cBlockDataFileWriter::writeBlock(const cBlockID& id)
{
    std::lock_guard<std::mutex> write_guard(mWriteMutex);

    std::uint32_t n = 0;
    mFile.write(reinterpret_cast<const char*>(&n), sizeof(n));

    BLOCK_CLASS_ID_t classID = id.classID();
    BLOCK_MAJOR_VERSION_t majorVersion = id.majorVersion();
    BLOCK_MINOR_VERSION_t minorVersion = id.minorVersion();
    BLOCK_DATA_ID_t dataID = id.dataID();

    mFile.write(reinterpret_cast<const char*>(&classID), sizeof(classID));
    mFile.write(reinterpret_cast<const char*>(&majorVersion), sizeof(majorVersion));
    mFile.write(reinterpret_cast<const char*>(&minorVersion), sizeof(minorVersion));
    mFile.write(reinterpret_cast<const char*>(&dataID), sizeof(dataID));

    uint32_t c = crc(id);
    mFile.write(reinterpret_cast<const char*>(&c), sizeof(c));

    return mFile.good();
}

bool cBlockDataFileWriter::writeBlock(const cBlockID& id, const std::byte* buf, std::size_t len)
{
    if (len == 0)
    {
        return writeBlock(id);
    }

    std::lock_guard<std::mutex> write_guard(mWriteMutex);

    std::uint32_t n = len;
    mFile.write(reinterpret_cast<const char*>(&n), sizeof(n));

    BLOCK_CLASS_ID_t classID = id.classID();
    BLOCK_MAJOR_VERSION_t majorVersion = id.majorVersion();
    BLOCK_MINOR_VERSION_t minorVersion = id.minorVersion();
    BLOCK_DATA_ID_t dataID = id.dataID();

    mFile.write(reinterpret_cast<const char*>(&classID), sizeof(classID));
    mFile.write(reinterpret_cast<const char*>(&majorVersion), sizeof(majorVersion));
    mFile.write(reinterpret_cast<const char*>(&minorVersion), sizeof(minorVersion));
    mFile.write(reinterpret_cast<const char*>(&dataID), sizeof(dataID));

    mFile.write(reinterpret_cast<const char*>(buf), len);

    uint32_t c = crc(id, buf, len);
    mFile.write(reinterpret_cast<const char*>(&c), sizeof(c));

    return mFile.good();
}


/*******************************************************************
 *
 * B L O C K   D A T A   F I L E   R E A D E R
 *
 ******************************************************************/

cBlockDataFileReader::cBlockDataFileReader() : mByteSwapNeeded(false)
{
    if (!crc_table_computed)
        make_crc_table();
}

cBlockDataFileReader::cBlockDataFileReader(const std::string& filename)
    : cBlockDataFileReader()
{
    open(filename);
}

cBlockDataFileReader::~cBlockDataFileReader()
{
    mFile.close();
}


bool cBlockDataFileReader::open(const std::string& filename)
{
    mFile.open(filename, std::ios_base::binary);
    if (mFile.is_open())
    {
        unsigned char header[BLOCK_FILE_HEADER_SIZE] = { 0 };
        mFile.read(reinterpret_cast<char*>(header), BLOCK_FILE_HEADER_SIZE);
        if (mFile.bad() || mFile.fail())
        {
            mFile.close();
            return false;
        }

        bool result = std::equal(std::begin(header), std::end(header),
            std::begin(BLOCK_FILE_HEADER), std::end(BLOCK_FILE_HEADER));

        if (!result)
        {
            mFile.close();
            return false;
        }

        uint16_t bom = 0;
        mFile.read(reinterpret_cast<char*>(&bom), sizeof(bom));
        if (mFile.bad() || mFile.fail())
        {
            mFile.close();
            return false;
        }

        const uint16_t BLOCK_FILE_BOM_SWAP = 0xAA55;
        mByteSwapNeeded = bom == BLOCK_FILE_BOM_SWAP;
    }
    else
    {
        return false;
    }

    return true;
}

bool cBlockDataFileReader::isOpen() const
{
    return mFile.is_open();
}

void cBlockDataFileReader::close()
{
    mFile.close();
}

bool cBlockDataFileReader::fail() const
{
    return mFile.fail();
}

bool cBlockDataFileReader::bad() const
{
    return mFile.bad();
}

bool cBlockDataFileReader::good() const
{
    return mFile.good();
}

bool cBlockDataFileReader::eof() const
{
    return mFile.eof();
}

bool cBlockDataFileReader::processBlock()
{
    if (mFile.eof())
        return false;

    if (mFile.fail())
    {
        throw bdf::formatting_error("I/O error while processing block.");
    }

    mStartOfBlock = mFile.tellg();

    std::uint32_t len = 0;
    mFile.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (mFile.bad())
    {
        std::string msg = "I/O error while reading block length: ";
        msg += std::strerror(errno);
        throw bdf::stream_error(errno, msg);
    }

    if (mByteSwapNeeded)
    {
    }

    BLOCK_CLASS_ID_t classID = 0;;
    BLOCK_MAJOR_VERSION_t majorVersion = 0;
    BLOCK_MINOR_VERSION_t minorVersion = 0;
    BLOCK_DATA_ID_t data_id = 0;

    mStartOfClassID = mFile.tellg();

    mFile.read(reinterpret_cast<char*>(&classID), sizeof(classID));;
    if (mFile.bad())
    {
        std::string msg = "I/O error while reading class id: ";
        msg += std::strerror(errno);
        throw bdf::stream_error(errno, msg);
    }

    mStartOfMajorVersion = mFile.tellg();

    mFile.read(reinterpret_cast<char*>(&majorVersion), sizeof(majorVersion));;
    if (mFile.bad())
    {
        std::string msg = "I/O error while reading major version: ";
        msg += std::strerror(errno);
        throw bdf::stream_error(errno, msg);
    }

    mStartOfMinorVersion = mFile.tellg();

    mFile.read(reinterpret_cast<char*>(&minorVersion), sizeof(minorVersion));;
    if (mFile.bad())
    {
        std::string msg = "I/O error while reading minor version: ";
        msg += std::strerror(errno);
        throw bdf::stream_error(errno, msg);
    }

    mStartOfDataID = mFile.tellg();
    mFile.read(reinterpret_cast<char*>(&data_id), sizeof(data_id));
    if (mFile.bad())
    {
        std::string msg = "I/O error while reading data id: ";
        msg += std::strerror(errno);
        throw bdf::stream_error(errno, msg);
    }


    if (mByteSwapNeeded)
    {
    }

    cBlockID blockId(static_cast<ClassIDs>(classID), majorVersion, minorVersion);
    blockId.dataID(data_id);

    if (len == 0)
    {
        mStartOfCRC = mFile.tellg();
        uint32_t file_crc = 0;
        mFile.read(reinterpret_cast<char*>(&file_crc), sizeof(file_crc));
        if (mFile.bad())
        {
            std::string msg = "I/O error while reading file CRC: ";
            msg += std::strerror(errno);
            throw bdf::stream_error(errno, msg);
        }

        if (file_crc != crc(blockId))
        {
            if (!tryToFixBlockId(blockId))
            {
                std::string msg = "CRC failure: Class ID=";
                msg += std::to_string(classID);
                msg += ", Major Version=";
                msg += std::to_string(majorVersion);
                msg += ", Minor Version=";
                msg += std::to_string(minorVersion);
                msg += ", Data ID=";
                msg += std::to_string(data_id);
                msg += ", Data Lenth=0";
                throw bdf::crc_error(classID, majorVersion, minorVersion, data_id, msg);
            }
        }

        mBuffer.reset();
    }
    else
    {
        if (mBuffer.capacity() < len)
        {
            mBuffer.capacity(len);
        }

        mBuffer.reset();
        mStartOfData = mFile.tellg();
        mFile.read(reinterpret_cast<char*>(mBuffer.data(len)), len);
        if (mFile.bad())
        {
            std::string msg = "I/O error while reading block data: ";
            msg += std::strerror(errno);
            throw bdf::stream_error(errno, msg);
        }

        mStartOfCRC = mFile.tellg();
        uint32_t file_crc = 0;
        mFile.read(reinterpret_cast<char*>(&file_crc), sizeof(file_crc));
        if (mFile.bad())
        {
            std::string msg = "I/O error while reading file CRC: ";
            msg += std::strerror(errno);
            throw bdf::stream_error(errno, msg);
        }

        uint32_t c = crc(blockId, mBuffer.data(), len);
        if (file_crc != c)
        {
            if (tryToFixBlock(blockId, mBuffer, len))
            {
                std::string msg = "CRC failure: Class ID=";
                msg += std::to_string(classID);
                msg += ", Major Version=";
                msg += std::to_string(majorVersion);
                msg += ", Minor Version=";
                msg += std::to_string(minorVersion);
                msg += ", Data ID=";
                msg += std::to_string(data_id);
                msg += ", Data Lenth=";
                msg += std::to_string(len);
                throw bdf::crc_error(classID, majorVersion, minorVersion, data_id, msg);
            }
        }
    }

    if (len == 0)
    {
        processBlock(blockId);
    }
    else
    {
        processBlock(blockId, mBuffer.data(), len);
    }

    return !mFile.eof();
}

bool cBlockDataFileReader::tryToFixBlockId(const cBlockID blockID)
{
    switch (checkBlockId(blockID))
    {
    case eBlockStatus::OK:
    default:
        break;
    }
//    mFile.seekg(mStartOfBlock);
    return false;
}

bool cBlockDataFileReader::tryToFixBlock(const cBlockID blockID, const cDataBuffer buffer, const uint32_t len)
{
    switch (checkBlockId(blockID))
    {
    case eBlockStatus::OK:
    default:
        break;
    }

    uint32_t testLen = 0;
    BLOCK_CLASS_ID_t classID = 0;
    BLOCK_MAJOR_VERSION_t majorVer = 0;
    BLOCK_MINOR_VERSION_t minorVer = 0;
    BLOCK_DATA_ID_t dataID = 0;

    auto n = buffer.read_size() - 6;
    std::size_t i = 0;
    for (; i < n; ++i)
    {
        if (i > 4)
        {
            testLen = *(reinterpret_cast<const uint32_t*>(buffer.data(i-4)));
        }
        classID = *(reinterpret_cast<const BLOCK_CLASS_ID_t*>(buffer.data(i)));
        majorVer = *(reinterpret_cast<const BLOCK_MAJOR_VERSION_t*>(buffer.data(i+2)));
        minorVer = *(reinterpret_cast<const BLOCK_MINOR_VERSION_t*>(buffer.data(i+3)));
        dataID = *(reinterpret_cast<const BLOCK_DATA_ID_t*>(buffer.data(i+4)));
        cBlockID testID(static_cast<ClassIDs>(classID), majorVer, minorVer);
        testID.dataID(dataID);
        if (eBlockStatus::OK == checkBlockId(testID, testLen))
        {
            break;
        }
    }

    return false;
}


cBlockDataFileReader::eBlockStatus cBlockDataFileReader::checkBlockId(const cBlockID blockID, uint32_t testLen)
{
    switch (static_cast<ClassIDs>(blockID.classID()))
    {
    case ClassIDs::EXPERIMENT_INFO:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::PVT:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::SPIDERCAM:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::OUSTER:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::OUSTER_LIDAR:
    {
        if (blockID.majorVersion() != 2)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 3)
            return eBlockStatus::BAD_MINOR_VERSION;

        auto dataId = static_cast<ouster::DataID>(blockID.dataID());

        switch (dataId)
        {
        case ouster::DataID::CONFIGURATION_INFO:
        case ouster::DataID::BEAM_INTRINSICS:
        case ouster::DataID::IMU_INTRINSICS:
        case ouster::DataID::IMU_DATA:
        case ouster::DataID::LIDAR_DATA:
        case ouster::DataID::LIDAR_DATA_FORMAT:
        case ouster::DataID::LIDAR_DATA_FRAME_TIMESTAMP:
        case ouster::DataID::LIDAR_INTRINSICS:
        case ouster::DataID::MULTIPURPOSE_IO:
        case ouster::DataID::NMEA:
        case ouster::DataID::SENSOR_INFO:
        case ouster::DataID::SYNC_PULSE_IN:
        case ouster::DataID::SYNC_PULSE_OUT:
        case ouster::DataID::TIMESTAMP:
        case ouster::DataID::TIME_INFO:
            break;
        default:
            return eBlockStatus::BAD_DATA_ID;
        }

        break;
    }
    case ClassIDs::SEPTENTRIO:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::SSNX:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::HYSPEX:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::HYSPEX_SWIR_384:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::HYSPEX_VNIR_3000N:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::AXIS_COMMUNICATIONS:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::AXIS_COMMUNICATIONS_CAMERA:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        break;
    }
    case ClassIDs::WEATHER:
    {
        if (blockID.majorVersion() != 1)
            return eBlockStatus::BAD_MAJOR_VERSION;

        if (blockID.minorVersion() != 0)
            return eBlockStatus::BAD_MINOR_VERSION;

        auto dataId = static_cast<weather::DataID>(blockID.dataID());

        switch (dataId)
        {
        case weather::DataID::CONFIGURATION_INFO:
        case weather::DataID::WIND_DATA_VALID:
        case weather::DataID::WIND_SPEED_MPS:
        case weather::DataID::WIND_SPEED_KNOTS:
        case weather::DataID::WIND_DIRECTION_DEG:
            break;

        default:
            return eBlockStatus::BAD_DATA_ID;
        }

        break;
    }
    default:
        return eBlockStatus::BAD_CLASS_ID;
    };

    return eBlockStatus::OK;
}

