
#include "HySpexSWIR_384_Serializer.hpp"
#include "HySpexDataIdentifiers.hpp"
#include "../../BlockDataFile/BlockDataFile.hpp"

#include <cassert>

using namespace hyspex;

cHySpexSWIR_384_Serializer::cHySpexSWIR_384_Serializer()
:
    cBlockSerializer()
{}

cHySpexSWIR_384_Serializer::cHySpexSWIR_384_Serializer(std::size_t n, cBlockDataFile* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

cBlockID& cHySpexSWIR_384_Serializer::blockID()
{
    return mBlockID;
}




