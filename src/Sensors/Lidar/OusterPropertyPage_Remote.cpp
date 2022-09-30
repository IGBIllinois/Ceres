

#include "OusterPropertyPage_Remote.hpp"


cOusterPropertyPage_Remote::cOusterPropertyPage_Remote(QWidget* parent)
	: cOusterPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cOusterPropertiesNetEncoder(255)
{}

void cOusterPropertyPage_Remote::onCurrentState(bool valid, const std::string& mode,
	double min_deg, double max_deg)
{
	if (!valid) return;
}

int cOusterPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

