
#include "MultispectralModel.hpp"


namespace
{
	static uint8_t multispectral_id = 0;
}



cMultispectralModel::cMultispectralModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent), mDeviceID(++multispectral_id)
{
}

uint8_t cMultispectralModel::device_id() const
{
	return mDeviceID;
}

