
#include "SensorModel.hpp"


cSensorModel::cSensorModel()
/*
:
    mpFile(nullptr),
    mDataBuffer(1024)
*/
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

void cSensorModel::writeDataHeader(cBlockDataFile& file)
{
    mIsRecording = false;
}

void cSensorModel::startDataRecording()
{
    mIsRecording = true;
}

void cSensorModel::pauseDataRecording()
{
    mIsRecording = false;
}

void cSensorModel::stopDataRecording()
{
    mIsRecording = false;
}


bool cSensorModel::isRecording()
{
    return mIsRecording;
}


