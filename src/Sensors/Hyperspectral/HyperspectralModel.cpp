
#include "HyperspectralModel.hpp"


namespace
{
	static uint8_t hyperspectral_id = 0;
}


cHyperspectralModel::cHyperspectralModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent)
{
}


