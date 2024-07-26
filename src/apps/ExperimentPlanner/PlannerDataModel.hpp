
#pragma once

#include "DataModel.hpp"
#include "PlannerDataThread.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <vector>
#include <ctime>

#include <nlohmann/json.hpp>

// Forward Declarations
class cExperimentControlModel;
//class cSensorModel;
//class cSensorPropertyPage;

namespace experiment
{
    enum class eState : uint8_t;
}

/*****************************************************************************
 * 
 * The cPlannerDataModel class is the base class for experiment control and
 * planning.
 * 
 *****************************************************************************/
class cPlannerDataModel : public cDataModel
{
    Q_OBJECT

public:
    explicit cPlannerDataModel(QObject* parent = nullptr);
    ~cPlannerDataModel();

    std::size_t sensorCount() const override;
    void addSensor(cSensorModel* pSensor) override;

    virtual void addExperimentControlModel(cExperimentControlModel* pModel);

    void startDataThread() override;
    void stopDataThread() override;

    virtual bool openDataFile(const QString& defaultPath, 
                              const std::string& defaultFileName,
                              bool autoSave) = 0;

    virtual bool isDataFileOpen() const = 0;
    virtual void closeDataFile() = 0;

    virtual bool systemReady() const;

    bool isExperimentRunning();
    bool isExperimentPaused();

    std::string experimentTitle() const;

    bool isExperimentLoaded() const;
    bool experimentRequiresDataFile() const;
    virtual bool loadExperiment(const std::string& expName, const nlohmann::json& expDoc);
    virtual bool unloadExperiment();

    virtual void startExperiment() = 0;
    void pauseExperiment();
    void terminateExperiment();

signals:
    void experimentTerminated();
    void experimentCompleted();

protected slots:
    /*
     * Toggles the recording state of the sensor.
     */
    virtual void dataRecordingStateChange(bool record) = 0;

private slots:
    void onExperimentStateChange(experiment::eState state);
    void onDataThreadTermination();

protected:
    virtual void endDataRecording() = 0;

protected:
    std::string mExperimentTitle;

    bool mFileOpen = false;

    cPlannerDataThread mThread;

private:
    void doExperimentCleanup();
};

