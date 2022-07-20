/**
 * @file
 * @brief Parser for reading Experiment data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "ExperimentBlockId.hpp"

#include <ctime>

 // Forward Declarations

struct sExperimentTime_t
{
	int year;		// Year
	int month;		// Month within year [1, 12]
	int day;		// Day of the month [1, 31]
	int hour;		// Hour since midnight [0, 23]
	int minutes;	// Minutes after the hour [0, 59]
	int seconds;	// Seconds after the minute [0, 59]
};

class cExperimentParser : public cBlockParser
{
public:
	cExperimentParser();
	~cExperimentParser() = default;

	cBlockID& blockID() override;

protected:
	virtual void onBeginHeader() = 0;
	virtual void onEndOfHeader() = 0;

	virtual void onBeginFooter() = 0;
	virtual void onEndOfFooter() = 0;

	virtual void onExperimentTitle(const std::string& title) = 0;
	virtual void onExperimentCultivar(const std::string& cultivar) = 0;
	virtual void onExperimentResearcher(const std::string& researcher) = 0;
	virtual void onExperimentDoc(const std::string& doc) = 0;

	virtual void onBeginSensorList() = 0;
	virtual void onEndOfSensorList() = 0;
	virtual void onSensorBlockInfo(unsigned int class_id, const std::string& name) = 0;


	virtual void onStartTime(sExperimentTime_t start_time) = 0;
	virtual void onEndTime(sExperimentTime_t end_time) = 0;

	virtual void onStartRecordingTimestamp(uint64_t timestamp) = 0;
	virtual void onEndRecordingTimestamp(uint64_t timestamp) = 0;

private:
	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	void processTitle(cDataBuffer& buffer);
	void processResearcher(cDataBuffer& buffer);
	void processCultivar(cDataBuffer& buffer);
	void processExperimentDoc(cDataBuffer& buffer);
	void processStartTime(cDataBuffer& buffer);
	void processEndTime(cDataBuffer& buffer);
	void processStartRecordingTimestamp(cDataBuffer& buffer);
	void processEndRecordingTimestamp(cDataBuffer& buffer);
	void processSensorBlockInfo(cDataBuffer& buffer);

private:
    cExperimentID         mBlockID;
};

