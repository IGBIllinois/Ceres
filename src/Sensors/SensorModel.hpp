
#pragma once

#include <QObject>

#include <nlohmann/json.hpp>

#include <string>

// Forward Declarations
class cBlockDataFileWriter;

namespace sensor
{
    enum class eStatus { UNKNOWN, CONFIGURED, INITIALIZED, CONNECTING, WARM_UP, RUNNING, STOPPED, FAILED };

    std::string to_string(eStatus status);
    eStatus to_sensor_status(const std::string& str);
}

const uint8_t   logSTATUS  = 0;
const uint8_t   logINFO    = 1;
const uint8_t   logWARNING = 2;
const uint8_t   logERROR   = 3;

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
     * Returns the class identifier used by the sensor's serializer
     */
    virtual uint16_t data_class_id() const = 0;

    /*
     * Returns a QString used as the name of the sensor.
     */
    const std::string& name() const { return mSensorName; };

    sensor::eStatus status() const { return mStatus; };

    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    virtual bool configure(const nlohmann::json& jsonCfg);

    /*
     * Do any sensor initialization needed before the
     * sensor model is moved to the data thread.
     */
    virtual bool initialize();

    /*
     * Attach/Detach the serializer to the data file.
     */
    virtual void enableDataRecording(cBlockDataFileWriter& file) = 0;
    virtual void disableDataRecording() = 0;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    virtual void writeDataHeader() = 0;
    virtual void writeDataFooter() {};

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
    void logMessage(uint8_t type, QString device, QString msg);

    void sensorStatusChanging(QString name, sensor::eStatus status);
    void sensorNameChanging(QString old_name, QString new_name);

public:
    virtual void update() = 0;

protected:
    cSensorModel(const std::string& name, QObject* parent = nullptr);

    void setStatus(const sensor::eStatus status);
    void updateName(const std::string& name);

    QString q_name() const { return QString::fromStdString(mSensorName); }

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

private:
    std::string mSensorName;
    sensor::eStatus mStatus = sensor::eStatus::UNKNOWN;
};

