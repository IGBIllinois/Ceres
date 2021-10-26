
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

class cDataModel : public QThread
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

    void startDataRecording(const std::string& filename);
    void stopDataRecording();

signals:
    void statusMessage(QString msg);

private slots:
    void onStatusUpdate(QString msg);

protected:
    void run() override;

private:
    cExperimentControlModel* mpController;
    std::vector<cSensorModel*> mActiveSensors;

    cBlockDataFile   mFile;

    QMutex mMutex;
    bool mAbort = false;

};

