
#include "ExperimentCtrlModel.hpp"
#include "ExperimentStates.hpp"



cExperimentControlModel::cExperimentControlModel()
    :
    QObject(),
    mRunning(false),
    mActiveStateNumber(0),
    mpActiveState(nullptr)
{
}


bool cExperimentControlModel::isRecording()
{
    return mRecording;
}

void cExperimentControlModel::recordingStateChanged(bool recording)
{
    emit requestDataRecordingState(recording);
}

bool cExperimentControlModel::hasExperiment() const
{
    return !mExperiment.empty();
}

bool cExperimentControlModel::isExperimentRunning()
{
    return mRunning;
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

    recordingStateChanged(false);

    mRunning = false;
    emit experimentTerminated();
    emit statusMessage("Experiment stopped!");
}


void cExperimentControlModel::updateExperimentStateMachine()
{
    if (!mRunning || (mpActiveState == nullptr)) return;

    mRecording = mpActiveState->recording();

    if (mRecording.HasChanged())
        recordingStateChanged(mRecording);

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
            recordingStateChanged(false);
            mRunning = false;
            emit experimentTerminated();
            emit statusMessage("Experiment completed!");
        }
    }
}

