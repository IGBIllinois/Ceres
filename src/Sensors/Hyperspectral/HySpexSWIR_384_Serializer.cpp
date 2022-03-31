
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

void cHySpexSWIR_384_Serializer::setVersion(uint8_t major, uint8_t minor)
{
	mBlockID.setVersion(major, minor);
}




