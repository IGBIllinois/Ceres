
#include "AxisCommunicationsView_F44.hpp"
#include "../../Utilities/Constants.hpp"
#include "AxisCommunicationsModel_F44.hpp"

#include <QToolBar>
#include <QToolButton>
#include <QCamera>

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

    mpCamera1 = new QToolButton(toolbar);
    mpCamera1->setText("1");
    mpCamera1->setEnabled(false);
    connect(mpCamera1, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_1);

    toolbar->addWidget(mpCamera1);

    mpCamera2 = new QToolButton(toolbar);
    mpCamera2->setText("2");
    mpCamera2->setEnabled(false);
    connect(mpCamera2, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_2);

    toolbar->addWidget(mpCamera2);

    mpCamera3 = new QToolButton(toolbar);
    mpCamera3->setText("3");
    mpCamera3->setEnabled(false);
    connect(mpCamera3, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_3);

    toolbar->addWidget(mpCamera3);

    mpCamera4 = new QToolButton(toolbar);
    mpCamera4->setText("4");
    mpCamera4->setEnabled(false);
    connect(mpCamera4, &QToolButton::released, this, &cAxisCommunicationsView_F44::cameraSelected_4);

    toolbar->addWidget(mpCamera4);
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


