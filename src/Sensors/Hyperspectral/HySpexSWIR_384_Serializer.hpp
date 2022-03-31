/**
 * @file
 * @brief Serializer for storing HySpex SWIR 384 data into a block based data file
 */
#pragma once

#include "../../BlockDataFile/BlockSerializer.hpp"
#include "HySpexBlockId.hpp"

// Forward Declarations


class cHySpexSWIR_384_Serializer : public cBlockSerializer
{
public:
	cHySpexSWIR_384_Serializer();
	explicit cHySpexSWIR_384_Serializer(std::size_t n, cBlockDataFile* pDataFile);
	~cHySpexSWIR_384_Serializer() = default;

	void setVersion(uint8_t major, uint8_t minor);


private:
	cHySpex_SWIR_384_ID       mBlockID;
};

