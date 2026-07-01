
#include "ExperimentStateMachine.hpp"
#include "ExperimentState.hpp"
#include "ExperimentStateCreator.hpp"
#include "BasicExperimentStates.hpp"
#include "ExperimentVariableTable.hpp"

#include <QThread>
#include <QString>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>

Q_DECLARE_METATYPE(experiment::eState)


namespace
{
    class invalid_include_file : public std::exception
    {
    public:
        invalid_include_file() = default;
        ~invalid_include_file() = default;
    };

    void delete_states(std::vector<cExperimentState*>& states)
    {
        for (cExperimentState* state : states)
        {
            delete state;
            state = nullptr;
        }

        states.clear();
    }
}

cExperimentStateMachine::cExperimentStateMachine(QObject* parent)
:
    QObject(parent),
    mStateCreatorsMutex(),
    mRunning(false),
    mPaused(false),
    mActiveStateNumber(0),
    mpActiveState(nullptr)
{
    qRegisterMetaType<experiment::eState>();
}

cExperimentStateMachine::~cExperimentStateMachine()
{
    mStateCreators.clear();

    std::lock_guard<std::mutex> lock{ mPendingDeleteMutex };

    for (std::size_t i = 0; i < mExperimentStates.size(); ++i)
    {
        mPendingDelete.emplace_back(mExperimentStates[i]);
        mExperimentStates[i] = nullptr;
    }

    mExperimentStates.clear();

    for (auto state : mPendingDelete)
    {
        delete state;
    }

    mPendingDelete.clear();
}


bool cExperimentStateMachine::inBatchMode() const
{
    return mBatchMode;
}

void cExperimentStateMachine::setBatchMode(bool mode)
{
    mBatchMode = mode;
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

    if (it != mStateCreators.end())
        mStateCreators.erase(it);
}

void cExperimentStateMachine::clearStateCreators()
{
    std::lock_guard<std::mutex> lock{ mStateCreatorsMutex };

    mStateCreators.clear();
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
    emit stateNumberChanged(-1);

    std::lock_guard<std::mutex> lock{mPendingDeleteMutex};

    for (std::size_t i = 0; i < mExperimentStates.size(); ++i)
    {
        mPendingDelete.emplace_back(mExperimentStates[i]);
        mExperimentStates[i] = nullptr;
    }

    mExperimentStates.clear();

    if (!mBatchMode && mVariableTable)
        mVariableTable->clear();
}

void cExperimentStateMachine::createGlobalVariableTable()
{
    if (mRunning) return;

    if (!mGlobalVariableTable)
        mGlobalVariableTable = std::make_shared<cExperimentVariableTable>();
}

void cExperimentStateMachine::clearGlobalVariableTable()
{
    if (mRunning) return;

    if (mGlobalVariableTable)
        mGlobalVariableTable->clear();
}

std::weak_ptr<cExperimentVariableTable> cExperimentStateMachine::getGlobalVariableTable() const
{
    return mGlobalVariableTable;
}

cExperimentState* cExperimentStateMachine::createState(const std::string& type, const nlohmann::json& expDoc)
{
    if (type == "delay")
        return new cExperimentState_Delay();

    if (type == "pause")
        return new cExperimentState_Pause();

    return nullptr;
}


