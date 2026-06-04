
#pragma once

#include "DataThread.hpp"
#include "../../Utilities/Timers.hpp"

#include <chrono>


// Forward Declarations
class cRemoteDataModel;
class cSensorController;


class cRemoteDataThread : public cDataThread
{
    Q_OBJECT

public:
    explicit cRemoteDataThread(cRemoteDataModel* pController);
    virtual ~cRemoteDataThread();

    bool startCommunications() override;
    bool stopCommunications() override;

signals:
    void updateLoopHeartbeat();
    void updateLoopTime(float avg_loop_time_ms);

protected:
    void run() override;
    void updateAll() override;

public:
    cRemoteDataModel* mpController;

protected:
    cIntervalTimer mHeartBeatTimer;
    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
    int mLoopCount = 0;
    float mAvgLoopTime_ms = 0.0f;
};

