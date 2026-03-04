
#include "ColorTable.hpp"

cColorTable::cColorTable()
{
	for (int i = 0; i < 256; ++i)
	{
		mColors[i] = sColor_t(0, i, 255 - i);
	}

	for (int i = 1; i < 256; ++i)
	{
		mColors[i + 255] = sColor_t(i, 255, 0);
	}

	for (int i = 1; i < 256; ++i)
	{
		mColors[i + 510] = sColor_t(255, 255 - i, 0);
	}
}

cColorTable::cColorTable(double min, double max) : cColorTable()
{
	mMinValue = min;
	mMaxValue = max;

	mDeltaValue = mMaxValue - mMinValue;
}

void cColorTable::setRange(double min, double max)
{
	mMinValue = min;
	mMaxValue = max;

	mDeltaValue = mMaxValue - mMinValue;
}

cColorTable::sColor_t cColorTable::getColor(double v) const
{
	if (v <= mMinValue) return mColors.front();
	if (v >= mMaxValue) return mColors.back();

	double x = (v - mMinValue) / mDeltaValue;
	int index = static_cast<int>(x * mColors.size());

	return mColors[index];
}


