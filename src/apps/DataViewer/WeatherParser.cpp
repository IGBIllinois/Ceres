
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
    mBlockID.setVersion(major_version, minor_version);
    mBlockID.dataID(static_cast<weather::DataID>(data_id));

    switch (static_cast<weather::DataID>(data_id))
    {
    case DataID::WIND_DATA_VALID:
        processWindDataValid(buffer);
        break;
    case DataID::WIND_SPEED_MPS:
        processWindSpeed_mps(buffer);
        break;
    case DataID::WIND_SPEED_KNOTS:
        processWindSpeed_knots(buffer);
        break;
    case DataID::WIND_DIRECTION_DEG:
        processWindDirection_deg(buffer);
        break;
    }
}

void cWeatherParser::processWindDataValid(cDataBuffer& buffer)
{
    buffer >> mWindDataValid;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processWindDataValid.");
}

void cWeatherParser::processWindSpeed_mps(cDataBuffer& buffer)
{
    double speed_mps = 0;
    buffer >> speed_mps;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processWindSpeed_mps.");

    onWindSpeed_mps(mWindDataValid, speed_mps);
}

void cWeatherParser::processWindSpeed_knots(cDataBuffer& buffer)
{
    double speed_knots = 0;
    buffer >> speed_knots;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processWindSpeed_knots.");

    onWindSpeed_knots(mWindDataValid, speed_knots);
}

void cWeatherParser::processWindDirection_deg(cDataBuffer& buffer)
{
    double direction_deg = 0;
    buffer >> direction_deg;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processWindDirection_deg.");

    onWindDirection_deg(mWindDataValid, direction_deg);
}


