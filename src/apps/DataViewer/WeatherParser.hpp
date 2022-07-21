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

protected:
	virtual void onWindSpeed_mps(bool valid, double speed_mps) = 0;
	virtual void onWindSpeed_knots(bool valid, double speed_knots) = 0;
	virtual void onWindDirection_deg(bool valid, double dir_deg) = 0;

private:
	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	void processWindDataValid(cDataBuffer& buffer);
	void processWindSpeed_mps(cDataBuffer& buffer);
	void processWindSpeed_knots(cDataBuffer& buffer);
	void processWindDirection_deg(cDataBuffer& buffer);

private:
    cWeatherID       mBlockID;

	bool mWindDataValid = false;
};

