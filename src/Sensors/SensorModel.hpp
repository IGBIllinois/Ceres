
#pragma once

#include <QObject>
#include <nlohmann/json.hpp>

// Forward Declarations
class cBlockDataFileWriter;


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
     * Returns a string used as a class descriptor of the 
     * type sensor.
     */
    virtual char* sensorClass() const = 0;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    virtual char* descriptor() const = 0;

    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    virtual bool configure(const nlohmann::json& jsonCfg);

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    virtual void writeDataHeader(cBlockDataFileWriter& file) = 0;

    /*
     * Detach the serializer.
     */
    virtual void endDataRecording() = 0;

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

public slots:
    /*
     * Toggles the recording state of the sensor.
     */
    void dataRecordingStateChange(bool record);

signals:
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);

public:
    virtual void update() = 0;

protected:
    cSensorModel(QObject* parent = nullptr);

protected:
    /**
     * A flag to signal that recording is active
     */
    bool mIsRecording;

    /**
     * Basic Sensor Information
     */
    std::string mManufacturer;
    std::string mModel;
    std::string mSerialNumber;
};

