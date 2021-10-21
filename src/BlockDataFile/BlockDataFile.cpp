
#include "BlockDataFile.hpp"

#include <cstddef>
#include <cstdint>

namespace
{
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
    uint32_t crc(uint32_t id, const std::byte* buf, std::size_t len)
    {
        auto c = update_crc(0xffffffffUL, reinterpret_cast<const std::byte*>(&id), sizeof(id));
        return update_crc(c, buf, len) ^ 0xffffffffUL;
    }

    /* Return the CRC of the block id.  Used in zero length data blocks */
    uint32_t crc(uint32_t id)
    {
        return update_crc(0xffffffffUL, reinterpret_cast<const std::byte*>(&id), sizeof(id)) ^ 0xffffffffUL;
    }
}

cBlockDataFile::cBlockDataFile() : mpFile(nullptr)
{
    if (!crc_table_computed)
        make_crc_table();
}

cBlockDataFile::cBlockDataFile(const std::string& filename)
    : cBlockDataFile()
{
    open(filename);
}

cBlockDataFile::~cBlockDataFile()
{

}


void cBlockDataFile::open(const std::string& filename)
{
    mFile.open(filename, std::ios_base::binary | std::ios_base::trunc);
    if (mFile.is_open())
    {
        mFile.write(reinterpret_cast<const char*>(mHeader), sizeof(mHeader));
        mFile.write(reinterpret_cast<const char*>(&mBOM), sizeof(mBOM));
    }
}

bool cBlockDataFile::isOpen()
{
    return mFile.is_open();
}

void cBlockDataFile::close()
{
    mFile.close();
}

void cBlockDataFile::writeBlock(BlockID_t id)
{
    mFile << static_cast<std::uint32_t>(0);
    mFile << static_cast<std::uint32_t>(id);
    mFile << crc(static_cast<std::uint32_t>(id));
}

void cBlockDataFile::writeBlock(BlockID_t id, const std::byte* buf, std::size_t len)
{
    if (len == 0)
    {
        writeBlock(id);
        return;
    }

    mFile << static_cast<std::uint32_t>(len);
    mFile << static_cast<std::uint32_t>(id);
    mFile.write(reinterpret_cast<const char*>(buf), len);
    mFile << crc(static_cast<std::uint32_t>(id), buf, len);
}

