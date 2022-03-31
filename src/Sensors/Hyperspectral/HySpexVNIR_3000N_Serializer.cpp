
#include "HySpexVNIR_3000N_Serializer.hpp"
#include "HySpexDataIdentifiers.hpp"
#include "../../BlockDataFile/BlockDataFile.hpp"


#include <cassert>

using namespace hyspex;

cHySpexVNIR_3000N_Serializer::cHySpexVNIR_3000N_Serializer()
:
    cBlockSerializer()
{}

cHySpexVNIR_3000N_Serializer::cHySpexVNIR_3000N_Serializer(std::size_t n, cBlockDataFile* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

void cHySpexVNIR_3000N_Serializer::setVersion(uint8_t major, uint8_t minor)
{
	mBlockID.setVersion(major, minor);
}




