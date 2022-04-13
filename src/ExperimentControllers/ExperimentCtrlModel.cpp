
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

bool cExperimentControlModel::isExperimentRunning() const
{
    return mRunning;
}

bool cExperimentControlModel::isExperimentPaused() const
{
    return mPaused;
}

void cExperimentControlModel::clearExperiment()
{
    if (mRunning) return;

    mActiveStateNumber = 0;
    mpActiveState = nullptr;

    for (std::size_t i = 0; i < mExperiment.size(); ++i)
    {
        delete mExperiment[i];
        mExperiment[i] = nullptr;
    }

    mExperiment.clear();
}

cExperimentState* cExperimentControlModel::createState(const std::string& type)
{
    if (type == "delay")
        return new cExperimentState_Delay();

    if (type == "pause")
        return new cExperimentState_Pause();

    return nullptr;
}


bool cExperimentControlModel::loadExperiment(const nlohmann::json& expDoc)
{
    using namespace experiment;

    if (mRunning)
        return false;

    clearExperiment();

    mExperiment.push_back(new cExperimentState_Dummy());

    for (auto entry : expDoc)
    {
        std::string type = entry["type"];

        cExperimentState* pState = createState(type);

        if (pState)
        {
            pState->configure(entry);
            mExperiment.push_back(pState);
        }
    }

    emit experimentStateChanged(to_int(State::LOADED));

    return true;
}


void cExperimentControlModel::startExperiment()
{
    using namespace experiment;

    if (mExperiment.empty())
        return;

    if (mPaused)
    {
        mPaused = false;
        return;
    }

    if (mRunning)
        return;

    mRunning = true;
    mActiveStateNumber = 0;
    mpActiveState = mExperiment[mActiveStateNumber];

    emit experimentStateChanged(to_int(State::RUNNING));
}

void cExperimentControlModel::terminateExperiment()
{
    using namespace experiment;

    if (!mRunning)
    {
        mPaused = false;
        return;
    }

    recordingStateChanged(false);

    mRunning = false;
    mPaused = false;
    emit experimentStateChanged(to_int(State::TERMINATED));
    emit statusMessage("Experiment stopped!");
}

void cExperimentControlModel::pauseExperiment()
{
    using namespace experiment;

    if (!mRunning || (mpActiveState == nullptr))
    {
        mPaused = false;
        return;
    }

    mPaused = true;
    emit experimentStateChanged(to_int(State::PAUSED));
    emit statusMessage("Experiment paused...");
}


void cExperimentControlModel::updateExperimentStateMachine()
{
    using namespace experiment;

    if (!mRunning || (mpActiveState == nullptr)) return;

    mRecording = mpActiveState->recording();

    if (mRecording.HasChanged())
        recordingStateChanged(mRecording);

    if (mPaused)
    {
        mpActiveState->pause();
        return;
    }

    mpActiveState->run();

    auto result = mpActiveState->finished();
    if (result == cExperimentState::eRESULT::ABORT)
    {
        terminateExperiment();
    }

    if (result == cExperimentState::eRESULT::DONE)
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
            emit experimentStateChanged(to_int(State::COMPLETED));
            emit statusMessage("Experiment completed!");
        }
    }
}

