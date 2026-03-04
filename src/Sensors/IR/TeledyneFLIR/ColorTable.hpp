
#pragma once

#include <array>


class cColorTable
{
public:
	struct sColor_t
	{
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
	};

public:
	cColorTable();
	cColorTable(double min, double max);

	void setRange(double min, double max);

	sColor_t getColor(double v) const;

private:
	double mMinValue = 0;
	double mMaxValue = 0;
	double mDeltaValue = 0;

	std::array<sColor_t, 766> mColors;
};

