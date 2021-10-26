
#pragma once

#include "../BlockDataFile/BlockDataFile.hpp"
#include "../BlockDataFile/RawDataBuffer.hpp"
#include "ExperimentCtrlIdentifiers.hpp"

#include <QObject>
#include <nlohmann/json.hpp>
#include <mutex>


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

    virtual void loadExperiment(const nlohmann::json& expDoc) = 0;

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

signals:
    void statusMessage(QString msg);

public:
    virtual void run() = 0;

protected:
    cExperimentControlModel();

    /**
     * A non-owning pointer to the data file
     */
    cBlockDataFile* mpFile;

    /**
     * The mutex for guarding the data file pointer
     */
    std::mutex mFileMutex;

    cRawDataBuffer mDataBuffer;


};