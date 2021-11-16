
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

//    void addExperimentControlModel(cExperimentControlModel* pSensor);
//    void addSensor(cSensorModel* pSensor);

    void start();
    void stop();

//    void loadExperiment(const nlohmann::json& expDoc);

//    void startExperiment();
//    void terminateExperiment();

//    void startDataRecording(const std::string& filename);
//    void stopDataRecording();

protected:
    void run() override;

public:
    cExperimentControlModel* mpController;
    std::vector<cSensorModel*> mActiveSensors;

private:
    bool mAbort = false;
};

