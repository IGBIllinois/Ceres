/**
 * @file
 * @brief Parser for reading HySpex VNIR 3000N data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "HySpexBlockId.hpp"

// Forward Declarations


class cHySpexVNIR_3000N_Parser : public cBlockParser
{
public:
	cHySpexVNIR_3000N_Parser();
	~cHySpexVNIR_3000N_Parser() = default;

	cBlockID& blockID() override;

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void process_DataField(cDataBuffer& buffer);

private:
	cHySpex_VNIR_3000N_ID       mBlockID;
};