bool cExperimentStateMachine::loadExperiment(const std::string& exp_path, const std::string& expName, const nlohmann::json& expDoc)
{
    using namespace experiment;
    using namespace nlohmann;

    if (mRunning)
        return false;

    clearExperiment();

    mpActiveState = nullptr;

    if (!mVariableTable)
        mVariableTable = std::make_shared<cExperimentVariableTable>();

    if (mGlobalVariableTable)
    {
        for (const auto& entry : *mGlobalVariableTable)
        {
            if (mVariableTable->contains(entry.first))
                continue;

            mVariableTable->insert(entry);
        }
    }

    mExperimentStates.push_back(new cExperimentState_Dummy());

    try
    {
        for (auto entry : expDoc)
        {
            if (entry.contains("variables"))
            {
                const auto& variables = entry["variables"];
                addVariables(variables);

                continue;
            }

            if (entry.contains("include"))
            {
                std::string filename = entry["include"];

                std::vector<cExperimentState*> states = loadMeasurementStates(exp_path, filename);

                for (auto* state : states)
                {
                    if (state)
                    {
                        mExperimentStates.push_back(std::move(state));
                    }
                }

                states.clear();

                continue;
            }

            std::string type = entry["type"];

            cExperimentState* pState = createState(type, entry);

            if (!pState)
            {
                for (auto* creator : mStateCreators)
                {
                    pState = creator->createState(type, entry, this);

                    if (pState)
                    {
                        break;
                    }
                }
            }

            if (pState)
            {
                pState->attachVariableTable(mVariableTable);
                if (pState->configure(entry))
                {
                    mExperimentStates.push_back(pState);
                }
                else
                {
                    emit experimentStateChanged(eState::EXP_ERROR);

                    mExperimentStates.push_back(pState);
                    delete_states(mExperimentStates);

                    return false;
                }
            }
        }
    }
    catch (const invalid_include_file&)
    {
        emit experimentStateChanged(eState::EXP_ERROR);

        delete_states(mExperimentStates);

        return false;
    }
    catch (const detail::parse_error& e)
    {
        QString msg = "Experiment \"";
        msg += QString::fromStdString(expName);
        msg += "\" failed to load due to parse error.";
        emitStatusMessage(msg);

        emit experimentStateChanged(eState::EXP_ERROR);

        delete_states(mExperimentStates);

        return false;
    }
    catch (const detail::type_error& e)
    {
        QString msg = "Experiment \"";
        msg += QString::fromStdString(expName);
        msg += "\" failed to load due to type error.";
        emitStatusMessage(msg);

        emit experimentStateChanged(eState::EXP_ERROR);

        delete_states(mExperimentStates);

        return false;
    }
    catch (const detail::exception& e)
    {
        QString msg = "Experiment \"";
        msg += QString::fromStdString(expName);
        msg += "\" failed to load due to unknown error.";
        emitStatusMessage(msg);

        emit experimentStateChanged(eState::EXP_ERROR);

        delete_states(mExperimentStates);

        return false;
    }

    mExperimentName = expName;

    emit experimentStateChanged(eState::LOADED);
    emit experimentStatus(QString("Loading experiment info..."));

    QString msg = "Experiment \"";
    msg += QString::fromStdString(mExperimentName);
    msg += "\" is loaded.";
    emitStatusMessage(msg);

    return true;
}

void cExperimentStateMachine::addVariables(const nlohmann::json& variables)
{
    if (variables.empty())
        return;

    auto items = variables.items();

    for (auto it = items.begin(); it != items.end(); ++it)
    {
        auto name = it.key();
        auto value = it.value();

        if (mVariableTable->contains(name))
        {
            if (value.is_number_float())
                mVariableTable->set(name, value.get<double>());
            else if (value.is_number_unsigned())
                mVariableTable->set(name, value.get<unsigned int>());
            else if (value.is_number_integer())
                mVariableTable->set(name, value.get<int>());
            else if (value.is_boolean())
                mVariableTable->set(name, value.get<bool>());
        }
        else
        {
            if (value.is_number_float())
                mVariableTable->add(name, value.get<double>());
            else if (value.is_number_unsigned())
                mVariableTable->add(name, value.get<unsigned int>());
            else if (value.is_number_integer())
                mVariableTable->add(name, value.get<int>());
            else if (value.is_boolean())
                mVariableTable->add(name, value.get<bool>());
        }

    }
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
        msg  = "Step ";
        msg += QString::number(mActiveStateNumber);
        msg += ": ";
        msg += mpActiveState->getStatusStr();
        emit experimentStatus(msg);

        return;
    }

    if (mRunning)
        return;

    mActiveStateNumber = 0;
    mpActiveState = mExperimentStates[mActiveStateNumber];
    mpActiveState->initialize();

    emit stateNumberChanged(mActiveStateNumber);
    emit experimentStateChanged(eState::RUNNING);

    QString msg = "Running experiment: ";
    msg += QString::fromStdString(mExperimentName);
    emitStatusMessage(msg);

    // Everything is setup so it is ok to update the state maching.
    // We need to be careful as updating the state machine is in 
    // another thread!
    mRunning = true;
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

    // We should stop the updating of the state machine before
    // removing the active state as the state maching is in 
    // another thread!
    mRunning = false;

    if (mpActiveState)
    {
        mpActiveState->stop();
        mpActiveState = nullptr;
    }

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

    if (!mRunning)
    {
        return;
    }

    if (mpActiveState == nullptr)
    {
        mRunning = false;
        return;
    }

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
                msg = "Step ";
                msg += QString::number(mActiveStateNumber);
                msg += ": ";
                msg += mpActiveState->getStatusStr();
                emit stateNumberChanged(mActiveStateNumber);
                emit experimentStatus(msg);
            }
            else
            {
                recordingStateChanged(false);
                mpActiveState->cleanup();
                mRunning = false;
                mExperimentName.clear();
                emit stateNumberChanged(-1);
                emit experimentStateChanged(eState::EXP_ERROR);
            }
        }
        else
        {
            recordingStateChanged(false);
            mpActiveState->cleanup();
            mRunning = false;
            mExperimentName.clear();
            emit stateNumberChanged(-1);
            emit experimentStateChanged(eState::COMPLETED);
        }
    }
}

