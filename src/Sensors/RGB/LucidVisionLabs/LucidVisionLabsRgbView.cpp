
#include "LucidVisionLabsRgbView.hpp"
#include "Constants.hpp"
#include "LucidVisionLabsRgbModel.hpp"

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


cLucidVisionLabsRgbView::cLucidVisionLabsRgbView(cLucidVisionLabsRgbModel* pModel, QWidget* parent)
	:
    cRgbCameraView(parent),
    mpModel(pModel)
{
    setWindowTitle("RGB Camera");
}

cLucidVisionLabsRgbView::~cLucidVisionLabsRgbView()
{
}

void cLucidVisionLabsRgbView::initialize()
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
	case cLucidVisionLabsRgbModel::SINGLE:
		mpMode->setCurrentIndex(0);
		break;
	case cLucidVisionLabsRgbModel::TIME_LAPSE:
		mpMode->setCurrentIndex(1);
		break;
	case cLucidVisionLabsRgbModel::CONTINUOUS:
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

	statusLayout->addStretch(1);

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cLucidVisionLabsRgbView::onGrabImage);

	mpAutoUpdateImages = new QPushButton("Auto Update Image", this);
	mpAutoUpdateImages->setCheckable(true);
	connect(mpAutoUpdateImages, &QPushButton::toggled, this, &cLucidVisionLabsRgbView::onGrabImages);

	statusLayout->addWidget(mpGrabImage);
	statusLayout->addSpacing(10);
	statusLayout->addWidget(mpAutoUpdateImages);

	auto* mainLayout = new QVBoxLayout();

	mainLayout->addLayout(statusLayout);

	// Don't use any alignment on adding this widget.  For some reason the widget won't paint!
	mainLayout->addWidget(mpImage, 1);

	viewport->setLayout(mainLayout);

	setViewport(viewport);
}

void cLucidVisionLabsRgbView::connectToModel()
{
	connect(mpModel, &cLucidVisionLabsRgbModel::sensorNameChanging, this, &cLucidVisionLabsRgbView::onSensorNameChanging);
	connect(mpModel, &cLucidVisionLabsRgbModel::modeChanged,        this, &cLucidVisionLabsRgbView::onModeChange);
	connect(mpModel, &cLucidVisionLabsRgbModel::imageSizeChanged,   this, &cLucidVisionLabsRgbView::onImageSizeChange);
	connect(mpModel, &cLucidVisionLabsRgbModel::onNewImage,         this, &cLucidVisionLabsRgbView::imageUpdated);

	connect(this, &cLucidVisionLabsRgbView::requestImage,  mpModel, &cLucidVisionLabsRgbModel::requestImage);
	connect(this, &cLucidVisionLabsRgbView::requestImages, mpModel, &cLucidVisionLabsRgbModel::requestImages);
}

void cLucidVisionLabsRgbView::onSensorNameChanging(QString old_name, QString new_name, QString instance)
{
    if (new_name.isEmpty())
        return;

	QString title = "RGB Camera";
	title += " - ";
	title += new_name;

	if (!instance.isEmpty())
	{
		title += ":";
		title += instance;
	}
	
	setWindowTitle(title);

	auto* parent = parentWidget();
	if (parent)
		parent->setWindowTitle(title);
}

void cLucidVisionLabsRgbView::onModeChange(int mode)
{
	mpMode->setCurrentIndex(mode);
}

void cLucidVisionLabsRgbView::onImageSizeChange(int width, int height)
{
	QString image_size = QString::number(width);
	image_size += " x ";
	image_size += QString::number(height);

	mpImageSize->setText(image_size);
}

void cLucidVisionLabsRgbView::onGrabImage()
{
	mpGrabImage->setEnabled(false);

	if (0 == mpMode->currentIndex())
		emit requestPhoto();
	else
		emit requestImage();
}

void cLucidVisionLabsRgbView::onGrabImages(bool state)
{
	mpGrabImage->setEnabled(!state);
	emit requestImages(state);
}

void cLucidVisionLabsRgbView::imageUpdated(const QImage& image)
{
    mpImage->setImage(image);
    if (!isHidden())
        mpImage->repaint();
}

