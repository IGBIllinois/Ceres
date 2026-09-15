
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
{}

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
	connect(mpMode, &QComboBox::currentTextChanged, this, &cTeledyneFlirStatusView::modeTextChanged);

	mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
	mpImageSizes = new QLineEdit(this);
	mpImageSizes->setReadOnly(true);

	mpLapseIntervalLabel = new QLabel("Time-Lapse Interval (sec):", this);
	mpLapseInterval_s = new QLineEdit(this);
	mpLapseInterval_s->setReadOnly(true);
	connect(mpLapseInterval_s, &QLineEdit::editingFinished, this, &cTeledyneFlirStatusView::lapseIntervalEditingFinished);

	mpFrameRateLabel = new QLabel("Frame Rate (f/s):", this);
	mpFrameRate_fps = new QLineEdit(this);
	mpFrameRate_fps->setReadOnly(true);
	connect(mpFrameRate_fps, &QLineEdit::editingFinished, this, &cTeledyneFlirStatusView::frameRateEditingFinished);

	mpThermalRangeLabel = new QLabel("Thermal Range (K):", this);
	mpThermalRange = new QLineEdit(this);
	mpThermalRange->setReadOnly(true);

	mpColorPalette = new QComboBox(this);
	mpColorPalette->addItem("Ironbow");
	mpColorPalette->addItem("Rainbow");
	mpColorPalette->addItem("Rainbow HC");
	mpColorPalette->addItem("White Hot");
	mpColorPalette->addItem("Black Hot");
	mpColorPalette->addItem("Arctic");
	connect(mpColorPalette, &QComboBox::currentTextChanged, this, &cTeledyneFlirStatusView::colorPaletteTextChanged);

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cTeledyneFlirStatusView::grabImageRequested);

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

	cameraInfoLayout->addWidget(mpLapseIntervalLabel);
	cameraInfoLayout->addWidget(mpLapseInterval_s);

	cameraInfoLayout->addWidget(mpFrameRateLabel);
	cameraInfoLayout->addWidget(mpFrameRate_fps);

	cameraInfoLayout->addSpacing(10);

	cameraInfoLayout->addWidget(mpThermalRangeLabel);
	cameraInfoLayout->addWidget(mpThermalRange);

	cameraInfoLayout->addSpacing(10);

	cameraInfoLayout->addWidget(mpColorPalette);

	cameraInfoLayout->addStretch(1);

	cameraInfoLayout->addWidget(mpGrabImage);

	infoBox->setLayout(cameraInfoLayout);

	mainLayout->addWidget(infoBox);

	mainLayout->addWidget(mpThermalImage, 1);

	setLayout(mainLayout);
}

void cTeledyneFlirStatusView::onModeChange(int mode)
{
	if ((mode < 0) || (mode >= mpMode->maxCount()))
		return;
	mpMode->setCurrentIndex(mode);
}

void cTeledyneFlirStatusView::onLapseIntervalChange(int interval_ms)
{
	mpLapseInterval_s->setText(QString::number(interval_ms * 0.001f));
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

void cTeledyneFlirStatusView::thermalRangeUpdated(float minValue_K, float maxValue_K)
{
	QString thermal_range = QString::number(minValue_K);
	thermal_range += " to ";
	thermal_range += QString::number(maxValue_K);

	mpThermalRange->setText(thermal_range);
}

void cTeledyneFlirStatusView::colorPaletteUpdated(eColorTable palette)
{
	switch (palette)
	{
	case eColorTable::IRONBOW:
		mpColorPalette->setCurrentIndex(0);
		break;
	case eColorTable::RAINBOW:
		mpColorPalette->setCurrentIndex(1);
		break;
	case eColorTable::RAINBOW_HC:
		mpColorPalette->setCurrentIndex(2);
		break;
	case eColorTable::WHITE_HOT:
		mpColorPalette->setCurrentIndex(3);
		break;
	case eColorTable::BLACK_HOT:
		mpColorPalette->setCurrentIndex(4);
		break;
	case eColorTable::ARCTIC:
		mpColorPalette->setCurrentIndex(5);
		break;
	default:
		return;
	}
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

void cTeledyneFlirStatusView::modeTextChanged(const QString& text)
{
	if (text == "Photo")
		emit requestMode(0);
	else if (text == "Time Lapse")
		emit requestMode(1);
	else if (text == "Continuous")
		emit requestMode(2);
}

void cTeledyneFlirStatusView::colorPaletteTextChanged(const QString& text)
{
	if (text == "Ironbow")
		emit requestColorPalette(eColorTable::IRONBOW);
	else if (text == "Rainbow")
		emit requestColorPalette(eColorTable::RAINBOW);
	else if (text == "Rainbow HC")
		emit requestColorPalette(eColorTable::RAINBOW_HC);
	else if (text == "White Hot")
		emit requestColorPalette(eColorTable::WHITE_HOT);
	else if (text == "Black Hot")
		emit requestColorPalette(eColorTable::BLACK_HOT);
	else if (text == "Arctic")
		emit requestColorPalette(eColorTable::ARCTIC);
}

void cTeledyneFlirStatusView::frameRateEditingFinished()
{
	double frame_rate_hz = mpFrameRate_fps->text().toDouble();
	emit requestFrameRate_Hz(frame_rate_hz);
}

void cTeledyneFlirStatusView::lapseIntervalEditingFinished()
{
	uint32_t interval_ms = static_cast<uint32_t>(mpLapseInterval_s->text().toDouble() * 1000.0);

	emit requestLapseInterval_ms(interval_ms);
}

void cTeledyneFlirStatusView::grabImageRequested()
{
	emit requestImages(true);
}
