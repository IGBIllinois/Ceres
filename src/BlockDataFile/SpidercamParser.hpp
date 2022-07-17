/**
 * @file
 * @brief Parser for reading Spidercam data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "SpidercamBlockId.hpp"

#include <spidercam/spidercam_types.hpp>


// Forward Declarations


class cSpidercamParser : public cBlockParser
{
public:
	cSpidercamParser();
	~cSpidercamParser() = default;

	cBlockID& blockID() override;

	spidercam::sPosition_1_t	getPosition() const { return mPosition; }

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

protected:
	virtual void processPosition(cDataBuffer& buffer);

protected:
	spidercam::sPosition_1_t	mPosition;

private:
    cSpidercamID    mBlockID;
};

