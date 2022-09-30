

#include "AxisCommunicationsPropertyPage.hpp"


cAxisCommunicationsPropertyPage::cAxisCommunicationsPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{}

cExperimentState* cAxisCommunicationsPropertyPage::createState(const std::string& type)
{
	return nullptr;
}
