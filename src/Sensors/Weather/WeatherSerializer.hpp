/**
 * @file
 * @brief Serializer for storing weather data into a block based data file
 */
#pragma once

#include "../../BlockDataFile/BlockSerializer.hpp"
#include "WeatherBlockId.hpp"

#include <string>

// Forward Declarations


class cWeatherSerializer : public cBlockSerializer
{
public:
	cWeatherSerializer();
	explicit cWeatherSerializer(std::size_t n, cBlockDataFile* pDataFile = nullptr);
	~cWeatherSerializer() = default;

	void setVersion(uint8_t major, uint8_t minor);

	void writeConfigInfo(const std::string& info);
	void writeWindData_mps(bool valid, double speed_mps, double dir_deg);
	void writeWindData_knots(bool valid, double speed_knots, double dir_deg);

private:
    cWeatherID       mBlockID;
};

