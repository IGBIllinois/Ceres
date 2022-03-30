
#pragma once

//#include "../BlockDataFile/BlockDataFile.hpp"
//#include "../BlockDataFile/RawDataBuffer.hpp"
//#include "ExperimentCtrlIdentifiers.hpp"
#include "../Utilities/Utilities.hpp"

#include <QObject>
#include <nlohmann/json.hpp>
//#include <mutex>
#include <vector>

// Forward Declarations
class cExperimentState;
class cBlockDataFile;


class cExperimentControlModel : public QObject
{
    Q_OBJECT

public:
	virtual ~cExperimentControlModel() = default;

    /*
     * Returns a string used as a descriptor of the experiment
     * controller.
     */
    virtual char* descriptor() const = 0;

    /*
     * Apply any configuration parameters to the experiment
     * controller model.
     */
    virtual void configure(const nlohmann::json& jsonCfg) = 0;

    /*
     * Load an experiment from JSON file.
     */
    virtual bool loadExperiment(const nlohmann::json& expDoc) = 0;

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
     * Clear the experiment (state machine) from the experiment
    * controller.
    */
    bool isExperimentRunning();

    /**
     * Started the loaded experiment.
     * 
     * Returns true if the experiment was started, false otherwise.
     */
    virtual bool startExperiment();

    /**
     * Terminate a running experiment, otherwise just returns.
     */
    virtual void terminateExperiment();

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    virtual void writeDataHeader(cBlockDataFile& file) = 0;

    /*
     * Sets the non-owning data file pointer to
     * start the writing of sensor data.
     */
//    virtual void startDataRecording();

//    virtual void pauseDataRecording() = 0;

    /*
     * Sets the non-owning data file pointer to null to
     * stop the writing of sensor data.
     */
    virtual void stopDataRecording() = 0;

    /*
     * Returns true if sensor data is being recorded
     */
    bool isRecording();

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    virtual bool startCommunications() = 0;
    virtual void stopCommunications() = 0;

signals:
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);

    void requestDataRecordingState(bool record);
    void experimentTerminated();

public:
    virtual void update() = 0;

protected:
    cExperimentControlModel();

    void recordingStateChanged(bool recording);

    void updateExperimentStateMachine();

    /**
     * A flag to signal that an experiment is active
     */
    bool mRunning;

    edge_detect<bool>	mRecording;

    std::vector<cExperimentState*> mExperiment;
    std::size_t mActiveStateNumber;
    cExperimentState* mpActiveState;

};