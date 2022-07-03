
#include "SpidercamSerializer.hpp"
#include "SpidercamDataIdentifiers.hpp"
#include "BlockDataFile.hpp"


#include <cassert>

using namespace spidercam;

cSpidercamSerializer::cSpidercamSerializer()
:
    cBlockSerializer()
{}

cSpidercamSerializer::cSpidercamSerializer(std::size_t n, cBlockDataFileWriter* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

cBlockID& cSpidercamSerializer::blockID()
{
    return mBlockID;
}


