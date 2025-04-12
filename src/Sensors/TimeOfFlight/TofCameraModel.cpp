
#include "TofCameraModel.hpp"


namespace
{
	static uint8_t tof_id = 0;
}


cTofCameralModel::cTofCameralModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent), mDeviceID(++tof_id)
{
}

uint8_t cTofCameralModel::device_id() const
{
	return mDeviceID;
}



