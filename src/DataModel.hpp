
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

    void startExperiment(const std::string& filename);
    void terminateExperiment();

/*
    void startDataRecording();
    void stopDataRecording();
*/

signals:
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);

private slots:
    void onStatusUpdate(QString msg);
    void onErrorUpdate(QString title, QString msg);

    void onExperimentTerminated();

private:
    cDataThread mThread;

    cBlockDataFile   mFile;

    QMutex mMutex;
};

