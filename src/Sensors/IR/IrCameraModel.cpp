
#include "IrCameraModel.hpp"


namespace
{
	static uint8_t ir_camera_id = 0;
}


cIrCameralModel::cIrCameralModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent), mDeviceID(++ir_camera_id)
{
}

uint8_t cIrCameralModel::device_id() const
{
	return mDeviceID;
}



