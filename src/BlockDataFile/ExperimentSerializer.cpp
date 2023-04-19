
#include "ExperimentSerializer.hpp"
#include "ExperimentDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace experiment;

cExperimentSerializer::cExperimentSerializer()
:
    cBlockSerializer(),
    mBlockID(1,0)
{}

cExperimentSerializer::cExperimentSerializer(std::size_t n, cBlockDataFileWriter* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

void cExperimentSerializer::writeBeginHeader()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::BEGIN_HEADER);
    mpDataFile->writeBlock(mBlockID);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeBeginHeader data.");
}

void cExperimentSerializer::writeEndOfHeader()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_OF_HEADER);
    mpDataFile->writeBlock(mBlockID);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeEndOfHeader data.");
}


void cExperimentSerializer::writeBeginFooter()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::BEGIN_FOOTER);
    mpDataFile->writeBlock(mBlockID);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeBeginFooter data.");
}

void cExperimentSerializer::writeEndOfFooter()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_OF_FOOTER);
    mpDataFile->writeBlock(mBlockID);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeEndOfFooter data.");
}

void cExperimentSerializer::writeTitle(const std::string& title)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::EXPERIMENT_TITLE);

    mDataBuffer.clear();
    mDataBuffer << title;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeTitle data.");
}

void cExperimentSerializer::writeResearcher(const std::string& researcher)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::RESEARCHER);

    mDataBuffer.clear();
    mDataBuffer << researcher;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeResearcher data.");
}

void cExperimentSerializer::writeCultivar(const std::string& cultivar)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::CULTIVAR);

    mDataBuffer.clear();
    mDataBuffer << cultivar;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeCultivar data.");
}

void cExperimentSerializer::writeExperimentDoc(const std::string& doc)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::EXPERIMENT);

    mDataBuffer.clear();

    if (mDataBuffer.capacity() < doc.size())
    {
        uint16_t n = doc.size() / 1024;
        ++n;
        mDataBuffer.capacity(n * 1024);
    }

    mDataBuffer << doc;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeExperimentDoc data.");
}

void cExperimentSerializer::writeDate(std::uint16_t year, std::uint8_t month, std::uint8_t day)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::EXPERIMENT_DATE);

    mDataBuffer.clear();
    mDataBuffer << year;
    mDataBuffer << month;
    mDataBuffer << day;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeDate data.");
}

void cExperimentSerializer::writeDayOfYear(std::uint16_t doy)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::EXPERIMENT_DAY_OF_YEAR);

    mDataBuffer.clear();
    mDataBuffer << doy;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeDayOfYear data.");
}

void cExperimentSerializer::writeTime(std::uint8_t hour, std::uint8_t minute, std::uint8_t seconds)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::EXPERIMENT_TIME);

    mDataBuffer.clear();
    mDataBuffer << hour;
    mDataBuffer << minute;
    mDataBuffer << seconds;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeTime data.");
}

void cExperimentSerializer::writeBeginSensorList()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::BEGIN_SENSOR_LIST);
    mpDataFile->writeBlock(mBlockID);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeBeginSensorList data.");
}

void cExperimentSerializer::writeEndOfSensorList()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_OF_SENSOR_LIST);
    mpDataFile->writeBlock(mBlockID);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeEndOfSensorList data.");
}

void cExperimentSerializer::writeSensorBlockInfo(uint16_t class_id, const std::string& name)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::SENSOR_DATA_BLOCK_INFO);

    mDataBuffer.clear();
    mDataBuffer << class_id;
    mDataBuffer << name;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeSensorBlockInfo data.");
}


void cExperimentSerializer::startTime(time_t time)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::START_TIME);

    tm* ltm = localtime(&time);

    mDataBuffer.clear();

    mDataBuffer << static_cast<int32_t>(ltm->tm_year + 1900);
    mDataBuffer << static_cast<int32_t>(ltm->tm_mon + 1);
    mDataBuffer << static_cast<int32_t>(ltm->tm_mday);
    mDataBuffer << static_cast<int32_t>(ltm->tm_hour);
    mDataBuffer << static_cast<int32_t>(ltm->tm_min);
    mDataBuffer << static_cast<int32_t>(ltm->tm_sec);

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing startTime data.");
}

void cExperimentSerializer::endTime(time_t time)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_TIME);

    tm* ltm = localtime(&time);

    mDataBuffer.clear();

    mDataBuffer << static_cast<int32_t>(ltm->tm_year + 1900);
    mDataBuffer << static_cast<int32_t>(ltm->tm_mon + 1);
    mDataBuffer << static_cast<int32_t>(ltm->tm_mday);
    mDataBuffer << static_cast<int32_t>(ltm->tm_hour);
    mDataBuffer << static_cast<int32_t>(ltm->tm_min);
    mDataBuffer << static_cast<int32_t>(ltm->tm_sec);

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing endTime data.");
}

void cExperimentSerializer::startRecordingTimestamp(uint64_t timestamp_ns)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::START_RECORDING_TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << timestamp_ns;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing startRecordingTimestamp data.");
}

void cExperimentSerializer::endRecordingTimestamp(uint64_t timestamp_ns)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_RECORDING_TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << timestamp_ns;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing endRecordingTimestamp data.");
}

void cExperimentSerializer::heartbeatTimestamp(uint64_t timestamp_ns)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::RECORDING_HEARTBEAT_TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << timestamp_ns;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing heartbeatTimestamp data.");
}


cBlockID& cExperimentSerializer::blockID()
{
    return mBlockID;
}






