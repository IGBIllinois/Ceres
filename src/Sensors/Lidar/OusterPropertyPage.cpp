

#include "OusterPropertyPage.hpp"


cOusterPropertyPage::cOusterPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{}

cExperimentState* cOusterPropertyPage::createState(const std::string& type)
{
	return nullptr;
}
