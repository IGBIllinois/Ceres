
#include "AxisCommunicationsView_F44.hpp"
#include "Constants.hpp"
#include "AxisCommunicationsModel_F44.hpp"

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

    mpSaveImage = new QPushButton("Save", toolbar);
    mpSaveImage->setEnabled(false);
    connect(mpSaveImage, &QPushButton::pressed, this, &cAxisCommunicationsView_F44::saveImage);

    auto* mainLayout = new QVBoxLayout();

    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(mpImage);

    setLayout(mainLayout);
}

void cAxisCommunicationsView_F44::enableCamera(int id)
{
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

