
#pragma once

#include "../BlockDataFile/BlockDataFile.hpp"
#include "../BlockDataFile/RawDataBuffer.hpp"
#include "ExperimentCtrlIdentifiers.hpp"
#include "../Utilities/Utilities.hpp"

#include <QObject>
#include <nlohmann/json.hpp>
#include <mutex>
#include <vector>

class cExperimentState;


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
    virtual void loadExperiment(const nlohmann::json& expDoc) = 0;

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
    void startDataRecording(cBlockDataFile& file);

    /*
     * Sets the non-owning data file pointer to null to
     * stop the writing of sensor data.
     */
    void stopDataRecording();

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

    void recordingStateChanged(bool recording);

public:
    virtual void update() = 0;

protected:
    cExperimentControlModel();

    void updateExperimentStateMachine();

    /**
     * A non-owning pointer to the data file
     */
    cBlockDataFile* mpFile;

    /**
     * The mutex for guarding the data file pointer
     */
    std::mutex mFileMutex;

    cRawDataBuffer mDataBuffer;

    bool mRunning;


    edge_detect<bool>	mRecording;

    std::vector<cExperimentState*> mExperiment;
    std::size_t mActiveStateNumber;
    cExperimentState* mpActiveState;

};