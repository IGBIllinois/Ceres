
#include "DataThread.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"


cDataThread::cDataThread()
:
    mpController(nullptr)
{
}

cDataThread::~cDataThread()
{
    stop();
}

void cDataThread::start()
{
    if (!isRunning()) 
    {
        mAbort = false;
        QThread::start(TimeCriticalPriority);
    }
}

void cDataThread::stop()
{
    mAbort = true;

    wait();
}

void cDataThread::run()
{
    // Start the network communications so that communication are tied to this thread
    if (!mpController->startCommunications())
    {
        goto cleanup;
    }
    for (auto& sensor : mActiveSensors)
    {
        if (!sensor->startCommunications())
        {
            goto cleanup;
        }
    }

    for (auto& sensor : mActiveSensors)
    {
        QObject::connect(mpController, &cExperimentControlModel::recordingStateChanged, sensor, &cSensorModel::recordingStateUpdated);
    }

    forever
    {
        if (mAbort)
            return;

        mpController->update();

        for (auto& sensor : mActiveSensors)
        {
            sensor->update();
        }
    }

    for (auto& sensor : mActiveSensors)
    {
        QObject::disconnect(mpController, &cExperimentControlModel::recordingStateChanged, sensor, &cSensorModel::recordingStateUpdated);
    }

cleanup:
    // Shutdown the network communications that are tied to this thread
    for (auto& sensor : mActiveSensors)
    {
        sensor->stopCommunications();
    }

    mpController->stopCommunications();

    QString msg("Data collection thread terminated.");

    emit statusMessage(msg);
}
