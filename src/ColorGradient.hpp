
#pragma once

#include <cstdint>
#include <vector>



/******************************************************************************
 *
 * cColorGradient:
 *
 * The data buffer will auto
 *
 ******************************************************************************/

struct sRGB
{
	float r, g, b;		// Red, green and blue color values

	sRGB() : r(0.0f), g(0.0f), b(0.0f) {};
	sRGB(float red, float green, float blue) : r(red), g(green), b(blue) {};
};

class cColorGradient
{
public:
	cColorGradient();
	~cColorGradient();

	void addColorPoint(float red, float green, float blue, float v);
	void clearGradient();
	sRGB getColorAtValue(const float v);

private:
	struct sColorPoint
	{
		sRGB color;
		float value;
		sColorPoint(float red, float green, float blue, float v)
			: color(red, green, blue), value(v)
		{}
	};

	std::vector<sColorPoint> mColorPoints;
};



