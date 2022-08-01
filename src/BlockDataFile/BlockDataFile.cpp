
#include "BlockDataFile.hpp"
#include "BlockId.hpp"
#include "BlockDataFileExceptions.hpp"

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

void cBlockDataFileReader::attach(cBlockParser* pParser)
{
    if (!pParser) return;

    // sBlockHeader id = pParser->blockID().blockHeader();
    BLOCK_CLASS_ID_t id = pParser->blockID().classID();

    mParsers.insert(std::make_pair(id, pParser));
}

cBlockParser* cBlockDataFileReader::detach(cBlockID id)
{
    auto it = mParsers.find(id.classID());
    if (mParsers.end() != it)
    {
        mParsers.erase(it);
        return it->second;
    }

    return nullptr;
}

bool cBlockDataFileReader::processBlock()
{
    if (mFile.eof())
        return false;

    if (mFile.fail())
    {
        throw bdf::formatting_error("I/O error while processing block.");
    }

    std::uint32_t len = 0;
    mFile.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (mFile.bad())
    {
        throw bdf::stream_error(errno, std::strerror(errno));
    }

    if (mByteSwapNeeded)
    {
    }

    BLOCK_CLASS_ID_t classID = 0;;
    BLOCK_MAJOR_VERSION_t majorVersion = 0;
    BLOCK_MINOR_VERSION_t minorVersion = 0;
    BLOCK_DATA_ID_t data_id = 0;

    mFile.read(reinterpret_cast<char*>(&classID), sizeof(classID));;
    if (mFile.bad())
    {
        throw std::runtime_error("I/O error while reading block class ID.");
    }

    mFile.read(reinterpret_cast<char*>(&majorVersion), sizeof(majorVersion));;
    if (mFile.bad())
    {
        throw std::runtime_error("I/O error while reading block major version.");
    }

    mFile.read(reinterpret_cast<char*>(&minorVersion), sizeof(minorVersion));;
    if (mFile.bad())
    {
        throw std::runtime_error("I/O error while reading block minor version.");
    }

    mFile.read(reinterpret_cast<char*>(&data_id), sizeof(data_id));
    if (mFile.bad())
    {
        throw std::runtime_error("I/O error while reading data ID.");
    }


    if (mByteSwapNeeded)
    {
    }

    cBlockID blockId(static_cast<ClassIDs>(classID), majorVersion, minorVersion);
    blockId.dataID(data_id);

    if (len == 0)
    {
        uint32_t file_crc = 0;
        mFile.read(reinterpret_cast<char*>(&file_crc), sizeof(file_crc));
        if (mFile.bad())
        {
            throw std::runtime_error("I/O error while reading file CRC.");
        }

        if (file_crc != crc(blockId))
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

        mBuffer.reset();
    }
    else
    {
        if (mBuffer.capacity() < len)
        {
            mBuffer.capacity(len);
        }

        mBuffer.reset();
        mFile.read(reinterpret_cast<char*>(mBuffer.data(len)), len);
        if (mFile.bad())
        {
            throw std::runtime_error("I/O error while reading block data.");
        }

        uint32_t file_crc = 0;
        mFile.read(reinterpret_cast<char*>(&file_crc), sizeof(file_crc));
        if (mFile.bad())
        {
            throw std::runtime_error("I/O error while reading file CRC.");
        }

        uint32_t c = crc(blockId, mBuffer.data(), len);
        if (file_crc != c)
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

    auto parser = mParsers.find(classID);
    if (parser != mParsers.end())
    {
        parser->second->processData(majorVersion, minorVersion, data_id, mBuffer);
    }
    else
    {
        unknownClassID(classID);
    }

    return !mFile.eof();
}

