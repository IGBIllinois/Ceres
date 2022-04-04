
#include "ExperimentCtrlModel.hpp"
#include "ExperimentStates.hpp"
#include "../ExperimentTypes.hpp"



cExperimentControlModel::cExperimentControlModel(QObject* parent)
:
    QObject(parent),
    mRunning(false),
    mPaused(false),
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

void cExperimentControlModel::startExperiment()
{
    if (mExperiment.empty())
        return;

    if (mRunning)
        return;

    if (mPaused)
    {
        mPaused = false;
        return;
    }

    mRunning = true;
    mActiveStateNumber = 0;
    mpActiveState = mExperiment[mActiveStateNumber];

    emit experimentStateChanged(experiment::State::RUNNING);
}

void cExperimentControlModel::terminateExperiment()
{
    if (!mRunning)
    {
        mPaused = false;
        return;
    }

    recordingStateChanged(false);

    mRunning = false;
    mPaused = false;
    emit experimentStateChanged(experiment::State::TERMINATED);
    emit statusMessage("Experiment stopped!");
}

void cExperimentControlModel::pauseExperiment()
{
    if (!mRunning || (mpActiveState == nullptr))
    {
        mPaused = false;
        return;
    }

    mPaused = true;
}


void cExperimentControlModel::updateExperimentStateMachine()
{
    if (!mRunning || (mpActiveState == nullptr)) return;

    mRecording = mpActiveState->recording();

    if (mRecording.HasChanged())
        recordingStateChanged(mRecording);

    if (mPaused) return;

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
            emit experimentStateChanged(experiment::State::COMPLETED);
            emit statusMessage("Experiment completed!");
        }
    }
}

