
#include "SensorModel.hpp"


cSensorModel::cSensorModel(QObject* parent)
:
    QObject(parent),
    mpFile(nullptr),
    mDataBuffer(1024)
{
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
