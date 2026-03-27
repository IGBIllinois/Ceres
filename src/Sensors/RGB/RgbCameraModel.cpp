
#include "RgbCameraModel.hpp"


cRgbCameraModel::cRgbCameraModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent)
{
}

cRgbCameraModel::eMode cRgbCameraModel::mode() const
{
	return mMode;
}

double cRgbCameraModel::frameRate_Hz() const
{
	return mFrameRate_fps;
}

uint32_t cRgbCameraModel::lapseInterval_ms() const
{
	return mLapseInterval_ms;
}

