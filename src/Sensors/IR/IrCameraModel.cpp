
#include "IrCameraModel.hpp"


namespace
{
	static uint8_t ir_camera_id = 0;
}


cIrCameraModel::cIrCameraModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent), mDeviceID(++ir_camera_id)
{
}

uint8_t cIrCameraModel::device_id() const
{
	return mDeviceID;
}



