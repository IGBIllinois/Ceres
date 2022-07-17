
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
}

void cExperimentSerializer::writeEndOfHeader()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_OF_HEADER);
    mpDataFile->writeBlock(mBlockID);
}


void cExperimentSerializer::writeBeginFooter()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::BEGIN_FOOTER);
    mpDataFile->writeBlock(mBlockID);
}

void cExperimentSerializer::writeEndOfFooter()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_OF_FOOTER);
    mpDataFile->writeBlock(mBlockID);
}

void cExperimentSerializer::writeTitle(const std::string& title)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::EXPERIMENT_TITLE);

    mDataBuffer.clear();
    mDataBuffer << title;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::writeResearcher(const std::string& researcher)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::RESEARCHER);

    mDataBuffer.clear();
    mDataBuffer << researcher;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::writeCultivar(const std::string& cultivar)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::CULTIVAR);

    mDataBuffer.clear();
    mDataBuffer << cultivar;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::writeExperimentDoc(const std::string& doc)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::EXPERIMENT);

    mDataBuffer.clear();
    mDataBuffer << doc;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::writeBeginSensorList()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::BEGIN_SENSOR_LIST);
    mpDataFile->writeBlock(mBlockID);
}

void cExperimentSerializer::writeEndOfSensorList()
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_OF_SENSOR_LIST);
    mpDataFile->writeBlock(mBlockID);
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
}


void cExperimentSerializer::startTime(time_t time)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::START_TIME);

    tm* ltm = localtime(&time);

    mDataBuffer.clear();

    mDataBuffer << (ltm->tm_year + 1900);
    mDataBuffer << (ltm->tm_mon + 1);
    mDataBuffer << ltm->tm_mday;
    mDataBuffer << ltm->tm_hour;
    mDataBuffer << ltm->tm_min;
    mDataBuffer << ltm->tm_sec;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::endTime(time_t time)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_TIME);

    tm* ltm = localtime(&time);

    mDataBuffer.clear();

    mDataBuffer << (ltm->tm_year + 1900);
    mDataBuffer << (ltm->tm_mon + 1);
    mDataBuffer << ltm->tm_mday;
    mDataBuffer << ltm->tm_hour;
    mDataBuffer << ltm->tm_min;
    mDataBuffer << ltm->tm_sec;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

}

void cExperimentSerializer::startRecordingTimestamp(uint64_t timestamp)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::START_RECORDING_TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << timestamp;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::endRecordingTimestamp(uint64_t timestamp)
{
    assert(mpDataFile);

    setVersion(1, 0);
    mBlockID.dataID(DataID::END_RECORDING_TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << timestamp;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}


cBlockID& cExperimentSerializer::blockID()
{
    return mBlockID;
}






