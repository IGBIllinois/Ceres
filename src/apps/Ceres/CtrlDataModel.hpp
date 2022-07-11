
#pragma once

#include "DataModel.hpp"
#include "CtrlDataThread.hpp"

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

/*****************************************************************************
 * 
 * The cCtrlDataModel class is the base class for experiment control and
 * data acquisition.
 * 
 *****************************************************************************/
class cCtrlDataModel : public cDataModel
{
    Q_OBJECT

public:
    explicit cCtrlDataModel(QObject* parent = nullptr);
    ~cCtrlDataModel();

    virtual void addExperimentControlModel(cExperimentControlModel* pModel);
    void addSensor(cSensorModel* pSensor) override;

    void startDataThread() override;
    void stopDataThread() override;

    virtual bool openDataFile(const QString& defaultPath) = 0;
    virtual bool isDataFileOpen() const = 0;
    virtual void closeDataFile() = 0;

    bool isExperimentRunning();
    bool isExperimentPaused();

    std::string experimentTitle() const;

    bool isExperimentLoaded() const;
    virtual bool loadExperiment(const nlohmann::json& expDoc);

    virtual void startExperiment() = 0;
    void pauseExperiment();
    void terminateExperiment();

signals:
/*
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);
*/

    void experimentCompleted();

protected slots:
    /*
     * Toggles the recording state of the sensor.
     */
    virtual void dataRecordingStateChange(bool record) = 0;

private slots:
    void onExperimentStateChange(int state);

protected:
    virtual void endDataRecording() = 0;

protected:
    std::string  mExperimentTitle;
    std::string  mResearcher;
    std::string  mCultivar;
    std::string  mExperimentDoc;

    cCtrlDataThread mThread;
};

