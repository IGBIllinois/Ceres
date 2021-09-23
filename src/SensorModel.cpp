
#include "SensorModel.hpp"


cSensorModel::cSensorModel(QObject* parent)
:
    QObject(parent),
    mpFile(nullptr),
    mDataBuffer(1024)
{
}

void cSensorModel::configure(nlohmann::json& jsonCfg)
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
}

void cSensorModel::startDataRecording(cDataFile& file)
{
    std::lock_guard<std::mutex> guard(mFileMutex);
    mpFile = &file;
}

void cSensorModel::stopDataRecording()
{
    std::lock_guard<std::mutex> guard(mFileMutex);
    mpFile = nullptr;
}

bool cSensorModel::isRecording()
{
    return mpFile;
}
