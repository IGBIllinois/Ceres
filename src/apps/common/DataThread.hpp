
#pragma once

#include "../../Utilities/Timers.hpp"

#include <QThread>
#include <vector>

// Forward Declarations
class cSensorModel;

class cDataThread : public QThread
{
    Q_OBJECT

public:
    explicit cDataThread();
    virtual ~cDataThread();

    void start();
    void stop();

    void abort();

signals:
    void statusMessage(QString msg);
    void errorMessage(QString title, QString msg);
    void updateLoopHeartbeat();
    void terminated();

protected:
    void run() override;
    virtual void updateAll();

    virtual bool startCommunications();
    virtual bool stopCommunications();

public:
    std::vector<cSensorModel*> mSensors;
    std::vector<cSensorModel*> mActiveSensors;

private:
    cIntervalTimer mHeartBeatTimer;

private:
    bool mStop  = false;
    bool mAbort = false;
};

