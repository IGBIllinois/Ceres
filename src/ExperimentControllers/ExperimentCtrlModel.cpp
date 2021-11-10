
#include "ExperimentCtrlModel.hpp"
#include "ExperimentStates.hpp"



cExperimentControlModel::cExperimentControlModel()
    :
    QObject(),
    mpFile(nullptr),
    mDataBuffer(1024),
    mRunning(false),
    mActiveStateNumber(0),
    mpActiveState(nullptr)
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
    return mpFile && mRecording;
}

bool cExperimentControlModel::hasExperiment() const
{
    return !mExperiment.empty();
}

void cExperimentControlModel::clearExperiment()
{
    if (mRunning) return;

    mActiveStateNumber = 0;
    mpActiveState = nullptr;
    mExperiment.clear();
}

bool cExperimentControlModel::startExperiment()
{
    if (mExperiment.empty())
        return false;

    if (mRunning)
        return false;

    mRunning = true;
    mActiveStateNumber = 0;
    mpActiveState = mExperiment[mActiveStateNumber];

    return true;
}

void cExperimentControlModel::terminateExperiment()
{
    if (!mRunning) return;

    mRunning = false;
    emit updateRecordingState(false);
    emit statusMessage("Experiment stopped!");
}


void cExperimentControlModel::updateExperimentStateMachine()
{
    if (!mRunning || (mpActiveState == nullptr)) return;

    mRecording = mpActiveState->recording();

    if (mRecording.HasChanged())
        emit updateRecordingState(mRecording);

    mpActiveState->run();

    if (mpActiveState->finished())
    {
        ++mActiveStateNumber;

        if (mActiveStateNumber < mExperiment.size())
        {
            mpActiveState = mExperiment[mActiveStateNumber];
            mpActiveState->initialize();
            QString msg;
            msg.sprintf("Step %d: ", mActiveStateNumber);
            msg += mpActiveState->getStatusStr();
            emit statusMessage(msg);
        }
        else
        {
            emit updateRecordingState(false);
            mRunning = false;
            emit statusMessage("Experiment completed!");
        }
    }
}

