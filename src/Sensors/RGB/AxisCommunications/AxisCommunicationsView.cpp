
#include "AxisCommunicationsView.hpp"
#include "Constants.hpp"
#include "AxisCommunicationsModel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QComboBox>
#include <QPaintEvent>
#include <QPainter>
#include <QToolBar>

#include <string>


cAxisCommunicationsView::cAxisCommunicationsView(cAxisCommunicationsModel* pModel, QWidget* parent)
	:
    cRgbCameraView(parent),
    mpModel(pModel)
{
    setWindowTitle("RGB Camera");
}

cAxisCommunicationsView::~cAxisCommunicationsView()
{
}

void cAxisCommunicationsView::initialize()
{
	auto* viewport = new QWidget(this);

	auto* statusLayout = new QHBoxLayout();

	mpModeLabel = new QLabel("Mode:", this);
	mpMode = new QComboBox(this);
	mpMode->addItem("Photo");
	mpMode->addItem("Time Lapse");
	mpMode->addItem("Continuous");
	mpMode->setEnabled(false);

	switch (mpModel->mode())
	{
	case cRgbCameraModel::SINGLE:
		mpMode->setCurrentIndex(0);
		break;
	case cRgbCameraModel::TIME_LAPSE:
		mpMode->setCurrentIndex(1);
		break;
	case cRgbCameraModel::CONTINUOUS:
		mpMode->setCurrentIndex(2);
		break;
	}

	statusLayout->addWidget(mpModeLabel);
	statusLayout->addWidget(mpMode);
	statusLayout->addSpacing(10);

	mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
	mpImageSize = new QLineEdit(this);
	mpImageSize->setReadOnly(true);

	statusLayout->addWidget(mpImageSizeLabel);
	statusLayout->addWidget(mpImageSize);
	statusLayout->addSpacing(10);

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);
	mpFrameRate_fps->setReadOnly(true);

	statusLayout->addWidget(mpFrameRateLabel);
	statusLayout->addWidget(mpFrameRate_fps);
	statusLayout->addSpacing(10);

	mpLapseIntervalLabel = new QLabel("Lapse time (sec):", this);
	mpLapseInterval_s = new QLineEdit(this);
	mpLapseInterval_s->setReadOnly(true);

	statusLayout->addWidget(mpLapseIntervalLabel);
	statusLayout->addWidget(mpLapseInterval_s);

	statusLayout->addStretch(1);

	auto* mainLayout = new QVBoxLayout();

	mainLayout->addLayout(statusLayout);

	viewport->setLayout(mainLayout);

	setViewport(viewport);
}

void cAxisCommunicationsView::onImageSizeChange(int width, int height)
{
	QString str = QString::number(width);
	str += " x ";
	str += QString::number(height);

	if (mpImageSize)
		mpImageSize->setText(str);
}

void cAxisCommunicationsView::onModeChange(int mode)
{
	if (!mpMode)
		return;

	if ((mode < 0) || (mode > 2))
		return;

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
}

void cAxisCommunicationsView::onFrameRateChange(int rate_fps)
{
	if (mpFrameRate_fps)
		mpFrameRate_fps->setText(QString::number(rate_fps));
}

void cAxisCommunicationsView::onLapseIntervalChange(int interval_ms)
{
	if (mpLapseInterval_s)
		mpLapseInterval_s->setText(QString::number(interval_ms * 0.001, (char)103, 3));
}


void cAxisCommunicationsView::imageUpdated(const QImage& image)
{
    mpImage->setImage(image);
    if (!isHidden())
        mpImage->repaint();
}

