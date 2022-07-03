
#include "WeatherParser.hpp"
#include "WeatherDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace weather;

cWeatherParser::cWeatherParser()
:
    cBlockParser()
{}

cBlockID& cWeatherParser::blockID()
{
    return mBlockID;
}

void cWeatherParser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    switch (static_cast<weather::DataID>(data_id))
    {
    }
}

void cWeatherParser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
//    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}


/*
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
*/
