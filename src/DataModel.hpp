
#pragma once

#include "BlockDataFile/BlockDataFile.hpp"
#include "DataThread.hpp"
#include "BlockDataFile/ExperimentSerializer.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>
#include <nlohmann/json.hpp>

// Forward Declarations
class cExperimentControlModel;
class cSensorModel;

namespace experiment
{
    enum class State : uint8_t;
}


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
    bool isExperimentPaused();

    std::string experimentTitle() const;
    bool isExperimentLoaded() const;
    bool loadExperiment(const nlohmann::json& expDoc);
    void startExperiment();
    void pauseExperiment();
    void terminateExperiment();

signals:
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);

    void experimentCompleted();

private slots:
    void onStatusUpdate(QString msg);
    void onErrorUpdate(QString title, QString msg);

    void onExperimentStateChange(int state);

private:
    cDataThread mThread;

    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;

    std::string  mExperimentTitle;
    std::string  mExperimentDoc;

    QMutex mMutex;
};

