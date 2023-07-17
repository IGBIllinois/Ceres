
#include "AxisCommunicationsStatusView.hpp"
#include "AxisCommunicationsModel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cAxisCommunicationsStatusView::cAxisCommunicationsStatusView(cAxisCommunicationsModel* pModel, QWidget* parent)
:
	cSensorStatusView(pModel, parent)
{
}

cAxisCommunicationsStatusView::~cAxisCommunicationsStatusView()
{
}

void cAxisCommunicationsStatusView::createWidgets()
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

	cameraInfoLayout->addWidget(mpFrameRateLabel);
	cameraInfoLayout->addWidget(mpFrameRate_fps);

	infoBox->setLayout(cameraInfoLayout);

	mainLayout->addWidget(infoBox);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cAxisCommunicationsStatusView::onCameraIdChange(int id)
{
	mpCameraId->setText(QString::number(id));
}

void cAxisCommunicationsStatusView::onFrameRateChange(int rate_fps)
{
	mpFrameRate_fps->setText(QString::number(rate_fps));
}

void cAxisCommunicationsStatusView::onImageSizeChange(int width, int height)
{
	QString str = QString::number(width);
	str += " x ";
	str += QString::number(height);

	mpImageSizes->setText(str);
}


