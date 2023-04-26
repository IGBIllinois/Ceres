
#include "HySpexVNIR_3000N_Serializer.hpp"
#include "HySpexDataIdentifiers.hpp"

#include <cbdf/BlockDataFile.hpp>


#include <cassert>

using namespace hyspex;

cHySpexVNIR_3000N_Serializer::cHySpexVNIR_3000N_Serializer()
:
    cBlockSerializer()
{}

cHySpexVNIR_3000N_Serializer::cHySpexVNIR_3000N_Serializer(std::size_t n, cBlockDataFileWriter* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

cBlockID& cHySpexVNIR_3000N_Serializer::blockID()
{
    return mBlockID;
}




