/**
 * @file
 * @brief Parser for reading Experiment data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "ExperimentBlockId.hpp"

#include <ctime>

 // Forward Declarations


class cExperimentParser : public cBlockParser
{
public:
	cExperimentParser();
	~cExperimentParser() = default;

	cBlockID& blockID() override;

	void startTime(time_t time);
	void endTime(time_t time);

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void processBeginHeader(cDataBuffer& buffer);
	virtual void processEndOfHeader(cDataBuffer& buffer);

	virtual void processBeginFooter(cDataBuffer& buffer);
	virtual void processEndOfFooter(cDataBuffer& buffer);

private:
    cExperimentID         mBlockID;
};

