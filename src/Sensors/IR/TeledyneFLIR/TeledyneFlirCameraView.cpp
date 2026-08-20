
#include "TeledyneFlirCameraView.hpp"
#include "Constants.hpp"
#include "TeledyneFlirCameraModel.hpp"

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


cTeledyneFlirCameraView::cTeledyneFlirCameraView(cTeledyneFlirCameraModel* pModel, QWidget* parent)
	:
    cIrCameraView(parent),
    mpModel(pModel)
{
    setWindowTitle("Teledyne FLIR Camera");
}

cTeledyneFlirCameraView::~cTeledyneFlirCameraView()
{
}

void cTeledyneFlirCameraView::initialize()
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
	case cTeledyneFlirCameraModel::SINGLE:
		mpMode->setCurrentIndex(0);
		break;
	case cTeledyneFlirCameraModel::TIME_LAPSE:
		mpMode->setCurrentIndex(1);
		break;
	case cTeledyneFlirCameraModel::CONTINUOUS:
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

	mpThermalRangeLabel = new QLabel("Thermal Range (K):", this);
	mpThermalRange = new QLineEdit(this);
	mpThermalRange->setReadOnly(true);

	std::optional<float> minValue_K = mpModel->minThermalValue_K();
	std::optional<float> maxValue_K = mpModel->maxThermalValue_K();

	if (minValue_K.has_value() && maxValue_K.has_value())
	{
		QString thermal_range = QString::number(minValue_K.value());
		thermal_range += " to ";
		thermal_range += QString::number(maxValue_K.value());

		mpThermalRange->setText(thermal_range);
	}
	else
	{
		mpThermalRange->setText("Unknown");
	}

	statusLayout->addWidget(mpThermalRangeLabel);
	statusLayout->addWidget(mpThermalRange);
	statusLayout->addStretch(1);

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cTeledyneFlirCameraView::onGrabImage);

	mpAutoUpdateImages = new QPushButton("Auto Update Image", this);
	mpAutoUpdateImages->setCheckable(true);
	connect(mpAutoUpdateImages, &QPushButton::toggled, this, &cTeledyneFlirCameraView::onGrabImages);

	statusLayout->addWidget(mpGrabImage);
	statusLayout->addSpacing(10);
	statusLayout->addWidget(mpAutoUpdateImages);

	auto* mainLayout = new QVBoxLayout();

	mainLayout->addLayout(statusLayout);

	// Don't use any alignment on adding this widget.  For some reason the widget won't paint!
	mainLayout->addWidget(mpThermalImage, 1);

	viewport->setLayout(mainLayout);

	setViewport(viewport);
}

void cTeledyneFlirCameraView::connectToModel()
{
	QObject::connect(mpModel, &cTeledyneFlirCameraModel::sensorNameChanging, this, &cTeledyneFlirCameraView::onSensorNameChanging);
	QObject::connect(mpModel, &cTeledyneFlirCameraModel::modeChanged,        this, &cTeledyneFlirCameraView::onModeChange);
	QObject::connect(mpModel, &cTeledyneFlirCameraModel::imageSizeChanged,   this, &cTeledyneFlirCameraView::onImageSizeChange);
	QObject::connect(mpModel, &cTeledyneFlirCameraModel::onNewImage,         this, &cTeledyneFlirCameraView::imageUpdated);

	QObject::connect(this, &cTeledyneFlirCameraView::requestImage,  mpModel, &cTeledyneFlirCameraModel::requestImage);
	QObject::connect(this, &cTeledyneFlirCameraView::requestImages, mpModel, &cTeledyneFlirCameraModel::requestImages);
}

void cTeledyneFlirCameraView::onSensorNameChanging(QString old_name, QString new_name, QString instance)
{
	if (new_name.isEmpty())
		return;

	QString title = "Teledyne FLIR Camera";
	title += " - ";
	title += new_name;
	setWindowTitle(title);

	auto* parent = parentWidget();
	if (parent)
		parent->setWindowTitle(title);
}

void cTeledyneFlirCameraView::onModeChange(int mode)
{
	mpMode->setCurrentIndex(mode);
}

void cTeledyneFlirCameraView::onImageSizeChange(int width, int height)
{
	QString image_size = QString::number(width);
	image_size += " x ";
	image_size += QString::number(height);

	mpImageSize->setText(image_size);
}

void cTeledyneFlirCameraView::onGrabImage()
{
	mpGrabImage->setEnabled(false);
	emit requestImage();
}

void cTeledyneFlirCameraView::onGrabImages(bool state)
{
	mpGrabImage->setEnabled(!state);
	emit requestImages(state);
}


void cTeledyneFlirCameraView::imageUpdated(const QImage& image)
{
	if (!mpAutoUpdateImages->isChecked())
		mpGrabImage->setEnabled(true);

	QString image_size = QString::number(image.width());
	image_size += " x ";
	image_size += QString::number(image.height());

	mpImageSize->setText(image_size);

    mpThermalImage->setImage(image);
    if (!isHidden())
        mpThermalImage->repaint();
}


