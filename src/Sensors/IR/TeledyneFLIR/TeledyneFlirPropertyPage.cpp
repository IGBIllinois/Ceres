

#include "TeledyneFlirPropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>


cTeledyneFlirPropertyPage::cTeledyneFlirPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{
}

void cTeledyneFlirPropertyPage::createWidgets()
{
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

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cTeledyneFlirPropertyPage::requestImage);
}

void cTeledyneFlirPropertyPage::enableControls(bool enable)
{
	mpCameraId->setEnabled(enable);
	mpImageSizes->setEnabled(enable);
	mpFrameRate_fps->setEnabled(enable);
	mpGrabImage->setEnabled(enable);
}

void cTeledyneFlirPropertyPage::doLayout()
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

	auto* frLayout = new QHBoxLayout();
	frLayout->addWidget(mpFrameRateLabel);
	frLayout->addWidget(mpFrameRate_fps);
	pMainLayout->addLayout(frLayout);

	pMainLayout->addWidget(mpGrabImage);
	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpButtons);

	setLayout(pMainLayout);
}

cExperimentState* cTeledyneFlirPropertyPage::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	return nullptr;
}

void cTeledyneFlirPropertyPage::requestImage()
{ }
