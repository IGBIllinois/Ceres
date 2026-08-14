
#include "AxisCommunicationsView_F44.hpp"
#include "Constants.hpp"
#include "AxisCommunicationsModel_F44.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QResizeEvent>
#include <QComboBox>
#include <QToolBar>
#include <QToolButton>
#include <QPushButton>
#include <QCamera>
#include <QLayout>
#include <QImage>

#include <string>


cAxisCommunicationsView_F44::cAxisCommunicationsView_F44(cAxisCommunicationsModel_F44* pModel, QWidget* parent)
    :
    cAxisCommunicationsView(pModel),
    mpCamera1(nullptr), mpCamera2(nullptr), mpCamera3(nullptr), mpCamera4(nullptr),
    mpModel(pModel)
{
    setWindowTitle("Axis Communications F44");
}

cAxisCommunicationsView_F44::~cAxisCommunicationsView_F44()
{
}

void cAxisCommunicationsView_F44::initialize()
{
    mpCameraIdLabel = new QLabel("Camera ID:", this);
    mpCameraId = new QLineEdit(this);
    mpCameraId->setReadOnly(true);

    mpImageSizeLabel = new QLabel("Image Size (w x h):", this);
    mpImageSize = new QLineEdit(this);
    mpImageSize->setReadOnly(true);

    mpModeLabel = new QLabel("Mode:", this);
    mpMode = new QComboBox(this);
    mpMode->addItem("Photo");
    mpMode->addItem("Time Lapse");
    mpMode->addItem("Continuous");
    mpMode->setCurrentIndex(2);
    mpMode->setEnabled(false);

    mpFrameRateLabel = new QLabel("Frames per Second:", this);
    mpFrameRate_fps = new QLineEdit(this);
    mpFrameRate_fps->setReadOnly(true);

    mpLapseIntervalLabel = new QLabel("Lapse time (sec):", this);
    mpLapseInterval_s = new QLineEdit(this);
    mpLapseInterval_s->setReadOnly(true);

    mpShowCrossHairs = new QPushButton("Show Crosshairs", this);
    mpShowCrossHairs->setCheckable(true);
    mpShowCrossHairs->setHidden(true);
    QObject::connect(mpShowCrossHairs, &QPushButton::clicked, this, &cAxisCommunicationsView_F44::enableCrosshairs);

    auto* viewport = new QWidget(this);

    auto* statusLayout = new QHBoxLayout();

    statusLayout->addWidget(mpCameraIdLabel);
    statusLayout->addWidget(mpCameraId);
    statusLayout->addSpacing(10);

    statusLayout->addWidget(mpModeLabel);
    statusLayout->addWidget(mpMode);
    statusLayout->addSpacing(10);

    statusLayout->addWidget(mpImageSizeLabel);
    statusLayout->addWidget(mpImageSize);
    statusLayout->addSpacing(10);

    statusLayout->addWidget(mpFrameRateLabel);
    statusLayout->addWidget(mpFrameRate_fps);
    statusLayout->addSpacing(10);

    statusLayout->addWidget(mpLapseIntervalLabel);
    statusLayout->addWidget(mpLapseInterval_s);

    statusLayout->addStretch(1);

    statusLayout->addWidget(mpShowCrossHairs);

    auto* mainLayout = new QVBoxLayout();

    mainLayout->addLayout(statusLayout);

    mainLayout->addWidget(mpImage, 1);

    viewport->setLayout(mainLayout);

    setViewport(viewport);


/*
    auto* toolbar = new QToolBar(this);
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setAutoFillBackground(true);

    mpCamera1 = new QToolButton(toolbar);
    mpCamera1->setIcon(QIcon(":/ripe.illinois.edu/one_button.png"));
    mpCamera1->setEnabled(false);
    connect(mpCamera1, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_1);

    toolbar->addWidget(mpCamera1);

    mpCamera2 = new QToolButton(toolbar);
    mpCamera2->setIcon(QIcon(":/ripe.illinois.edu/two_button.png"));
    mpCamera2->setEnabled(false);
    connect(mpCamera2, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_2);

    toolbar->addWidget(mpCamera2);

    mpCamera3 = new QToolButton(toolbar);
    mpCamera3->setIcon(QIcon(":/ripe.illinois.edu/three_button.png"));
    mpCamera3->setEnabled(false);
    connect(mpCamera3, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_3);

    toolbar->addWidget(mpCamera3);

    mpCamera4 = new QToolButton(toolbar);
    mpCamera4->setIcon(QIcon(":/ripe.illinois.edu/four_button.png"));
    mpCamera4->setEnabled(false);
    connect(mpCamera4, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_4);

    toolbar->addWidget(mpCamera4);

    toolbar->addSeparator();

    mpShowCrossHairs = new QPushButton("Show Crosshairs", toolbar);
    mpShowCrossHairs->setCheckable(true);
    QObject::connect(mpShowCrossHairs, &QPushButton::clicked, this, &cAxisCommunicationsView_F44::enableCrosshairs);

    toolbar->addWidget(mpShowCrossHairs);

    toolbar->addSeparator();

    mpSaveImage = new QPushButton("Save", toolbar);
    mpSaveImage->setEnabled(false);
    connect(mpSaveImage, &QPushButton::pressed, this, &cAxisCommunicationsView_F44::saveImage);

    toolbar->addWidget(mpSaveImage);

    auto* mainLayout = new QVBoxLayout();

    mainLayout->addWidget(toolbar, 1);
    mainLayout->addWidget(mpImage);

    setLayout(mainLayout);
*/
}

void cAxisCommunicationsView_F44::showCrossHairButton(bool show)
{
    mpShowCrossHairs->setHidden(!show);
}

void cAxisCommunicationsView_F44::onCameraIdChange(int id)
{
    if (mpCameraId)
        mpCameraId->setText(QString::number(id));
}

void cAxisCommunicationsView_F44::enableCamera(int id)
{
    if (mpCameraId)
        mpCameraId->setText(QString::number(id));

/*
    switch (id)
    {
    case 1:
        mpCamera1->setEnabled(true);
        break;

    case 2:
        mpCamera2->setEnabled(true);
        break;

    case 3:
        mpCamera3->setEnabled(true);
        break;

    case 4:
        mpCamera4->setEnabled(true);
        break;
    }
*/
}

void cAxisCommunicationsView_F44::cameraSelected_1()
{
    emit activateCamera(1);
}

void cAxisCommunicationsView_F44::cameraSelected_2()
{
    emit activateCamera(2);
}

void cAxisCommunicationsView_F44::cameraSelected_3()
{
    emit activateCamera(3);
}

void cAxisCommunicationsView_F44::cameraSelected_4()
{
    emit activateCamera(4);
}

void cAxisCommunicationsView_F44::imageUpdated(const QImage& image)
{
    cAxisCommunicationsView::imageUpdated(image);
}

