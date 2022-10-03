

#include "AxisCommunicationsPropertyPage_Remote.hpp"


cAxisCommunicationsPropertyPage_Remote::cAxisCommunicationsPropertyPage_Remote(QWidget* parent)
	: cAxisCommunicationsPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cAxisPropertiesNetEncoder(255)
{}

bool cAxisCommunicationsPropertyPage_Remote::queryState()
{
	return true;
}

void cAxisCommunicationsPropertyPage_Remote::onCurrentState(bool valid, const std::string& mode,
	double min_deg, double max_deg)
{
	if (!valid) return;
}

void cAxisCommunicationsPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cAxisPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cAxisCommunicationsPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

