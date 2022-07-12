
#include "SensorModel.hpp"


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

    emit sensorStatusChanging(eSensorStatus::CONFIGURED);

    return true;
}

bool cSensorModel::initialize()
{
    emit sensorStatusChanging(eSensorStatus::INITIALIZED);

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

void cSensorModel::updateName(const std::string& name)
{
    QString old_name = QString::fromStdString(mSensorName);
    QString new_name = QString::fromStdString(name);

    mSensorName = name;

    emit sensorNameChanging(old_name, new_name);
}


