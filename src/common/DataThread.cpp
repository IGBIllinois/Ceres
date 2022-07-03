
#include "DataThread.hpp"
#include "Sensors/SensorModel.hpp"

#include <QAbstractEventDispatcher>

cDataThread::cDataThread()
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

bool cDataThread::startCommunications()
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

    return true;
}

bool cDataThread::stopCommunications()
{
    // Shutdown the network communications that are tied to this thread
    for (auto& sensor : mSensors)
    {
        sensor->stopCommunications();
    }

    return true;
}

void cDataThread::updateAll()
{
    for (auto& sensor : mActiveSensors)
    {
        sensor->update();
    }
}

void cDataThread::run()
{
    if (!startCommunications())
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

        updateAll();

    }

cleanup:
    stopCommunications();

    QString msg("Data collection thread terminated.");

    emit statusMessage(msg);
}
