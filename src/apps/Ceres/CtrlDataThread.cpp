
#include "CtrlDataThread.hpp"
#include "ExperimentCtrlModel.hpp"
#include "SensorModel.hpp"

cCtrlDataThread::cCtrlDataThread()
:
    mpController(nullptr)
{
}

cCtrlDataThread::~cCtrlDataThread()
{
    stop();
}

bool cCtrlDataThread::startCommunications()
{
    cDataThread::startCommunications();

    if (!mpController->startCommunications())
    {
        return false;
    }

    for (auto& sensor : mActiveSensors)
    {
        QObject::connect(mpController, &cExperimentControlModel::requestDataRecordingState, sensor, &cSensorModel::dataRecordingStateChange);
    }

    return true;
}

bool cCtrlDataThread::stopCommunications()
{
    for (auto& sensor : mActiveSensors)
    {
        QObject::disconnect(mpController, &cExperimentControlModel::requestDataRecordingState, sensor, &cSensorModel::dataRecordingStateChange);
    }

    // Shutdown the network communications that are tied to this thread
    cDataThread::stopCommunications();

    mpController->stopCommunications();

    return true;
}

void cCtrlDataThread::updateAll()
{
    mpController->update();

    cDataThread::updateAll();
}


