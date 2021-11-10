
#pragma once

#include "BlockDataFile/BlockDataFile.hpp"
#include "DataThread.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>
#include <nlohmann/json.hpp>

// Forward Declarations
class cExperimentControlModel;
class cSensorModel;

class cDataModel : public QObject
{
    Q_OBJECT

public:
    explicit cDataModel(QObject* parent = nullptr);
    ~cDataModel();

    void addExperimentControlModel(cExperimentControlModel* pSensor);
    void addSensor(cSensorModel* pSensor);

    void startDataThread();
    void stopDataThread();

    void loadExperiment(const nlohmann::json& expDoc);

    void startExperiment();
    void terminateExperiment();

    void startDataRecording(const std::string& filename);
    void stopDataRecording();

signals:
    void statusMessage(QString msg);

private slots:
    void onStatusUpdate(QString msg);

private:
    cDataThread mThread;

    cBlockDataFile   mFile;

    QMutex mMutex;
};

