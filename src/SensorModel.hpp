
#pragma once

#include "DataFile.hpp"
#include "DataFileBlockIdentifiers.hpp"
#include "DataBuffer.hpp"

#include <QObject>
#include <mutex>
#include <nlohmann/json.hpp>



/**
 * Abstract Base Class for all Sensor Based Models
 * 
 * 
 */

class cSensorModel : public QObject
{
    Q_OBJECT

public:
    virtual ~cSensorModel() = default;

    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    virtual void configure(nlohmann::json& jsonCfg) = 0;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    virtual void writeDataHeader(cDataFile& file) = 0;

    /*
     * Sets the non-owning data file pointer to
     * start the writing of sensor data.
     */
    void startDataRecording(cDataFile& file);

    /*
     * Sets the non-owning data file pointer to null to
     * stop the writing of sensor data.
     */
    void stopDataRecording();

    /*
     * Returns true if sensor data is being recorded
     */
    bool isRecording();

protected:
    cSensorModel(QObject* parent = nullptr);

    /**
     * A non-owning pointer to the data file
     */
    cDataFile* mpFile;

    /**
     * The mutex for guarding the data file pointer
     */
    std::mutex mFileMutex;

    cDataBuffer mDataBuffer;
};

