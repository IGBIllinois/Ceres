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

	bool hasBeginHeader() const { return mHasBeginHeader; }
	bool hasEndOfHeader() const { return mHasEndOfHeader; }

	bool hasBeginFooter() const { return mHasBeginFooter; }
	bool hasEndOfFooter() const { return mHasEndOfFooter; }

	const std::string& title() const { return mTitle; }
	const std::string& researcher() const { return mResearcher; }
	const std::string& cultivar() const { return mCultivar; }
	const std::string& experimentDoc() const { return mExperimentDoc; }

	/*
	void writeBeginSensorList();
	void writeEndOfSensorList();
	void writeSensorBlockInfo(uint16_t class_id, const std::string& name);

	void startTime(time_t time);
	void endTime(time_t time);

	void startRecordingTimestamp(uint64_t timestamp);
	void endRecordingTimestamp(uint64_t timestamp);
	*/


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

	virtual void processTitle(cDataBuffer& buffer);
	virtual void processResearcher(cDataBuffer& buffer);
	virtual void processCultivar(cDataBuffer& buffer);
	virtual void processExperimentDoc(cDataBuffer& buffer);

private:
    cExperimentID         mBlockID;

	bool mHasBeginHeader = false;
	bool mHasEndOfHeader = false;

	bool mHasBeginFooter = false;
	bool mHasEndOfFooter = false;

	bool mHasBeginSensorList = false;
	bool mHasEndOfSensorList = false;

	std::string mTitle;
	std::string mResearcher;
	std::string mCultivar;
	std::string mExperimentDoc;

};

