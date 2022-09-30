

#include "AxisCommunicationsPropertyPage_Remote.hpp"


cAxisCommunicationsPropertyPage_Remote::cAxisCommunicationsPropertyPage_Remote(QWidget* parent)
	: cAxisCommunicationsPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cAxisPropertiesNetEncoder(255)
{}

void cAxisCommunicationsPropertyPage_Remote::onCurrentState(bool valid, const std::string& mode,
	double min_deg, double max_deg)
{
	if (!valid) return;
}

int cAxisCommunicationsPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

