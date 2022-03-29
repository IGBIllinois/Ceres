
#include "DataThread.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"

#include <QAbstractEventDispatcher>

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
    mActiveSensors.clear();

    // Start the network communications so that communication are tied to this thread
    for (auto& sensor : mSensors)
    {
        if (!sensor->startCommunications())
        {
            continue;
            //goto cleanup;
        }

        mActiveSensors.push_back(sensor);
    }

    for (auto& sensor : mActiveSensors)
    {
        QObject::connect(mpController, &cExperimentControlModel::requestDataRecordingState, sensor, &cSensorModel::dataRecordingStateChange);
    }

    if (!mpController->startCommunications())
    {
        goto cleanup;
    }

    auto* pDispatcher = eventDispatcher();

    forever
    {
        if (pDispatcher->hasPendingEvents())
        {
            pDispatcher->processEvents(QEventLoop::ExcludeUserInputEvents);
        }

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
        QObject::disconnect(mpController, &cExperimentControlModel::requestDataRecordingState, sensor, &cSensorModel::dataRecordingStateChange);
    }

cleanup:
    // Shutdown the network communications that are tied to this thread
    for (auto& sensor : mSensors)
    {
        sensor->stopCommunications();
    }

    mpController->stopCommunications();

    QString msg("Data collection thread terminated.");

    emit statusMessage(msg);
}
