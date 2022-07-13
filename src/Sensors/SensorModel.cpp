
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
        case eStatus::WARM_UP:
            return "WARM UP";
        case eStatus::RUNNING:
            return "RUNNING";
        case eStatus::STOPPED:
            return "STOPPED";
        case eStatus::FAILED:
            return "FAILED";
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
        if (str == "WARM UP" || str == "warm up")
            return eStatus::WARM_UP;
        if (str == "RUNNING" || str == "running")
            return eStatus::RUNNING;
        if (str == "STOPPED" || str == "stopped")
            return eStatus::STOPPED;
        if (str == "FAILED" || str == "failed")
            return eStatus::FAILED;

        return eStatus::UNKNOWN;
    }
}


cSensorModel::cSensorModel(const std::string& name, QObject* parent)
:
    QObject(parent),
    mSensorName(name)
{
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

bool cSensorModel::initialize()
{
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


