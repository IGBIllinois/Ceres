

#include "HySpexCamera_PropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTimer>

cHySpexCamera_PropertyPage::cHySpexCamera_PropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{}

void cHySpexCamera_PropertyPage::createWidgets()
{
	/** Acquisition Info */
	mpAvgFramesLabel = new QLabel("Avg Frames", this);
	mpAvgFrames = new QLineEdit(this);

	mpFramePeriodLabel = new QLabel("Frame Period (us) :", this);
	mpFramePeriod_us = new QLineEdit(this);

	mpMinFramePeriodLabel = new QLabel("Min Frame Period (us) :", this);
	mpMinFramePeriod_us = new QLineEdit(this);
	mpMinFramePeriod_us->setReadOnly(true);

	mpIntegrationTimeLabel = new QLabel("Integration Time (us) :", this);
	mpIntegrationTime_us = new QLineEdit(this);

	mpMaxIntegrationTimeLabel = new QLabel("Max Integration Time (us) :", this);
	mpMaxIntegrationTime_us = new QLineEdit(this);
	mpMaxIntegrationTime_us->setReadOnly(true);

	/** Lens Info */
	mpLensLabel = new QLabel("Name");
	mpLenses = new QComboBox();

	mpShutter = new QPushButton("Unknown");
	connect(mpShutter, &QPushButton::pressed, this, &cHySpexCamera_PropertyPage::changeShutterState);

	/** Background Info */
	mpNumBackgroundsLabel = new QLabel("Backgrounds :", this);
	mpNumBackgrounds = new QLineEdit(this);

	mpDoBackground = new QPushButton("Calc Background");

//	mpDoBackground->setCheckable(true);
//	connect(mpDoBackground, &QPushButton::clicked, this, &cHySpexCamera_PropertyPage::doCalcBackground);
	connect(mpDoBackground, &QPushButton::pressed, this, &cHySpexCamera_PropertyPage::doCalcBackground);
}

void cHySpexCamera_PropertyPage::enableControls(bool enable)
{
	mpLenses->setEnabled(enable);
	mpAvgFrames->setEnabled(enable);
	mpFramePeriod_us->setEnabled(enable);
	mpMinFramePeriod_us->setEnabled(enable);
	mpIntegrationTime_us->setEnabled(enable);
	mpMaxIntegrationTime_us->setEnabled(enable);

	mpShutter->setEnabled(enable);

	mpNumBackgrounds->setEnabled(enable);
	mpDoBackground->setEnabled(enable);
}

QGroupBox* cHySpexCamera_PropertyPage::getLensLayout()
{
	QGroupBox* lensBox = new QGroupBox("Lens");

	auto* lensLayout = new QFormLayout();

	lensLayout->addRow(mpLensLabel, mpLenses);

	lensBox->setLayout(lensLayout);

	return lensBox;
}

QGroupBox* cHySpexCamera_PropertyPage::getAcquisitionLayout()
{
	QGroupBox* acqBox = new QGroupBox("Acquisition Parameters");

	auto* acqLayout = new QFormLayout();

	acqLayout->addRow(mpAvgFramesLabel, mpAvgFrames);
	acqLayout->addRow(mpFramePeriodLabel, mpFramePeriod_us);
	acqLayout->addRow(mpMinFramePeriodLabel, mpMinFramePeriod_us);
	acqLayout->addRow(mpIntegrationTimeLabel, mpIntegrationTime_us);
	acqLayout->addRow(mpMaxIntegrationTimeLabel, mpMaxIntegrationTime_us);

	acqBox->setLayout(acqLayout);

	return acqBox;
}

QGroupBox* cHySpexCamera_PropertyPage::getShutterLayout()
{
	QGroupBox* shutterBox = new QGroupBox("Shutter");

	auto* shutterLayout = new QHBoxLayout();

	shutterLayout->addWidget(mpShutter);

	shutterBox->setLayout(shutterLayout);

	return shutterBox;
}

QGroupBox* cHySpexCamera_PropertyPage::getBackgroundLayout()
{
	QGroupBox* bgBox = new QGroupBox("Background");

	auto* bgLayout = new QHBoxLayout();

	auto* numLayout = new QFormLayout();

	numLayout->addRow(mpNumBackgroundsLabel, mpNumBackgrounds);

	bgLayout->addLayout(numLayout);
	bgLayout->addWidget(mpDoBackground);

	bgBox->setLayout(bgLayout);

	return bgBox;
}

cExperimentState* cHySpexCamera_PropertyPage::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	return nullptr;
}
