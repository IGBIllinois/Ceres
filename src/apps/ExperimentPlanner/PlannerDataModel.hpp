
#pragma once

#include "DataModel.hpp"
#include "PlannerDataThread.hpp"

#include <spidercam/spidercam_types.hpp>

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <vector>
#include <ctime>

#include <nlohmann/json.hpp>

// Forward Declarations
//class cExperimentControlModel;
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

    bool isConnected() const;

    std::size_t sensorCount() const override;
    void addSensor(cSensorModel* pSensor) override;

    virtual void addExperimentControlModel(cExperimentControlModel* pModel);
    virtual cExperimentControlModel* removeExperimentControlModel();

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

    std::string measurementTitle() const;

    bool isExperimentLoaded() const;
    bool experimentRequiresDataFile() const;
    virtual bool loadExperiment(const std::string& expName, const nlohmann::json& expDoc);
    virtual bool unloadExperiment();

    virtual void startExperiment() = 0;
    void pauseExperiment();
    void terminateExperiment();

    spidercam::sPosition_1_t getPosition() const;

signals:
    void connectedToController();
    void disconnectedFromController();

signals:
    void limitsChanged(spidercam::sWorkingDimensions limits);
    void positionChanged(spidercam::sPosition_1_t pos);
    void recordingStateChanged(bool recording);

signals:
    void experimentStatus(QString msg);
    void experimentStateChanged(experiment::eState state);

signals:
    void experimentTerminated();
    void experimentCompleted();

protected slots:
    /*
     * Toggles the recording state of the sensor.
     */
    virtual void dataRecordingStateChange(bool record) = 0;

private slots:
    void updateLimits(spidercam::sWorkingDimensions limits);
    void updatePosition(spidercam::sPosition_1_t pos);
    void updateRecordingState(bool recording);

private slots:
    void onExperimentStateChange(experiment::eState state);
    void onDataThreadTermination();

private slots:
    void controllerConnected();
    void controllerDisconnected();

protected:
    virtual void endDataRecording() = 0;

protected:
    std::string mMeasurementTitle;

    bool mFileOpen = false;

    bool mRecording = false;

    spidercam::sWorkingDimensions mLimits = { 10000, 190000, 10000, 190000, 1000, 9000 };
    spidercam::sPosition_1_t mCurrentPosition = spidercam::sPosition_1_t();

    bool mConnected = false;

    cPlannerDataThread mThread;

private:
    void doExperimentCleanup();
};

