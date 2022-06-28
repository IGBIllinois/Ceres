
#include "DataModel.hpp"
#include "Sensors/SensorModel.hpp"

// Static Data
std::chrono::time_point<std::chrono::high_resolution_clock> cDataModel::mStartTime;

// Static Methods
std::uint64_t cDataModel::timestamp_ns()
{
    const auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStartTime).count();
}


cDataModel::cDataModel(QObject* parent)
:
    QObject(parent)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cDataModel::onStatusUpdate);
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


