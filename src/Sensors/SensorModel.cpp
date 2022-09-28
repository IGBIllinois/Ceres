
#include "SensorModel.hpp"

namespace sensor
{
    std::string to_string(eStatus status)
    {
        switch (status)
        {
        case eStatus::UNKNOWN:
            return "UNKNOWN";
        case eStatus::CONFIGURED:
            return "CONFIGURED";
        case eStatus::INITIALIZED:
            return "INITIALIZED";
        case eStatus::CONNECTING:
            return "CONNECTING";
        case eStatus::CONNECTED:
            return "CONNECTED";
        case eStatus::WARM_UP:
            return "WARM UP";
        case eStatus::RUNNING:
            return "RUNNING";
        case eStatus::STOPPED:
            return "STOPPED";
        case eStatus::FAILED:
            return "FAILED";
        case eStatus::REINITIALIZING:
            return "REINITIALIZING";
        case eStatus::BUSY:
            return "BUSY";
        case eStatus::WAITING:
            return "WAITING";
        }

        return "UNKNOWN";
    }

    eStatus to_sensor_status(const std::string& str)
    {
        if (str == "CONFIGURED" || str == "configured")
            return eStatus::CONFIGURED;
        if (str == "INITIALIZED" || str == "initialized")
            return eStatus::INITIALIZED;
        if (str == "CONNECTING" || str == "connecting")
            return eStatus::CONNECTING;
        if (str == "CONNECTED" || str == "connected")
            return eStatus::CONNECTED;
        if (str == "WARM UP" || str == "warm up")
            return eStatus::WARM_UP;
        if (str == "RUNNING" || str == "running")
            return eStatus::RUNNING;
        if (str == "STOPPED" || str == "stopped")
            return eStatus::STOPPED;
        if (str == "FAILED" || str == "failed")
            return eStatus::FAILED;
        if (str == "REINITIALIZING" || str == "reinitializing")
            return eStatus::REINITIALIZING;
        if (str == "BUSY" || str == "busy")
            return eStatus::BUSY;
        if (str == "WAITING" || str == "waiting")
            return eStatus::WAITING;

        return eStatus::UNKNOWN;
    }
}

Q_DECLARE_METATYPE(sensor::eStatus)


cSensorModel::cSensorModel(const std::string& name, QObject* parent)
:
    QObject(parent),
    mSensorName(name)
{
    qRegisterMetaType<sensor::eStatus>();

    mIsRecording = false;
}

bool cSensorModel::configure(const nlohmann::json& jsonCfg)
{
    if (!jsonCfg.contains("Manufacturer"))
        throw std::logic_error("Missing \"Manufacturer\" entry.");
    mManufacturer = jsonCfg["Manufacturer"];

    if (!jsonCfg.contains("Model"))
        throw std::logic_error("Missing \"Model\" entry.");
    mModel = jsonCfg["Model"];

    if (!jsonCfg.contains("Serial Number"))
        throw std::logic_error("Missing \"Serial Number\" entry.");
    mSerialNumber = jsonCfg["Serial Number"];

    setStatus(sensor::eStatus::CONFIGURED);
    return true;
}

bool cSensorModel::isInitialized() const
{
    return mIsInitialized;
}

bool cSensorModel::initialize()
{
    mIsInitialized = true;
    setStatus(sensor::eStatus::INITIALIZED);
    return true;
};

void cSensorModel::enableDataRecording(cBlockDataFileWriter& file)
{
    mIsRecording = false;
}

void cSensorModel::disableDataRecording()
{
    mIsRecording = false;
}

void cSensorModel::dataRecordingStateChange(bool record)
{
    mIsRecording = record;
}


bool cSensorModel::isRecording()
{
    return mIsRecording;
}

void cSensorModel::setStatus(sensor::eStatus status)
{
    mStatus = status;
    emit sensorStatusChanging(q_name(), mStatus);
}

void cSensorModel::updateName(const std::string& name)
{
    QString old_name = QString::fromStdString(mSensorName);
    QString new_name = QString::fromStdString(name);

    mSensorName = name;

    emit sensorNameChanging(old_name, new_name);
}


