
#include "SensorModel.hpp"


cSensorModel::cSensorModel(QObject* parent)
:
    QObject(parent)
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

    return true;
}

void cSensorModel::writeDataHeader(cBlockDataFileWriter& file)
{
    mIsRecording = false;
}

void cSensorModel::dataRecordingStateChange(bool record)
{
    mIsRecording = record;
}

void cSensorModel::endDataRecording()
{
    mIsRecording = false;
}


bool cSensorModel::isRecording()
{
    return mIsRecording;
}


