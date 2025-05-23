
#pragma once

#include <QObject>

#include <nlohmann/json.hpp>

#include <string>

// Forward Declarations
class cBlockDataFileWriter;

namespace sensor
{
    enum class eStatus 
    { UNKNOWN, CONFIGURED, INITIALIZED, CONNECTING, CONNECTED, WARM_UP, 
        RUNNING, STOPPED, FAILED, REINITIALIZING, BUSY, WAITING, PENDING };

    std::string to_string(eStatus status);
    eStatus to_sensor_status(const std::string& str);
}

Q_DECLARE_METATYPE(sensor::eStatus)


const quint8   logSTATUS  = 0;
const quint8   logINFO    = 1;
const quint8   logWARNING = 2;
const quint8   logERROR   = 3;

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
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: GPS, LiDAR, etc.
     */
    virtual uint8_t device_id() const = 0;

    /*
     * Returns a string used as a class descriptor of the 
     * type sensor.
     */
    virtual const char* sensorClass() const = 0;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    virtual const char* descriptor() const = 0;

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    virtual uint16_t data_class_id() const = 0;

    /*
     * Returns a string used as the name of the sensor.
     */
    const std::string& name() const { return mSensorName; };

    /*
     * Returns a string used as the sensor instance.
     */
    bool has_instance() const { return !mSensorInstance.empty(); };
    const std::string& instance() const { return mSensorInstance; };

    /*
     * Returns the manufacturer of the sensor as a string.
     */
    virtual const std::string& manufacturer() const { return mManufacturer; };

    /*
     * Returns the model of the sensor as a string.
     */
    virtual const std::string& model() const { return mModel; };

    /*
     * Returns the serial number of the sensor as a string.
     */
    virtual const std::string& serial_number() const { return mSerialNumber; };

    /*
     * Returns an enumeration of the current sensor status.
     */
    sensor::eStatus getStatus() const { return mStatus; };

    /*
     * Emit all status messages to update all views
     */
    virtual void updateViews() = 0;

    /*
     * Apply any configuration parameters to the sensor
     * model.
     * 
     * A sensor model is configured first..
     */
    virtual bool configure(const nlohmann::json& jsonCfg);

    /*
     * Do any sensor initialization needed before the
     * sensor model is moved to the data thread.
     * 
     * and then initialize is called.
     */
    virtual bool isInitialized() const;
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

    void setInstanceName(const std::string& instance);

public slots:
    /*
     * Toggles the recording state of the sensor.
     */
    virtual void dataRecordingStateChange(bool record);

signals:
    void statusMessage(QString msg);

    void elogMessage(quint8 type, QString device, QString instance, QString msg);

    void sensorStatusChanging(QString name, QString instance, sensor::eStatus status);
    void sensorNameChanging(QString old_name, QString new_name, QString instance);

public:
    /*
     * Call by the data thread to process an incoming data
     */
    virtual void update() = 0;

protected:
    cSensorModel(const std::string& name, QObject* parent = nullptr);
    cSensorModel(const std::string& name, const std::string& instance, QObject* parent = nullptr);

    void setStatus(const sensor::eStatus status);
    void updateName(const std::string& name);
    void logMessage(quint8 type, QString msg);

    QString q_name() const      { return QString::fromStdString(mSensorName); }
    QString q_instance() const  { return QString::fromStdString(mSensorInstance); }

protected:
    bool mIsInitialized = false;

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
    std::string mSensorInstance;
    sensor::eStatus mStatus = sensor::eStatus::UNKNOWN;
};

