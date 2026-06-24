
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
	virtual ~cExperimentStateMachine();

    void addStateCreator(cExperimentStateCreator* pCreator);
    void removeStateCreator(cExperimentStateCreator* pCreator);
    void clearStateCreators();

    /*
     * In batch mode, variables will be preserved between runs.
     */
    bool inBatchMode() const;
    void setBatchMode(bool mode);

    /*
     * Load an experiment from JSON file.
     */
    bool loadExperiment(const std::string& exp_path, const std::string& exp_name, const nlohmann::json& expDoc);

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

    /*
     * Create the global experiment variable table
     */
    void createGlobalVariableTable();

    /*
     * Clear the global experiment variable table
     */
    void clearGlobalVariableTable();

    /*
     * Returns the global experiment variable table that affects all experiments
     */
    std::weak_ptr<cExperimentVariableTable> getGlobalVariableTable() const;

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

    std::vector<cExperimentState*> loadMeasurementStates(const std::string& root_path, const std::string& include_filename);

    virtual cExperimentState* createState(const std::string& type, const nlohmann::json& expState);

    void recordingStateChanged(bool recording);

    virtual void emitStatusMessage(const QString& msg) = 0;

private:
    void addVariables(const nlohmann::json& variables);

protected:
    /**
     * A flag to signal that an experiment is active
     */
    bool mRunning = false;
    bool mPaused = false;

    edge_detect<bool>	mRecording = false;

private:
    /**
     * A flag to signal we are in "batch mode" and should not clear the variable table
     */
    bool mBatchMode = false;

    std::mutex mStateCreatorsMutex;
    std::vector<cExperimentStateCreator*> mStateCreators;

    std::string mExperimentName;

    std::vector<cExperimentState*> mExperimentStates;
    std::size_t mActiveStateNumber;
    cExperimentState* mpActiveState;

    std::mutex mPendingDeleteMutex;
    std::vector<cExperimentState*> mPendingDelete;

    std::shared_ptr<cExperimentVariableTable> mGlobalVariableTable;
    std::shared_ptr<cExperimentVariableTable> mVariableTable;
};