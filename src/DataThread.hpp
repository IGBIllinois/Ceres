
#pragma once

#include "BlockDataFile/BlockDataFile.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>
#include <nlohmann/json.hpp>

// Forward Declarations
class cExperimentControlModel;
class cSensorModel;

class cDataThread : public QThread
{
    Q_OBJECT

public:
    explicit cDataThread();
    ~cDataThread();

    void start();
    void stop();

signals:
    void statusMessage(QString msg);

protected:
    void run() override;

public:
    cExperimentControlModel* mpController;
    std::vector<cSensorModel*> mActiveSensors;

private:
    bool mAbort = false;
};

