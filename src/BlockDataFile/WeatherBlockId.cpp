
#include "WeatherBlockId.hpp"

#include "ClassIdentifiers.hpp"


cWeatherID::cWeatherID(uint8_t majorVer, uint8_t minorVer)
:
    cBlockID(ClassIDs::WEATHER, majorVer, minorVer)
{}

void cWeatherID::dataID(weather::DataID id) noexcept
{
    cBlockID::dataID(static_cast<uint16_t>(id));
}


