
#pragma once

#include "ThermalColorTypes.hpp"

#include <cstdint>


class cThermalColorTable
{
public:
	struct sColor_t
	{
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
	};

public:
	cThermalColorTable(uint16_t table_size);
	cThermalColorTable(double min, double max, uint16_t table_size);
	virtual ~cThermalColorTable() {};

	virtual eColorTable getType() const = 0;

	double getMinValue() const;
	double getMaxValue() const;

	void setRange(double min, double max);

	virtual uint32_t getColorValue(double v) const = 0;
	
	sColor_t getColorRGB(double v) const;

protected:
	int getIndex(double v) const;

protected:
	const uint16_t mTableSize;

private:
	double mMinValue = 0;
	double mMaxValue = 0;
	double mDeltaValue = 0;
};

