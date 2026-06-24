
#pragma once

#include "DataModel.hpp"
#include "CtrlDataThread.hpp"
#include "ExperimentStateCreator.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <vector>
#include <ctime>
#include <memory>

#include <nlohmann/json.hpp>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

// Forward Declarations
class cExperimentControlModel;
class cSensorModel;
class cSensorPropertyPage;
class cExperimentVariableTable;

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
class cCtrlDataModel : public cDataModel, public cExperimentStateCreator
{
    Q_OBJECT

public:
    explicit cCtrlDataModel(QObject* parent = nullptr);
    ~cCtrlDataModel();

    virtual void addExperimentControlModel(cExperimentControlModel* pModel);

    std::size_t sensorCount() const override;
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

    bool isInBatchMode() const;
    void setBatchMode(bool mode);

    std::weak_ptr<cExperimentVariableTable> getGlobalVariableTable() const;
    void clearGlobalVariableTable();

    bool isExperimentRunning();
    bool isExperimentPaused();

    std::string measurementTitle() const;

    bool isExperimentLoaded() const;
    bool experimentRequiresDataFile() const;
    virtual bool loadExperiment(const std::string& exp_path, const std::string& exp_name, const nlohmann::json& expDoc);
    virtual bool unloadExperiment();

    virtual void startExperiment() = 0;
    void pauseExperiment();
    void terminateExperiment();

signals:
    void connectToSensorMenu(QAction* pAction);
    void disconnectFromSensorMenu(QAction* pAction);

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
    std::string  mExperimentTitle;
    std::string  mMeasurementTitle;
    std::string  mPrincipalInvestigator;
    std::vector<std::string> mResearchers;
    std::string  mSpecies;
    std::string  mCultivar;
    std::string  mAuthorization;
    std::string  mPermit;
    std::string  mExperimentDoc;
    std::vector<std::string> mTreatments;

    std::string mConstructName;
    std::vector<std::string> mEventNumbers;
    std::string mFieldDesign;

    std::time_t mPlantingDate;
    std::time_t mHarvestDate;

    std::vector<std::string> mComments;

    cCtrlDataThread mThread;

//    std::vector<cExperimentStateCreator*> mStateCreators;

private:
    void doMeasurementCleanup();
};

