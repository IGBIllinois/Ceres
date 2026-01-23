
#include "LucidVisionLabsRgbStatusView.hpp"
#include "LucidVisionLabsRgbModel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QResizeEvent>

#include <string>


cLucidVisionLabsRgbStatusView::cLucidVisionLabsRgbStatusView(cLucidVisionLabsRgbModel* pModel, QWidget* parent)
:
	cSensorStatusView(pModel, parent)
{
}

cLucidVisionLabsRgbStatusView::~cLucidVisionLabsRgbStatusView()
{}

void cLucidVisionLabsRgbStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpCameraIdLabel = new QLabel("Camera ID:", this);
	mpCameraId = new QLineEdit(this);
	mpCameraId->setReadOnly(true);

	mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
	mpImageSizes = new QLineEdit(this);
	mpImageSizes->setReadOnly(true);

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);
	mpFrameRate_fps->setReadOnly(true);

	mpGrabImage = new QPushButton("Grab Image", this);
	connect(mpGrabImage, &QPushButton::pressed, this, &cLucidVisionLabsRgbStatusView::requestImage);

	mpImage = new cRgbImageWidget(this);
}

void cLucidVisionLabsRgbStatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	QGroupBox* infoBox = new QGroupBox("Camera Information");

	auto* cameraInfoLayout = new QHBoxLayout();

	cameraInfoLayout->addWidget(mpCameraIdLabel);
	cameraInfoLayout->addWidget(mpCameraId);

	cameraInfoLayout->addWidget(mpImageSizeLabel);
	cameraInfoLayout->addWidget(mpImageSizes);

	cameraInfoLayout->addWidget(mpFrameRateLabel);
	cameraInfoLayout->addWidget(mpFrameRate_fps);

	cameraInfoLayout->addStretch(1);

	cameraInfoLayout->addWidget(mpGrabImage);

	infoBox->setLayout(cameraInfoLayout);

	mainLayout->addWidget(infoBox);

	mainLayout->addWidget(mpImage, 1);
//	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cLucidVisionLabsRgbStatusView::onCameraIdChange(int id)
{
	mpCameraId->setText(QString::number(id));
}

void cLucidVisionLabsRgbStatusView::onFrameRateChange(int rate_fps)
{
	mpFrameRate_fps->setText(QString::number(rate_fps));
}

void cLucidVisionLabsRgbStatusView::onImageSizeChange(int width, int height)
{
	QString str = QString::number(width);
	str += " x ";
	str += QString::number(height);

	mpImageSizes->setText(str);
}

void cLucidVisionLabsRgbStatusView::imageUpdated(const QImage& image)
{
	mpImage->setImage(image);

	if (!isHidden())
		mpImage->repaint();
}

void cLucidVisionLabsRgbStatusView::resizeEvent(QResizeEvent* e)
{
	cSensorStatusView::resizeEvent(e);
	mpImage->resizeImage(e->size().width(), e->size().height());
}
