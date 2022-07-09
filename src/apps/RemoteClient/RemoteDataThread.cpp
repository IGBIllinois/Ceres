
#include "RemoteDataThread.hpp"
#include "RemoteDataModel.hpp"
#include "SensorModel.hpp"


cRemoteDataThread::cRemoteDataThread(cRemoteDataModel* pController)
    :
    mpController(pController)
{
}

cRemoteDataThread::~cRemoteDataThread()
{
}


bool cRemoteDataThread::startCommunications()
{
    QString msg("Starting communications...");
    emit statusMessage(msg);

    if (!cDataThread::startCommunications())
    {
        return false;
    }

    for (auto& sensor : mActiveSensors)
    {
        QObject::connect(mpController, &cRemoteDataModel::requestDataRecordingState, sensor, &cSensorModel::dataRecordingStateChange);
    }

    return true;
}

bool cRemoteDataThread::stopCommunications()
{
    QString msg("Stopping communications...");
    emit statusMessage(msg);

    for (auto& sensor : mActiveSensors)
    {
        QObject::disconnect(mpController, &cRemoteDataModel::requestDataRecordingState, sensor, &cSensorModel::dataRecordingStateChange);
    }

    cDataThread::stopCommunications();

    return true;
}


