
#include "AxisCommunicationsView_file.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QCamera>
#include <QLayout>

#include <string>


cAxisCommunicationsView_file::cAxisCommunicationsView_file(QWidget* parent)
    :
    cAxisCommunicationsView(nullptr, parent)
{
    setWindowTitle("Axis Communications F44");
}

cAxisCommunicationsView_file::~cAxisCommunicationsView_file()
{
}

void cAxisCommunicationsView_file::initialize()
{
    mpCameraIdLabel = new QLabel("Camera ID:");
    mpCameraId = new QLineEdit();
    mpCameraId->setEnabled(false);
    mpFrameRateLabel = new QLabel("Frame Rate:");
    mpFrameRate = new QLineEdit();
    mpFrameRate->setEnabled(false);
    mpImageSizeLabel = new QLabel("Image Size:");
    mpImageSize = new QLineEdit();
    mpImageSize->setEnabled(false);

    auto* mainLayout = new QVBoxLayout();

    auto* infoLayout = new QHBoxLayout();
    infoLayout->addWidget(mpCameraIdLabel);
    infoLayout->addWidget(mpCameraId);
    infoLayout->addWidget(mpFrameRateLabel);
    infoLayout->addWidget(mpFrameRate);
    infoLayout->addWidget(mpImageSizeLabel);
    infoLayout->addWidget(mpImageSize);

    mainLayout->addLayout(infoLayout);
    mainLayout->addWidget(mpImage);

    setLayout(mainLayout);
}

void cAxisCommunicationsView_file::activeCameraIdUpdated(int id)
{
    mpCameraId->setText(QString::number(id));
}

void cAxisCommunicationsView_file::framesRateUpdated(int frames_per_sec)
{
    mpFrameRate->setText(QString::number(frames_per_sec));
}

void cAxisCommunicationsView_file::imageSizeUpdated(int width, int height)
{
    QString image_size = QString::number(width);
    image_size += "x";
    image_size += QString::number(height);
    mpImageSize->setText(image_size);
}


