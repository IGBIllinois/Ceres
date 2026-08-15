
#include "AxisCommunicationsStatusView.hpp"
#include "AxisCommunicationsModel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QResizeEvent>

#include <string>


cAxisCommunicationsStatusView::cAxisCommunicationsStatusView(cAxisCommunicationsModel* pModel, QWidget* parent)
:
	cSensorStatusView(pModel, parent)
{
}

cAxisCommunicationsStatusView::~cAxisCommunicationsStatusView()
{}

void cAxisCommunicationsStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpCameraIdLabel = new QLabel("Camera ID:", this);
	mpCameraId = new QLineEdit(this);
	mpCameraId->setReadOnly(true);

	mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
	mpImageSizes = new QLineEdit(this);
	mpImageSizes->setReadOnly(true);

	mpModeLabel = new QLabel("Mode:", this);
	mpMode = new QLineEdit(this);
	mpMode->setText("Continuous");
	mpMode->setReadOnly(true);

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);
	mpFrameRate_fps->setReadOnly(true);

	mpLapseIntervalLabel = new QLabel("Lapse time (sec):", this);
	mpLapseInterval_s = new QLineEdit(this);
	mpLapseInterval_s->setReadOnly(true);


	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cAxisCommunicationsStatusView::requestImage);

/*
	mpSaveImage = new QPushButton("Save Image", this);
	connect(mpSaveImage, &QPushButton::pressed, this, &cAxisCommunicationsStatusView::saveImage);
*/

	mpImage = new cRgbImageWidget(this);
}

void cAxisCommunicationsStatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	QGroupBox* infoBox = new QGroupBox("Camera Information");

	auto* cameraInfoLayout = new QHBoxLayout();

	cameraInfoLayout->addWidget(mpCameraIdLabel);
	cameraInfoLayout->addWidget(mpCameraId);

	cameraInfoLayout->addWidget(mpImageSizeLabel);
	cameraInfoLayout->addWidget(mpImageSizes);

	cameraInfoLayout->addWidget(mpModeLabel);
	cameraInfoLayout->addWidget(mpMode);

	cameraInfoLayout->addWidget(mpFrameRateLabel);
	cameraInfoLayout->addWidget(mpFrameRate_fps);

	cameraInfoLayout->addWidget(mpLapseIntervalLabel);
	cameraInfoLayout->addWidget(mpLapseInterval_s);

	cameraInfoLayout->addStretch(1);

	cameraInfoLayout->addWidget(mpGrabImage);

/*
	cameraInfoLayout->addSpacing(20);

	cameraInfoLayout->addWidget(mpSaveImage);
*/

	infoBox->setLayout(cameraInfoLayout);

	mainLayout->addWidget(infoBox);

	mainLayout->addWidget(mpImage, 1);

	setLayout(mainLayout);
}

void cAxisCommunicationsStatusView::onCameraIdChange(int id)
{
	mpCameraId->setText(QString::number(id));
}

void cAxisCommunicationsStatusView::onImageSizeChange(int width, int height)
{
	QString str = QString::number(width);
	str += " x ";
	str += QString::number(height);

	mpImageSizes->setText(str);
}

void cAxisCommunicationsStatusView::onModeChange(int mode)
{
	if ((mode < 0) || (mode > 2))
		return;

	switch (mode)
	{
	case 0:
		mpMode->setText("Photo");
		break;
	case 1:
		mpMode->setText("Time Lapse");
		break;
	case 2:
		mpMode->setText("Continuous");
		break;
	}

/*
	mpMode->setCurrentIndex(mode);

	switch (mode)
	{
	case 0:
		mpFrameRate_fps->setEnabled(false);
		mpLapseInterval_s->setEnabled(false);
		break;
	case 1:
		mpFrameRate_fps->setEnabled(false);
		mpLapseInterval_s->setEnabled(true);
		break;
	case 2:
		mpFrameRate_fps->setEnabled(true);
		mpLapseInterval_s->setEnabled(false);
		break;
	}
*/
}

void cAxisCommunicationsStatusView::onFrameRateChange(int rate_fps)
{
	mpFrameRate_fps->setText(QString::number(rate_fps));
}

void cAxisCommunicationsStatusView::onLapseIntervalChange(int interval_ms)
{
	mpLapseInterval_s->setText(QString::number(interval_ms * 0.001, (char) 103, 3));
}

void cAxisCommunicationsStatusView::imageUpdated(const QImage& image)
{
	mpImage->setImage(image);

	if (!isHidden())
		mpImage->repaint();
}

void cAxisCommunicationsStatusView::resizeEvent(QResizeEvent* e)
{
	cSensorStatusView::resizeEvent(e);
//	mpImage->resizeImage(e->size().width(), e->size().height());
}
