

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

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);

	mpFrameIntervalLabel = new QLabel("Frame Interval (s):", this);;
	mpFrameInterval_s = new QLineEdit(this);

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

	auto* idLayout = new QHBoxLayout();
	idLayout->addWidget(mpModeLabel);
	idLayout->addWidget(mpMode);
	pMainLayout->addLayout(idLayout);

	auto* frLayout = new QHBoxLayout();
	frLayout->addWidget(mpFrameRateLabel);
	frLayout->addWidget(mpFrameRate_fps);
	pMainLayout->addLayout(frLayout);

	auto* fiLayout = new QHBoxLayout();
	fiLayout->addWidget(mpFrameIntervalLabel);
	fiLayout->addWidget(mpFrameInterval_s);
	pMainLayout->addLayout(fiLayout);

	pMainLayout->addWidget(mpGrabImage);
	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpButtons);

	setLayout(pMainLayout);
}

cExperimentState* cTeledyneFlirPropertyPage::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	return nullptr;
}

