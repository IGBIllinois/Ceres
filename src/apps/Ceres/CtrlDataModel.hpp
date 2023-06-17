
#pragma once

#include "DataModel.hpp"
#include "CtrlDataThread.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <vector>
#include <ctime>

#include <nlohmann/json.hpp>

// Forward Declarations
class cExperimentControlModel;
class cSensorModel;
class cSensorPropertyPage;

namespace experiment
{
    enum class eState : uint8_t;
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

    void addSensorPropertyPage(cSensorPropertyPage* pPage);
    void removeSensorPropertyPage(cSensorPropertyPage* pPage);

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

protected:
    virtual void endDataRecording() = 0;

protected:
    std::string  mExperimentTitle;
    std::string  mPrincipalInvestigator;
    std::string  mResearcher;
    std::string  mSpecies;
    std::string  mCultivar;
    std::string  mPermitInfo;
    std::string  mExperimentDoc;
    std::vector<std::string> mTreatments;

    std::string mConstructName;
    std::string mEventNumber;
    std::string mFieldDesign;

    std::time_t mPlantingDate;
    std::time_t mHarvestDate;

    std::vector<std::string> mComments;

    cCtrlDataThread mThread;

    std::vector<cSensorPropertyPage*> mPropertyPages;

private:
    void doExperimentCleanup();
};

