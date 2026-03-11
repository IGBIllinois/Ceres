

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
	mpModeLabel = new QLabel("Mode:", this);
	mpMode = new QComboBox(this);
	mpMode->addItem("Photo");
	mpMode->addItem("Time Lapse");
	mpMode->addItem("Video");

	mpImageSizeLabel = new QLabel("Image Size (w x d):", this);
	mpImageSize = new QLineEdit(this);
	mpImageSize->setReadOnly(true);

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);

	mpFrameIntervalLabel = new QLabel("Frame Interval (s):", this);;
	mpFrameInterval_s = new QLineEdit(this);

	mpThermalRangeLabel = new QLabel("Thermal Range (K):", this);
	mpThermalRange = new QLineEdit(this);
	mpThermalRange->setReadOnly(true);

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cTeledyneFlirPropertyPage::requestImage);
}

void cTeledyneFlirPropertyPage::enableControls(bool enable)
{
	mpMode->setEnabled(enable);
	mpFrameRate_fps->setEnabled(enable);
	mpFrameInterval_s->setEnabled(enable);
	mpGrabImage->setEnabled(enable);
}

void cTeledyneFlirPropertyPage::doLayout()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();

	auto* pLayout = new QFormLayout();

	pLayout->addRow(mpModeLabel, mpMode);
	pLayout->addRow(mpImageSizeLabel, mpImageSize);
	pLayout->addRow(mpFrameRateLabel, mpFrameRate_fps);
	pLayout->addRow(mpFrameIntervalLabel, mpFrameInterval_s);
	pLayout->addRow(mpThermalRangeLabel, mpThermalRange);
	pMainLayout->addLayout(pLayout);

	pMainLayout->addWidget(mpGrabImage);
	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpButtons);

	setLayout(pMainLayout);
}


