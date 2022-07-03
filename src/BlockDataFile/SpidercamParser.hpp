/**
 * @file
 * @brief Parser for reading Spidercam data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "SpidercamBlockId.hpp"

// Forward Declarations


class cSpidercamParser : public cBlockParser
{
public:
	cSpidercamParser();
	~cSpidercamParser() = default;

	cBlockID& blockID() override;

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void process_DataField(cDataBuffer& buffer);

private:
    cSpidercamID    mBlockID;
};

