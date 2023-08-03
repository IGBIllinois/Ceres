
#include "ExperimentStateMachine.hpp"
#include "ExperimentState.hpp"
#include "ExperimentStateCreator.hpp"
#include "BasicExperimentStates.hpp"

#include <QThread>

#include <algorithm>

Q_DECLARE_METATYPE(experiment::eState)


cExperimentStateMachine::cExperimentStateMachine(QObject* parent)
:
    QObject(parent),
    mRunning(false),
    mPaused(false),
    mActiveStateNumber(0),
    mpActiveState(nullptr)
{
    qRegisterMetaType<experiment::eState>();
}

void cExperimentStateMachine::addStateCreator(cExperimentStateCreator* pCreator)
{
    std::lock_guard<std::mutex> lock{mStateCreatorsMutex};

    if (std::find(mStateCreators.begin(), mStateCreators.end(), pCreator) == mStateCreators.end())
    {
        mStateCreators.push_back(pCreator);
    }
}

void cExperimentStateMachine::removeStateCreator(cExperimentStateCreator* pCreator)
{
    std::lock_guard<std::mutex> lock{mStateCreatorsMutex};

    auto it = std::remove(mStateCreators.begin(), mStateCreators.end(), pCreator);
}

bool cExperimentStateMachine::isRecording()
{
    return mRecording;
}

void cExperimentStateMachine::recordingStateChanged(bool recording)
{
    emit requestDataRecordingState(recording);
}

bool cExperimentStateMachine::hasExperiment() const
{
    return !mExperimentStates.empty();
}

bool cExperimentStateMachine::experimentRequiresDataFile() const
{
	for (auto* state : mExperimentStates)
	{
        if (state->needsDataFile())
            return true;
	}
	return false;
}

bool cExperimentStateMachine::isExperimentRunning() const
{
    return mRunning;
}

bool cExperimentStateMachine::isExperimentPaused() const
{
    return mPaused;
}

void cExperimentStateMachine::clearExperiment()
{
    if (mRunning) return;

    mActiveStateNumber = 0;
    mpActiveState = nullptr;

    std::lock_guard<std::mutex> lock{mPendingDeleteMutex};

    for (std::size_t i = 0; i < mExperimentStates.size(); ++i)
    {
        mPendingDelete.emplace_back(mExperimentStates[i]);
        mExperimentStates[i] = nullptr;
    }

    mExperimentStates.clear();
}

cExperimentState* cExperimentStateMachine::createState(const std::string& type, const nlohmann::json& expDoc)
{
    if (type == "delay")
        return new cExperimentState_Delay();

    if (type == "pause")
        return new cExperimentState_Pause();

    return nullptr;
}


bool cExperimentStateMachine::loadExperiment(const std::string& expName, const nlohmann::json& expDoc)
{
    using namespace experiment;
    using namespace nlohmann;

    if (mRunning)
        return false;

    clearExperiment();

    mExperimentStates.push_back(new cExperimentState_Dummy());

    try
    {
        for (auto entry : expDoc)
        {
            std::string type = entry["type"];

            cExperimentState* pState = createState(type, entry);

            if (!pState)
            {
                for (auto* creator : mStateCreators)
                {
                    pState = creator->createState(type, entry);
                }
            }

            if (pState)
            {
                pState->configure(entry);
                mExperimentStates.push_back(pState);
            }
        }
    }
    catch (const detail::parse_error& e)
    {
        QString msg = "Experiment \"";
        msg += QString::fromStdString(expName);
        msg += "\" failed to load due to parse error.";
        emitStatusMessage(msg);

        emit experimentStateChanged(eState::EXP_ERROR);
        return false;
    }
    catch (const detail::type_error& e)
    {
        QString msg = "Experiment \"";
        msg += QString::fromStdString(expName);
        msg += "\" failed to load due to type error.";
        emitStatusMessage(msg);

        emit experimentStateChanged(eState::EXP_ERROR);
        return false;
    }
    catch (const detail::exception& e)
    {
        QString msg = "Experiment \"";
        msg += QString::fromStdString(expName);
        msg += "\" failed to load due to unknown error.";
        emitStatusMessage(msg);

        emit experimentStateChanged(eState::EXP_ERROR);
        return false;
    }

    mExperimentName = expName;

    emit experimentStateChanged(eState::LOADED);

    QString msg = "Experiment \"";
    msg += QString::fromStdString(mExperimentName);
    msg += "\" is loaded.";
    emitStatusMessage(msg);

    return true;
}


void cExperimentStateMachine::startExperiment()
{
    using namespace experiment;

    if (mExperimentStates.empty())
        return;

    if (mPaused)
    {
        mPaused = false;
        emit experimentStateChanged(eState::RUNNING);

        QString msg;
        msg.sprintf("Step %d: ", mActiveStateNumber);
        msg += mpActiveState->getStatusStr();
        emit experimentStatus(msg);

        return;
    }

    if (mRunning)
        return;

    mRunning = true;
    mActiveStateNumber = 0;
    mpActiveState = mExperimentStates[mActiveStateNumber];
    mpActiveState->initialize();

    emit experimentStateChanged(eState::RUNNING);

    QString msg = "Running experiment: ";
    msg += QString::fromStdString(mExperimentName);
    emitStatusMessage(msg);
}

void cExperimentStateMachine::terminateExperiment()
{
    using namespace experiment;

    if (!mRunning)
    {
        mPaused = false;
        emit experimentStateChanged(eState::TERMINATED);
        return;
    }

    recordingStateChanged(false);

    if (mpActiveState)
    {
        mpActiveState->stop();
    }

    mRunning = false;
    mPaused = false;
    mExperimentName.clear();
    emit experimentStateChanged(eState::TERMINATED);
}

void cExperimentStateMachine::pauseExperiment()
{
    using namespace experiment;

    if (!mRunning || (mpActiveState == nullptr))
    {
        mPaused = false;
        return;
    }

    mPaused = true;
    emit experimentStateChanged(eState::PAUSED);
    emit experimentStatus("Experiment paused...");
}


void cExperimentStateMachine::updateExperimentStateMachine()
{
    using namespace experiment;

    if (!mPendingDelete.empty())
    {
        std::lock_guard<std::mutex> lock{mPendingDeleteMutex};
        for (auto state : mPendingDelete)
        {
            delete state;
        }

        mPendingDelete.clear();
    }

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

        if (mActiveStateNumber < mExperimentStates.size())
        {
            mpActiveState->cleanup();
            mpActiveState = mExperimentStates[mActiveStateNumber];
            if (mpActiveState->initialize())
            {
                QString msg;
                msg.sprintf("Step %d: ", mActiveStateNumber);
                msg += mpActiveState->getStatusStr();
                emit experimentStatus(msg);
            }
            else
            {
                recordingStateChanged(false);
                mpActiveState->cleanup();
                mRunning = false;
                mExperimentName.clear();
                emit experimentStateChanged(eState::EXP_ERROR);
            }
        }
        else
        {
            recordingStateChanged(false);
            mpActiveState->cleanup();
            mRunning = false;
            mExperimentName.clear();
            emit experimentStateChanged(eState::COMPLETED);
        }
    }
}

