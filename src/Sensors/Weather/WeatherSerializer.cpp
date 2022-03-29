
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

void cWeatherSerializer::setVersion(uint8_t major, uint8_t minor)
{
	mBlockID.setVersion(major, minor);
}

void cWeatherSerializer::writeConfigInfo(const std::string& info)
{
    assert(mpDataFile);

	mBlockID.dataID(DataID::CONFIGURATION_INFO);

    mDataBuffer.clear();
    mDataBuffer << info;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}


