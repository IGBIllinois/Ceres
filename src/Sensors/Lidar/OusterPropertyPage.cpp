

#include "OusterPropertyPage.hpp"

#include <QComboBox>
#include <QTextEdit>

cOusterPropertyPage::cOusterPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{
	mpModes = new QComboBox(this);
	mpModes->addItem("512x10");
	mpModes->addItem("512x20");
	mpModes->addItem("1024x10");
	mpModes->addItem("1024x20");
	mpModes->addItem("2048x10");

	mpMinAzimuthAngle_deg = new QTextEdit(this);
	mpMaxAzimuthAngle_deg = new QTextEdit(this);
}

cExperimentState* cOusterPropertyPage::createState(const std::string& type)
{
	return nullptr;
}
