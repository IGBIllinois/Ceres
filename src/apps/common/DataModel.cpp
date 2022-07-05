
#include "DataModel.hpp"

cDataModel::cDataModel(QObject* parent)
:
    QObject(parent)
{
}

cDataModel::~cDataModel()
{
}

void cDataModel::onStatusUpdate(QString msg)
{
    emit statusMessage(msg);
}

void cDataModel::onErrorUpdate(QString title, QString msg)
{
    emit errorMessage(title, msg);
}

/*
void cDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        pSensor->moveToThread(&mThread);
        mThread.mSensors.push_back(pSensor);
    }
}

void cDataModel::startDataThread()
{
    mThread.start();
}

void cDataModel::stopDataThread()
{
    mThread.stop();
}
*/


