

#include "OusterPropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>

cOusterPropertyPage::cOusterPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{
	mpModeLabel = new QLabel("Lidar Mode:", this);
	mpModes = new QComboBox(this);
	mpModes->addItem("512x10");
	mpModes->addItem("512x20");
	mpModes->addItem("1024x10");
	mpModes->addItem("1024x20");
	mpModes->addItem("2048x10");

	mpMinAzimuthAngleLabel = new QLabel("Min (deg):", this);
	mpMinAzimuthAngle_deg = new QLineEdit(this);
	mpMaxAzimuthAngleLabel = new QLabel("Max (deg):", this);
	mpMaxAzimuthAngle_deg = new QLineEdit(this);


	mpAzimuthWindow = new QGroupBox("Azimuth Window");
	auto* azWindowLayout = new QFormLayout();
	azWindowLayout->addRow(mpMinAzimuthAngleLabel, mpMinAzimuthAngle_deg);
	azWindowLayout->addRow(mpMaxAzimuthAngleLabel, mpMaxAzimuthAngle_deg);
	mpAzimuthWindow->setLayout(azWindowLayout);
}

void cOusterPropertyPage::doLayout()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();

	auto* modeLayout = new QHBoxLayout();

	modeLayout->addWidget(mpModeLabel);
	modeLayout->addWidget(mpModes);

	pMainLayout->addLayout(modeLayout);

	auto* azLayout = new QHBoxLayout();
	azLayout->addWidget(mpAzimuthWindow);

	pMainLayout->addLayout(azLayout);

	pMainLayout->addWidget(mpButtons);

	setLayout(pMainLayout);
}

cExperimentState* cOusterPropertyPage::createState(const std::string& type)
{
	return nullptr;
}
