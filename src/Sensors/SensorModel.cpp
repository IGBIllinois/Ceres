
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

/*
void cSensorModel::startDataRecording(cBlockDataFile& file)
{
    std::lock_guard<std::mutex> guard(mFileMutex);
    mpFile = &file;
}
*/

/*
void cSensorModel::stopDataRecording()
{
    std::lock_guard<std::mutex> guard(mFileMutex);
    mpFile = nullptr;
}
*/

bool cSensorModel::isRecording()
{
//    return mIsRecording && mpFile;
    return mIsRecording;
}

void cSensorModel::recordingStateUpdated(bool recording)
{
    mIsRecording = recording;

/*
    if (recording)
    {
        if (canSaveData())
        {
            mIsRecording = true;
        }
    }
    else
    {
        mIsRecording = false;
    }
*/
}

