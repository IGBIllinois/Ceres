
#include "WeatherSerializer.hpp"
#include "WeatherDataIdentifiers.hpp"
#include "../../BlockDataFile/BlockDataFile.hpp"

#include <cassert>

using namespace weather;

cWeatherSerializer::cWeatherSerializer()
:
    cBlockSerializer(),
    mBlockID(1,0)
{}

cWeatherSerializer::cWeatherSerializer(std::size_t n, cBlockDataFile* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

cBlockID& cWeatherSerializer::blockID()
{
    return mBlockID;
}

void cWeatherSerializer::writeConfigInfo(const std::string& info)
{
    assert(mpDataFile);

	mBlockID.dataID(DataID::CONFIGURATION_INFO);

    mDataBuffer.clear();
    mDataBuffer << info;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cWeatherSerializer::writeWindData_mps(bool valid, double speed_mps, double dir_deg)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::WIND_DATA_VALID);
    mDataBuffer.clear();
    mDataBuffer << valid;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_SPEED_MPS);
    mDataBuffer.clear();
    mDataBuffer << speed_mps;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_DIRECTION_DEG);
    mDataBuffer.clear();
    mDataBuffer << dir_deg;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cWeatherSerializer::writeWindData_knots(bool valid, double speed_knots, double dir_deg)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::WIND_DATA_VALID);
    mDataBuffer.clear();
    mDataBuffer << valid;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_SPEED_KNOTS);
    mDataBuffer.clear();
    mDataBuffer << speed_knots;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_DIRECTION_DEG);
    mDataBuffer.clear();
    mDataBuffer << dir_deg;
    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

