
#include "ThermalColorTable.hpp"

cThermalColorTable::cThermalColorTable(uint16_t table_size)
 : mTableSize(table_size)
{}

cThermalColorTable::cThermalColorTable(double min, double max, uint16_t table_size)
	: cThermalColorTable(table_size)
{
	mMinValue = min;
	mMaxValue = max;

	mDeltaValue = mMaxValue - mMinValue;
}

double cThermalColorTable::getMinValue() const { return mMinValue; }
double cThermalColorTable::getMaxValue() const { return mMaxValue; }

void cThermalColorTable::setRange(double min, double max)
{
	mMinValue = min;
	mMaxValue = max;

	mDeltaValue = mMaxValue - mMinValue;
}

cThermalColorTable::sColor_t cThermalColorTable::getColorRGB(double v) const
{
	uint32_t color = getColorValue(v);

	uint8_t red = (color & 0x00ff0000) >> 16;
	uint8_t green = (color & 0x0000ff00) >> 7;
	uint8_t blue = (color & 0x000000ff);

	return { red, green, blue };
}

int cThermalColorTable::getIndex(double v) const
{
	if (v <= mMinValue) return 0;
	if (v >= mMaxValue) return mTableSize-1;

	double x = (v - mMinValue) / mDeltaValue;
	return static_cast<int>(x * mTableSize);
}


