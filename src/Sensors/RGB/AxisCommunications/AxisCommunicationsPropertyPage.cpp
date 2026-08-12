

#include "AxisCommunicationsPropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>


cAxisCommunicationsPropertyPage::cAxisCommunicationsPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{
}

void cAxisCommunicationsPropertyPage::createWidgets()
{
	mpModeLabel = new QLabel("Mode:", this);
	mpMode = new QComboBox(this);
	mpMode->addItem("Photo");
	mpMode->addItem("Time Lapse");
	mpMode->addItem("Video");
	connect(mpMode, &QComboBox::currentTextChanged, this, &cAxisCommunicationsPropertyPage::modeTextChanged);

	mpCameraIdLabel = new QLabel("Camera ID:", this);
	mpCameraId = new QLineEdit(this);

	mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
	mpImageSizes = new QComboBox(this);
	mpImageSizes->addItem("1920x1080");
	mpImageSizes->addItem("1280x720");
	mpImageSizes->addItem("1024x768");
	mpImageSizes->addItem("1024x640");
	mpImageSizes->addItem("800x600");
	mpImageSizes->addItem("640x480");

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);

	mpLapseIntervalLabel = new QLabel("Time-Lapse Interval (s):", this);;
	mpLapseInterval_s = new QLineEdit(this);

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cAxisCommunicationsPropertyPage::onGrabImagePressed);
}

void cAxisCommunicationsPropertyPage::enableControls(bool enable)
{
	mpMode->setEnabled(enable);
	mpCameraId->setEnabled(enable);
	mpImageSizes->setEnabled(enable);
	mpFrameRate_fps->setEnabled(enable);
	mpLapseInterval_s->setEnabled(enable);
	mpGrabImage->setEnabled(enable);
}

void cAxisCommunicationsPropertyPage::doLayout()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();

	auto* idLayout = new QHBoxLayout();
	idLayout->addWidget(mpCameraIdLabel);
	idLayout->addWidget(mpCameraId);
	pMainLayout->addLayout(idLayout);

	auto* sizeLayout = new QHBoxLayout();
	sizeLayout->addWidget(mpImageSizeLabel);
	sizeLayout->addWidget(mpImageSizes);
	pMainLayout->addLayout(sizeLayout);

	pMainLayout->addSpacing(5);

	auto* pLayout = new QFormLayout();

	pLayout->addRow(mpModeLabel, mpMode);
	pLayout->addRow(mpFrameRateLabel, mpFrameRate_fps);
	pLayout->addRow(mpLapseIntervalLabel, mpLapseInterval_s);
	pMainLayout->addLayout(pLayout);

	pMainLayout->addWidget(mpGrabImage);
	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpButtons);

	setLayout(pMainLayout);
}

cExperimentState* cAxisCommunicationsPropertyPage::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	return nullptr;
}

void cAxisCommunicationsPropertyPage::modeTextChanged(const QString& text)
{
	if (text == "Photo")
	{
		mpFrameRate_fps->setEnabled(false);
		mpLapseInterval_s->setEnabled(false);
	}
	else if (text == "Time Lapse")
	{
		mpFrameRate_fps->setEnabled(false);
		mpLapseInterval_s->setEnabled(true);
	}
	else if (text == "Video")
	{
		mpLapseInterval_s->setEnabled(false);
		mpFrameRate_fps->setEnabled(true);
	}
}


