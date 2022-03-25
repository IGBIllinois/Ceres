
#include "BlockSerializer.hpp"
#include "BlockDataFile.hpp"


cBlockSerializer::cBlockSerializer()
:
    cBlockSerializer(0)
{}

cBlockSerializer::cBlockSerializer(std::size_t n, cBlockDataFile* pDataFile)
:
    mpDataFile(pDataFile)
{
    mDataBuffer.capacity(n);
}


void cBlockSerializer::attach(cBlockDataFile* pDataFile)
{
    mpDataFile = pDataFile;
}

cBlockDataFile* cBlockSerializer::detach()
{
    auto pFile = mpDataFile;
    mpDataFile = nullptr;
    return pFile;
}

void cBlockSerializer::setBufferCapacity(std::size_t n)
{
    mDataBuffer.capacity(n);
}




