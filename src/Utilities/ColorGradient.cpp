
#include "ColorGradient.hpp"

namespace 
{
	template<typename T>
	T bound(T input, T min, T max)
	{
		if (input < min) return min;
		if (input > max) return max;
		return input;
	}
}

cColorGradient::cColorGradient()
{
	mColorPoints.clear();
	mColorPoints.emplace_back(0.0f, 0.0f, 1.0f, 0.0f);	// Blue
	mColorPoints.emplace_back(0.0f, 1.0f, 1.0f, 0.25f);	// Cyan
	mColorPoints.emplace_back(0.0f, 1.0f, 0.0f, 0.5f);	// Green
	mColorPoints.emplace_back(1.0f, 1.0f, 0.0f, 0.75f);	// Yellow
	mColorPoints.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);	// Red
}

cColorGradient::~cColorGradient()
{
	mColorPoints.clear();
}

void cColorGradient::addColorPoint(float red, float green, float blue, float v)
{
	for (int i = 0; i < mColorPoints.size(); ++i)
	{
		if (v < mColorPoints[i].value)
		{
			mColorPoints.insert(mColorPoints.begin()+i, sColorPoint(red, green, blue, v));
		}
	}

	mColorPoints.emplace_back(red, green, blue, v);
}

void cColorGradient::clearGradient()
{
	mColorPoints.clear();
}

sRGB cColorGradient::getColorAtValue(const float v)
{
	if (mColorPoints.empty())
		return sRGB();

	if (v <= 0.0)
		return mColorPoints.front().color;

	if (v >= 1.0)
		return mColorPoints.back().color;

	for (int i = 1; i < mColorPoints.size(); ++i)
	{
		sColorPoint& current = mColorPoints[i];
		if (v < current.value)
		{
			sColorPoint& previous = mColorPoints[i-1];
			auto valueDiff = (current.value - previous.value);
			auto fractBetween = bound((v - previous.value) / valueDiff, 0.0f, 1.0f);

			float r = static_cast<float>((current.color.r - previous.color.r) * fractBetween + previous.color.r);
			float g = static_cast<float>((current.color.g - previous.color.g) * fractBetween + previous.color.g);
			float b = static_cast<float>((current.color.b - previous.color.b) * fractBetween + previous.color.b);

			return sRGB(r, g, b);
		}
	}

	return sRGB();
}
