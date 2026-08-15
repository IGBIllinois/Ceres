
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
//    mpCamera1(nullptr), mpCamera2(nullptr), mpCamera3(nullptr), mpCamera4(nullptr),
    mpModel(pModel)
{
    setWindowTitle("Axis Communications F44");
}

cAxisCommunicationsView_F44::~cAxisCommunicationsView_F44()
{
}

void cAxisCommunicationsView_F44::createWidgets()
{
    cAxisCommunicationsView::createWidgets();

    mpCameraIdLabel = new QLabel("Camera ID:", this);
    mpCameraId = new QComboBox(this);

    mMinCameraId = mpModel->getMinCameraID();
    mMaxCameraId = mpModel->getMaxCameraID();

    for (auto i = mMinCameraId; i <= mMaxCameraId; i++)
    {
        mpCameraId->addItem(QString::number(i));
    }

    mpCameraId->setCurrentIndex(mpModel->getActiveCameraID() - mMinCameraId);
    connect(mpCameraId, &QComboBox::currentIndexChanged, this, &cAxisCommunicationsView_F44::cameraIdIndexChanged);

    mpShowCrossHairs = new QPushButton("Show Crosshairs", this);
    mpShowCrossHairs->setCheckable(true);
    QObject::connect(mpShowCrossHairs, &QPushButton::clicked, this, &cAxisCommunicationsView_F44::enableCrosshairs);
}

void cAxisCommunicationsView_F44::doControlLayout(QHBoxLayout* pControlLayout)
{
    pControlLayout->addWidget(mpCameraIdLabel);
    pControlLayout->addWidget(mpCameraId);
    pControlLayout->addSpacing(10);

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

    pControlLayout->addWidget(mpShowCrossHairs);
}

void cAxisCommunicationsView_F44::cameraIdIndexChanged(int id)
{
    emit requestCameraID(id + mMinCameraId);
}

void cAxisCommunicationsView_F44::onCameraIdChange(int id)
{
    if (mpCameraId)
    {
        mpCameraId->setCurrentIndex(id - mMinCameraId);
    }
}

void cAxisCommunicationsView_F44::onCameraRangeChange(int min_id, int max_id)
{
    mMinCameraId = min_id;
    mMaxCameraId = max_id;

    mpCameraId->clear();

    for (auto i = mMinCameraId; i <= mMaxCameraId; i++)
    {
        mpCameraId->addItem(QString::number(i));
    }

    mpCameraId->setCurrentIndex(mpModel->getActiveCameraID() - mMinCameraId);
}


