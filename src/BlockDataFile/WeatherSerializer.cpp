
#include "WeatherSerializer.hpp"
#include "WeatherDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace weather;

cWeatherSerializer::cWeatherSerializer()
:
    cBlockSerializer(),
    mBlockID(1,0)
{}

cWeatherSerializer::cWeatherSerializer(std::size_t n, cBlockDataFileWriter* pDataFile)
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

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::CONFIGURATION_INFO);

    mDataBuffer.clear();
    mDataBuffer << info;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeConfigInfo data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cWeatherSerializer::writeWindData_mps(bool valid, double speed_mps, double dir_deg)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);

    mBlockID.dataID(DataID::WIND_DATA_VALID);

    mDataBuffer.clear();
    mDataBuffer << valid;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeWindData_mps (valid) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_SPEED_MPS);

    mDataBuffer.clear();
    mDataBuffer << speed_mps;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeWindData_mps (speed) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_DIRECTION_DEG);
    mDataBuffer.clear();
    mDataBuffer << dir_deg;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeWindData_mps (direction) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cWeatherSerializer::writeWindData_knots(bool valid, double speed_knots, double dir_deg)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);

    mBlockID.dataID(DataID::WIND_DATA_VALID);
    mDataBuffer.clear();
    mDataBuffer << valid;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeWindData_knots (valid) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_SPEED_KNOTS);
    mDataBuffer.clear();
    mDataBuffer << speed_knots;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeWindData_knots (speed) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mBlockID.dataID(DataID::WIND_DIRECTION_DEG);
    mDataBuffer.clear();
    mDataBuffer << dir_deg;

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeWindData_knots (direction) data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

