
#include "BlockSerializer.hpp"
#include "BlockDataFile.hpp"
#include "BlockId.hpp"


cBlockSerializer::cBlockSerializer()
:
    cBlockSerializer(1024)
{}

cBlockSerializer::cBlockSerializer(std::size_t n, cBlockDataFileWriter* pDataFile)
:
    mpDataFile(pDataFile)
{
    mDataBuffer.capacity(n);
}

uint16_t cBlockSerializer::classID() const
{
    return const_cast<cBlockSerializer*>(this)->blockID().classID();
}

void cBlockSerializer::attach(cBlockDataFileWriter* pDataFile)
{
    mpDataFile = pDataFile;
}

cBlockDataFileWriter* cBlockSerializer::detach()
{
    auto pFile = mpDataFile;
    mpDataFile = nullptr;
    return pFile;
}

void cBlockSerializer::setBufferCapacity(std::size_t n)
{
    mDataBuffer.capacity(n);
}

void cBlockSerializer::setVersion(uint8_t major, uint8_t minor)
{
    blockID().setVersion(major, minor);
}




