
#include "DataThread.hpp"
#include "../Sensors/SensorModel.hpp"

#include <QAbstractEventDispatcher>
#include <QDebug>

extern bool g_AppIsClosing;

cDataThread::cDataThread()
{
    mHeartBeatTimer.interval_sec(3);
}

cDataThread::~cDataThread()
{
    abort();
}

void cDataThread::start()
{
    if (!isRunning()) 
    {
        mStop = false;
        mAbort = false;
        QThread::start(TimeCriticalPriority);
    }
}

void cDataThread::stop()
{
    disconnect();

    mStop = true;

    wait();
}

void cDataThread::abort()
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
    try
    {
        mHeartBeatTimer.start();

        if (!startCommunications())
        {
            QString msg("Data collection thread: Communications failed during startup.");
            emit errorMessage("Critical Error", msg);

            goto cleanup;
        }
    }
    catch (const std::exception& e)
    {
        qCritical() << e.what();

        QString msg("Data collection thread terminal error: ");
        msg += e.what();

        emit errorMessage("Critical Error", msg);

        goto cleanup;
    }

    try
    {
        QAbstractEventDispatcher* pDispatcher = eventDispatcher();

        forever
        {
//            if (pDispatcher && pDispatcher->hasPendingEvents())
            {
                pDispatcher->processEvents(QEventLoop::ExcludeUserInputEvents);
            }

            if (mStop)
            {
                goto cleanup;
            }

            if (mAbort)
                return;

            updateAll();

            if (mHeartBeatTimer.elapsed())
            {
                emit updateLoopHeartbeat();
            }
        }
    }
    catch (const std::exception& e)
    {
        qCritical() << e.what();

        QString msg("Data collection thread terminal error: ");
        msg += e.what();

        emit errorMessage("Critical Error", msg);
    }

cleanup:

    mHeartBeatTimer.stop();

    try
    {
        stopCommunications();
    }
    catch (const std::exception& e)
    {
        if (!g_AppIsClosing)
            qCritical() << e.what();
    }

    mActiveSensors.clear();

    if (!g_AppIsClosing)
    {
        QString msg("Data collection thread terminated.");

        emit statusMessage(msg);
        emit terminated();
    }
}
