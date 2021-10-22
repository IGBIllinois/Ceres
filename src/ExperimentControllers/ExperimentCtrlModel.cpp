
#include "ExperimentCtrlModel.hpp"




cExperimentControlModel::cExperimentControlModel()
    :
    QObject(),
    mpFile(nullptr),
    mDataBuffer(1024)
{
}


void cExperimentControlModel::startDataRecording(cBlockDataFile& file)
{
    std::lock_guard<std::mutex> guard(mFileMutex);
    mpFile = &file;
}

void cExperimentControlModel::stopDataRecording()
{
    std::lock_guard<std::mutex> guard(mFileMutex);
    mpFile = nullptr;
}

bool cExperimentControlModel::isRecording()
{
    return mpFile;
}
