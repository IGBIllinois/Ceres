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

	/*
	void writeBeginSensorList();
	void writeEndOfSensorList();
	void writeSensorBlockInfo(uint16_t class_id, const std::string& name);

	void startTime(time_t time);
	void endTime(time_t time);

	void startRecordingTimestamp(uint64_t timestamp);
	void endRecordingTimestamp(uint64_t timestamp);
	*/


protected:
	virtual void onBeginHeader() = 0;
	virtual void onEndOfHeader() = 0;

	virtual void onBeginFooter() = 0;
	virtual void onEndOfFooter() = 0;

	virtual void onExperimentTitle(const std::string& title) = 0;
	virtual void onExperimentCultivar(const std::string& cultivar) = 0;
	virtual void onExperimentResearcher(const std::string& researcher) = 0;
	virtual void onExperimentDoc(const std::string& doc) = 0;

private:
	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	void processTitle(cDataBuffer& buffer);
	void processResearcher(cDataBuffer& buffer);
	void processCultivar(cDataBuffer& buffer);
	void processExperimentDoc(cDataBuffer& buffer);

private:
    cExperimentID         mBlockID;
};

