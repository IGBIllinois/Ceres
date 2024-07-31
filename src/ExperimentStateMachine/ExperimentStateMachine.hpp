
#pragma once

#include "ExperimentTypes.hpp"
#include "../Utilities/Utilities.hpp"

#include <QObject>
#include <nlohmann/json.hpp>
#include <vector>
#include <mutex>
#include <memory>

// Forward Declarations
class cExperimentState;
class cExperimentStateCreator;
class cExperimentVariableTable;


class cExperimentStateMachine : public QObject
{
    Q_OBJECT

public:
	virtual ~cExperimentStateMachine() = default;

    void addStateCreator(cExperimentStateCreator* pCreator);
    void removeStateCreator(cExperimentStateCreator* pCreator);
    void clearStateCreators();

    /*
     * Load an experiment from JSON file.
     */
    bool loadExperiment(const std::string& expName, const nlohmann::json& expDoc);

    /*
     * Is there an experiment (state machine) loaded in the experiment
     * controller.
     */
    bool hasExperiment() const;

    /*
     * Clear the experiment (state machine) from the experiment
     * controller.
     */
    void clearExperiment();

    /*
     * Is the experiment (state machine) running?
    */
    bool isExperimentRunning() const;

    /*
     * Is the experiment (state machine) paused?
    */
    bool isExperimentPaused() const;

    /**
     * Is the experiment going to record any of the sensor data.
     * 
     * Returns true if the experiment will save sensor data, false otherwise.
     */
	bool experimentRequiresDataFile() const;

    /**
     * Terminate a running experiment, otherwise just returns.
     */
//    virtual void terminateExperiment();

    /*
     * Returns true if sensor data is being recorded
     */
    bool isRecording();

signals:
    void experimentStatus(QString msg);
    void experimentStateChanged(experiment::eState state);
    void stateNumberChanged(int id);
    void requestDataRecordingState(bool record);

 /**
  * Slots for controlling experiment state machine.
  */
public slots:
    virtual void startExperiment();
    virtual void terminateExperiment();
    virtual void pauseExperiment();

public:
    void updateExperimentStateMachine();

protected:
    cExperimentStateMachine(QObject* parent = nullptr);

    virtual cExperimentState* createState(const std::string& type, const nlohmann::json& expState);

    void recordingStateChanged(bool recording);

    virtual void emitStatusMessage(const QString& msg) = 0;

protected:
    /**
     * A flag to signal that an experiment is active
     */
    bool mRunning;
    bool mPaused;

    edge_detect<bool>	mRecording;

private:
    std::mutex mStateCreatorsMutex;
    std::vector<cExperimentStateCreator*> mStateCreators;

    std::string mExperimentName;

    std::vector<cExperimentState*> mExperimentStates;
    std::size_t mActiveStateNumber;
    cExperimentState* mpActiveState;

    std::mutex mPendingDeleteMutex;
    std::vector<cExperimentState*> mPendingDelete;

    std::shared_ptr<cExperimentVariableTable> mVariableTable;
};