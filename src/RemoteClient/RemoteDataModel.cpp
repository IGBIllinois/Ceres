
#include "RemoteDataModel.hpp"

#include "Sensors/SensorModel.hpp"

#include <QTcpServer>


cRemoteDataModel::cRemoteDataModel(QObject* parent)
    :
    QObject(parent),
    mpTcpServer(nullptr)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cRemoteDataModel::onStatusUpdate);

    mpTcpServer = new QTcpServer();
}

cRemoteDataModel::~cRemoteDataModel()
{
}

void cRemoteDataModel::onStatusUpdate(QString msg)
{
    emit statusMessage(msg);
}

void cRemoteDataModel::onErrorUpdate(QString title, QString msg)
{
    emit errorMessage(title, msg);
}

void cRemoteDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        pSensor->moveToThread(&mThread);
        mThread.mSensors.push_back(pSensor);
    }
}

void cRemoteDataModel::startDataThread()
{
    mThread.start();
}

void cRemoteDataModel::stopDataThread()
{
    mThread.stop();
}


