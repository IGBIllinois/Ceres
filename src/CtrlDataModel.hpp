
#pragma once

#include "DataThread.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>
#include <nlohmann/json.hpp>
#include <chrono>

// Forward Declarations
class cExperimentControlModel;
class cSensorModel;

namespace experiment
{
    enum class State : uint8_t;
}

/*****************************************************************************
 * 
 * The cCtrlDataModel class is the base class for experiment control and
 * data acquisition.
 * 
 *****************************************************************************/
class cCtrlDataModel : public QObject
{
    Q_OBJECT

public:
    explicit cCtrlDataModel(QObject* parent = nullptr);
    ~cCtrlDataModel();

    static std::uint64_t timestamp_ns();

    void addExperimentControlModel(cExperimentControlModel* pModel);
    void addSensor(cSensorModel* pSensor);

    virtual void startDataThread();
    virtual void stopDataThread();

    virtual bool openDataFile(const QString& defaultPath) = 0;
    virtual void closeDataFile() = 0;

    bool isExperimentRunning();
    bool isExperimentPaused();

    std::string experimentTitle() const;

    bool isExperimentLoaded() const;
    bool loadExperiment(const nlohmann::json& expDoc);

    virtual void startExperiment() = 0;
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

protected:
    cDataThread mThread;

    std::string  mExperimentTitle;
    std::string  mExperimentDoc;

private:
    static std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
};

