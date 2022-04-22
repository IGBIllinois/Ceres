
#include "ExperimentSerializer.hpp"
#include "ExperimentDataIdentifiers.hpp"
#include "./BlockDataFile/BlockDataFile.hpp"

#include <cassert>

using namespace experiment;

cExperimentSerializer::cExperimentSerializer()
:
    cBlockSerializer(),
    mBlockID(1,0)
{}

cExperimentSerializer::cExperimentSerializer(std::size_t n, cBlockDataFile* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
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

cBlockID& cExperimentSerializer::blockID()
{
    return mBlockID;
}






