
#include "RemoteDataThread.hpp"
#include "RemoteDataModel.hpp"
#include "SensorModel.hpp"


cRemoteDataThread::cRemoteDataThread(cRemoteDataModel* pController)
    :
    mpController(pController)
{
    mHeartBeatTimer.interval_sec(3);
    mHeartBeatTimer.reset();

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

    emit statusMessage("");

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

    emit statusMessage("");

    return true;
}

void cRemoteDataThread::run()
{
    mStartTime = std::chrono::steady_clock::now();
    cDataThread::run();
}

void cRemoteDataThread::updateAll()
{
    ++mLoopCount;

    cDataThread::updateAll();

    auto endTime = std::chrono::steady_clock::now();
    auto diff_us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - mStartTime).count();

    if (diff_us >= 1'000'000)
    {
        float time_ms = diff_us / 1000.0f;
        mAvgLoopTime_ms = time_ms / mLoopCount;
        mLoopCount = 0;
        mStartTime = endTime;
        emit updateLoopTime(mAvgLoopTime_ms);
    }

    if (mHeartBeatTimer.elapsed())
    {
        emit updateLoopHeartbeat();
    }
}
