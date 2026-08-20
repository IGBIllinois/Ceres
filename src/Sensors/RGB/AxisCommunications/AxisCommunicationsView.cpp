
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
#include <QDoubleValidator>

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
	createWidgets();

	auto* viewport = new QWidget(this);

	auto* controlLayout = new QHBoxLayout();

	doControlLayout(controlLayout);

	auto* mainLayout = new QVBoxLayout();

	mainLayout->addLayout(controlLayout);

	mainLayout->addWidget(mpImage);

	viewport->setLayout(mainLayout);

	setViewport(viewport);
}

void cAxisCommunicationsView::connectToModel()
{
	QObject::connect(mpModel, &cAxisCommunicationsModel::modeChanged,          this, &cAxisCommunicationsView::onModeChange);
	QObject::connect(mpModel, &cAxisCommunicationsModel::lapseIntervalChanged, this, &cAxisCommunicationsView::onLapseIntervalChange);
	QObject::connect(mpModel, &cAxisCommunicationsModel::frameRateChanged,     this, &cAxisCommunicationsView::onFrameRateChange);
	QObject::connect(mpModel, &cAxisCommunicationsModel::imageSizeChanged,     this, &cAxisCommunicationsView::onImageSizeChange);
	QObject::connect(mpModel, &cAxisCommunicationsModel::onNewImage,           this, &cAxisCommunicationsView::imageUpdated);

	QObject::connect(this, &cAxisCommunicationsView::requestMode,             mpModel, &cAxisCommunicationsModel::requestMode);
	QObject::connect(this, &cAxisCommunicationsView::requestFrameRate_Hz,     mpModel, &cAxisCommunicationsModel::requestFrameRate_Hz);
	QObject::connect(this, &cAxisCommunicationsView::requestLapseInterval_ms, mpModel, &cAxisCommunicationsModel::requestLapseInterval_ms);
	QObject::connect(this, &cAxisCommunicationsView::requestImageSize,        mpModel, &cAxisCommunicationsModel::requestImageSize);
}


void cAxisCommunicationsView::createWidgets()
{
	mpModeLabel = new QLabel("Mode:", this);
	mpMode = new QComboBox(this);
	mpMode->addItem("Photo");
	mpMode->addItem("Time Lapse");
	mpMode->addItem("Continuous");

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
	connect(mpMode, &QComboBox::currentTextChanged, this, &cAxisCommunicationsView::modeTextChanged);

	mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
	mpImageSizes = new QComboBox(this);
	mpImageSizes->addItem("1920x1080");
	mpImageSizes->addItem("1280x720");
	mpImageSizes->addItem("1024x768");
	mpImageSizes->addItem("1024x640");
	mpImageSizes->addItem("800x600");
	mpImageSizes->addItem("640x480");
	connect(mpImageSizes, &QComboBox::currentTextChanged, this, &cAxisCommunicationsView::imageSizesTextChanged);

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);

	auto minRate_fps = mpModel->minFrameRate_fps();
	auto maxRate_fps = mpModel->maxFrameRate_fps();

	if (minRate_fps.has_value() && maxRate_fps.has_value())
	{
		mpFrameRate_fps->setValidator(new QDoubleValidator(minRate_fps.value(), maxRate_fps.value(), 0));
	}
	else
	{
		mpFrameRate_fps->setValidator(new QDoubleValidator(1.0, 50.0, 0));
	}
	connect(mpFrameRate_fps, &QLineEdit::editingFinished, this, &cAxisCommunicationsView::frameRateChanged);

	mpLapseIntervalLabel = new QLabel("Lapse time (sec):", this);
	mpLapseInterval_s = new QLineEdit(this);
	mpLapseInterval_s->setValidator(new QDoubleValidator(0.1, 300.0, 3));
	connect(mpLapseInterval_s, &QLineEdit::editingFinished, this, &cAxisCommunicationsView::lapseIntervalChanged);
}

void cAxisCommunicationsView::doControlLayout(QHBoxLayout* pControlLayout)
{
	pControlLayout->addWidget(mpModeLabel);
	pControlLayout->addWidget(mpMode);
	pControlLayout->addSpacing(10);

	pControlLayout->addWidget(mpImageSizeLabel);
	pControlLayout->addWidget(mpImageSizes);
	pControlLayout->addSpacing(10);

	pControlLayout->addWidget(mpFrameRateLabel);
	pControlLayout->addWidget(mpFrameRate_fps);
	pControlLayout->addSpacing(10);

	pControlLayout->addWidget(mpLapseIntervalLabel);
	pControlLayout->addWidget(mpLapseInterval_s);

	pControlLayout->addStretch(1);
}


void cAxisCommunicationsView::modeTextChanged(const QString& text)
{
	if (text == "Photo")
	{
		emit requestMode(cRgbCameraModel::SINGLE);
	}
	else if (text == "Time Lapse")
	{
		emit requestMode(cRgbCameraModel::TIME_LAPSE);
	}
	else if (text == "Continuous")
	{
		emit requestMode(cRgbCameraModel::CONTINUOUS);
	}
}

void cAxisCommunicationsView::imageSizesTextChanged(const QString& text)
{
	rgb::sImageSize_t image_size = axis::to_image_size(text.toStdString());

	emit requestImageSize(image_size.width, image_size.height);
}

void cAxisCommunicationsView::frameRateChanged()
{
	double frame_rate_hz = mpFrameRate_fps->text().toDouble();

	emit requestFrameRate_Hz(frame_rate_hz);
}

void cAxisCommunicationsView::lapseIntervalChanged()
{
	uint32_t interval_ms = static_cast<uint32_t>(mpLapseInterval_s->text().toDouble() * 1000.0);

	emit requestLapseInterval_ms(interval_ms);
}

void cAxisCommunicationsView::onImageSizeChange(int width, int height)
{
	if (!mpImageSizes) return;

	if ((width == 1920) && (height == 1080))
		mpImageSizes->setCurrentIndex(0);
	else if ((width == 1280) && (height == 720))
		mpImageSizes->setCurrentIndex(1);
	else if ((width == 1024) && (height == 768))
		mpImageSizes->setCurrentIndex(2);
	else if ((width == 1024) && (height == 640))
		mpImageSizes->setCurrentIndex(3);
	else if ((width == 800) && (height == 600))
		mpImageSizes->setCurrentIndex(4);
	else if ((width == 640) && (height == 480))
		mpImageSizes->setCurrentIndex(5);
}


void cAxisCommunicationsView::onModeChange(int mode)
{
	if (!mpMode)
		return;

	if ((mode < 0) || (mode > 2))
		return;

	mpMode->setCurrentIndex(mode);

/*
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

