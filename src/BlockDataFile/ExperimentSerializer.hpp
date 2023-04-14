/**
 * @file
 * @brief Serializer for storing Experiment data into a block based data file
 */
#pragma once

#include "BlockSerializer.hpp"
#include "ExperimentBlockId.hpp"

#include <ctime>

 // Forward Declarations


class cExperimentSerializer : public cBlockSerializer
{
public:
	cExperimentSerializer();
	explicit cExperimentSerializer(std::size_t n, cBlockDataFileWriter* pDataFile = nullptr);
	~cExperimentSerializer() = default;

	void writeBeginHeader();
	void writeEndOfHeader();

	void writeBeginFooter();
	void writeEndOfFooter();

	void writeTitle(const std::string& title);
	void writeResearcher(const std::string& researcher);
	void writeCultivar(const std::string& cultivar);
	void writeExperimentDoc(const std::string& doc);

	void writeDate(std::uint16_t year, std::uint8_t month, std::uint8_t day);
	void writeDayOfYear(std::uint16_t doy);
	void writeTime(std::uint8_t hour, std::uint8_t minute, std::uint8_t seconds);

	void writeBeginSensorList();
	void writeEndOfSensorList();
	void writeSensorBlockInfo(uint16_t class_id, const std::string& name);

	void startTime(time_t time);
	void endTime(time_t time);

	void startRecordingTimestamp(uint64_t timestamp_ns);
	void endRecordingTimestamp(uint64_t timestamp_ns);
	void heartbeatTimestamp(uint64_t timestamp_ns);

protected:
	cBlockID& blockID() override;

private:
    cExperimentID         mBlockID;
};

