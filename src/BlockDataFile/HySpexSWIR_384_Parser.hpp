/**
 * @file
 * @brief Parser for reading HySpex SWIR 384 data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "HySpexBlockId.hpp"

// Forward Declarations


class cHySpexSWIR_384_Parser : public cBlockParser
{
public:
	cHySpexSWIR_384_Parser();
	~cHySpexSWIR_384_Parser() = default;

	cBlockID& blockID() override;

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void process_DataField(cDataBuffer& buffer);

private:
	cHySpex_SWIR_384_ID       mBlockID;
};

