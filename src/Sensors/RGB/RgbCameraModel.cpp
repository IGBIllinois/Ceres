
#include "RgbCameraModel.hpp"


cRgbCameraModel::cRgbCameraModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent)
{
	mLapseInterval_ms = 1000;
}

cRgbCameraModel::eMode cRgbCameraModel::mode() const
{
	return mMode;
}

double cRgbCameraModel::frameRate_Hz() const
{
	return mFrameRate_fps;
}

std::optional<double> cRgbCameraModel::minFrameRate_fps() const
{
	return mMinFrameRate_fps;
}

std::optional<double> cRgbCameraModel::maxFrameRate_fps() const
{
	return mMaxFrameRate_fps;
}

uint32_t cRgbCameraModel::lapseInterval_ms() const
{
	return mLapseInterval_ms;
}

