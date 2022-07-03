/**
 * @file
 * @brief Parser for reading weather data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "WeatherBlockId.hpp"

#include <string>

// Forward Declarations


class cWeatherParser : public cBlockParser
{
public:
	cWeatherParser();
	~cWeatherParser() = default;

	cBlockID& blockID() override;

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void process_DataField(cDataBuffer& buffer);

/*
	void writeConfigInfo(const std::string& info);
	void writeWindData_mps(bool valid, double speed_mps, double dir_deg);
	void writeWindData_knots(bool valid, double speed_knots, double dir_deg);
*/

private:
    cWeatherID       mBlockID;
};