std::vector<cExperimentState*> cExperimentStateMachine::loadMeasurementStates(const std::string& root_path, const std::string& include_filename)
{
    using namespace experiment;
    using namespace nlohmann;

    std::filesystem::path includeFile = include_filename;

    if (includeFile.is_relative())
    {
        std::filesystem::path rootPath = root_path;

        includeFile = rootPath / includeFile;
    }

    std::ifstream in;
    in.open(includeFile);

    if (!in.is_open())
    {
        throw invalid_include_file();
    }

    nlohmann::json jsonDoc = nlohmann::json::parse(in, nullptr, false, true);

    if (!jsonDoc.contains("experiment"))
    {
        return std::vector<cExperimentState*>();
    }

    nlohmann::json expDoc = jsonDoc["experiment"];

    std::vector<cExperimentState*> states;

    try
    {
        for (auto entry : expDoc)
        {
            if (entry.contains("variables"))
            {
                const auto& variables = entry["variables"];
                addVariables(variables);

                continue;
            }

            if (entry.contains("include"))
            {
                std::string filename = entry["include"];

                std::vector<cExperimentState*> include_states = loadMeasurementStates(root_path, filename);

                for (auto* state : include_states)
                {
                    if (state)
                    {
                        states.push_back(std::move(state));
                    }
                }

                include_states.clear();

                continue;
            }


            std::string type = entry["type"];

            cExperimentState* pState = createState(type, entry);

            if (!pState)
            {
                for (auto* creator : mStateCreators)
                {
                    pState = creator->createState(type, entry, this);

                    if (pState)
                    {
                        break;
                    }
                }
            }

            if (pState)
            {
                pState->attachVariableTable(mVariableTable);
                if (pState->configure(entry))
                    states.push_back(pState);
            }
        }
    }
    catch (const invalid_include_file& )
    {
        delete_states(states);
        throw;
    }
    catch (const detail::parse_error& e)
    {
        QString msg = "Experiment include file \"";
        msg += QString::fromStdString(include_filename);
        msg += "\" failed to load due to parse error.";
        emitStatusMessage(msg);

        delete_states(states);

        throw invalid_include_file();
    }
    catch (const detail::type_error& e)
    {
        QString msg = "Experiment include file \"";
        msg += QString::fromStdString(include_filename);
        msg += "\" failed to load due to type error.";
        emitStatusMessage(msg);

        delete_states(states);

        throw invalid_include_file();
    }
    catch (const detail::exception& e)
    {
        QString msg = "Experiment include file \"";
        msg += QString::fromStdString(include_filename);
        msg += "\" failed to load due to unknown error.";
        emitStatusMessage(msg);

        delete_states(states);

        throw invalid_include_file();
    }


    return states;
}
