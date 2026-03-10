
#include "TeledyneFlirStatusView.hpp"
#include "TeledyneFlirCameraModel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QComboBox>

#include <string>


cTeledyneFlirStatusView::cTeledyneFlirStatusView(cTeledyneFlirCameraModel* pModel, QWidget* parent)
:
	cSensorStatusView(pModel, parent)
{
}

cTeledyneFlirStatusView::~cTeledyneFlirStatusView()
{}

void cTeledyneFlirStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpModeLabel = new QLabel("Mode:", this);
	mpMode = new QComboBox(this);
	mpMode->addItem("Photo");
	mpMode->addItem("Time Lapse");
	mpMode->addItem("Continuous");

	mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
	mpImageSizes = new QLineEdit(this);
	mpImageSizes->setReadOnly(true);

	mpFrameIntervalLabel = new QLabel("Frame Interval (sec):", this);
	mpFrameInterval_s = new QLineEdit(this);
	mpFrameInterval_s->setReadOnly(true);

	mpFrameRateLabel = new QLabel("Frame Rate (f/s):", this);
	mpFrameRate_fps = new QLineEdit(this);
	mpFrameRate_fps->setReadOnly(true);

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cTeledyneFlirStatusView::requestImage);

	mpThermalImage = new cThermalImageWidget(this);
}

void cTeledyneFlirStatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	QGroupBox* infoBox = new QGroupBox("Camera Information");

	auto* cameraInfoLayout = new QHBoxLayout();

	cameraInfoLayout->addWidget(mpModeLabel);
	cameraInfoLayout->addWidget(mpMode);

	cameraInfoLayout->addWidget(mpImageSizeLabel);
	cameraInfoLayout->addWidget(mpImageSizes);

	cameraInfoLayout->addWidget(mpFrameIntervalLabel);
	cameraInfoLayout->addWidget(mpFrameInterval_s);

	cameraInfoLayout->addWidget(mpFrameRateLabel);
	cameraInfoLayout->addWidget(mpFrameRate_fps);

	cameraInfoLayout->addStretch(1);

	cameraInfoLayout->addWidget(mpGrabImage);

	infoBox->setLayout(cameraInfoLayout);

	mainLayout->addWidget(infoBox);

	mainLayout->addWidget(mpThermalImage, 1);

	setLayout(mainLayout);
}


void cTeledyneFlirStatusView::onSensorNameChanging(QString old_name, QString new_name, QString instance)
{
	if (new_name.isEmpty())
		return;

	setWindowTitle(new_name);
}

void cTeledyneFlirStatusView::onModeChange(int mode)
{
	if ((mode < 0) || (mode >= mpMode->maxCount()))
	mpMode->setCurrentIndex(mode);
}

void cTeledyneFlirStatusView::onFrameIntervalChange(int interval_ms)
{
	mpFrameInterval_s->setText(QString::number(interval_ms * 0.001f));
}

void cTeledyneFlirStatusView::onFrameRateChange(double rate_fps)
{
	mpFrameRate_fps->setText(QString::number(rate_fps));
}

void cTeledyneFlirStatusView::onImageSizeChange(int width, int height)
{
	QString str = QString::number(width);
	str += " x ";
	str += QString::number(height);

	mpImageSizes->setText(str);
}

void cTeledyneFlirStatusView::imageUpdated(const QImage& image)
{
	mpThermalImage->setImage(image);

	if (!isHidden())
		mpThermalImage->repaint();
}

void cTeledyneFlirStatusView::resizeEvent(QResizeEvent* e)
{
	cSensorStatusView::resizeEvent(e);
	mpThermalImage->resizeImage(e->size().width(), e->size().height());
}
