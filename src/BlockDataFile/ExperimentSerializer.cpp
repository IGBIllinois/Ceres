
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

void cExperimentSerializer::writeTitle(const std::string& title)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::EXPERIMENT_TITLE);

    mDataBuffer.clear();
    mDataBuffer << title;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::writeResearcher(const std::string& researcher)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::RESEARCHER);

    mDataBuffer.clear();
    mDataBuffer << researcher;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::writeCultivar(const std::string& cultivar)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::CULTIVAR);

    mDataBuffer.clear();
    mDataBuffer << cultivar;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::writeExperimentDoc(const std::string& doc)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::EXPERIMENT);

    mDataBuffer.clear();
    mDataBuffer << doc;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::startTime(time_t time)
{
    assert(mpDataFile);

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

void cExperimentSerializer::startTimestamp(uint64_t timestamp)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::START_RECORDING_TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << timestamp;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cExperimentSerializer::endTimestamp(uint64_t timestamp)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::END_RECORDING_TIMESTAMP);

    mDataBuffer.clear();
    mDataBuffer << timestamp;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}


cBlockID& cExperimentSerializer::blockID()
{
    return mBlockID;
}






