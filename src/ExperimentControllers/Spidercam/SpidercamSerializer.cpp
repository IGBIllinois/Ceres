
#include "SpidercamSerializer.hpp"
#include "SpidercamDataIdentifiers.hpp"
#include "../BlockDataFile/BlockDataFile.hpp"


#include <cassert>

using namespace spidercam;

cSpidercamSerializer::cSpidercamSerializer()
:
    cSpidercamSerializer(nullptr)
{}

cSpidercamSerializer::cSpidercamSerializer(cBlockDataFile* pDataFile)
:
    mpDataFile(pDataFile)
{
    mDataBuffer.capacity(32 * 1024);
}


void cSpidercamSerializer::attach(cBlockDataFile* pDataFile)
{
    mpDataFile = pDataFile;
}

cBlockDataFile* cSpidercamSerializer::detach()
{
    auto pFile = mpDataFile;
    mpDataFile = nullptr;
    return pFile;
}


void cSpidercamSerializer::setVersion(uint8_t major, uint8_t minor)
{
	mBlockID.majorVersion(major);
	mBlockID.minorVersion(minor);
}

void cSpidercamSerializer::setBufferCapacity(std::size_t n)
{
    mDataBuffer.capacity(n);
}




