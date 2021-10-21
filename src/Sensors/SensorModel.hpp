
#pragma once

#include "../BlockDataFile/BlockDataFile.hpp"
#include "../BlockDataFile/RawDataBuffer.hpp"
#include "SensorIdentifiers.hpp"

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
    virtual void configure(const nlohmann::json& jsonCfg);

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
    cSensorModel(QObject* parent = nullptr);

    /**
     * A non-owning pointer to the data file
     */
    cBlockDataFile* mpFile;

    /**
     * The mutex for guarding the data file pointer
     */
    std::mutex mFileMutex;

    cRawDataBuffer mDataBuffer;

    /**
     * Basic Sensor Information
     */
    std::string mManufacturer;
    std::string mModel;
    std::string mSerialNumber;
};

