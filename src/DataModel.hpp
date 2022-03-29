
#pragma once

#include "BlockDataFile/BlockDataFile.hpp"
#include "DataThread.hpp"
#include "ExperimentSerializer.hpp"

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

    bool openDataFile(const std::string& filename);
    void closeDataFile();

    bool isExperimentRunning();

    bool loadExperiment(const nlohmann::json& expDoc);
    void startExperiment(const nlohmann::json& expDoc);
    void terminateExperiment();

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

    cBlockDataFile          mFile;
    cExperimentSerializer   mSerializer;

    QMutex mMutex;
};

