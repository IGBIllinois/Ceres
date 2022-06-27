
#include "DataModel.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"

// Static Data
std::chrono::time_point<std::chrono::high_resolution_clock> cDataModel::mStartTime;

// Static Methods
std::uint64_t cDataModel::timestamp_ns()
{
    const auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStartTime).count();
}


cDataModel::cDataModel(QObject* parent)
:
    QObject(parent)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cDataModel::onStatusUpdate);
}

cDataModel::~cDataModel()
{
}

void cDataModel::onStatusUpdate(QString msg)
{
    emit statusMessage(msg);
}

void cDataModel::onErrorUpdate(QString title, QString msg)
{
    emit errorMessage(title, msg);
}

void cDataModel::addExperimentControlModel(cExperimentControlModel* pControlModel)
{
    if (pControlModel)
    {
        mThread.mpController = pControlModel;
        QObject::connect(mThread.mpController, &cExperimentControlModel::experimentStateChanged, this, &cDataModel::onExperimentStateChange);
        mThread.mpController->moveToThread(&mThread);
    }
}


void cDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        pSensor->moveToThread(&mThread);
        mThread.mSensors.push_back(pSensor);
    }
}

void cDataModel::startDataThread()
{
    mThread.start();
}

void cDataModel::stopDataThread()
{
    mThread.stop();
}

std::string cDataModel::experimentTitle() const
{
    return mExperimentTitle;
}

bool cDataModel::isExperimentRunning()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentRunning();
}

bool cDataModel::isExperimentPaused()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentPaused();
}

bool cDataModel::isExperimentLoaded() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->hasExperiment();
}

bool cDataModel::loadExperiment(const nlohmann::json& expDoc)
{
    if (isExperimentRunning())
    {
        return false;
    }

    if (!expDoc.contains("experiment"))
    {
        return false;
    }
    
    std::string ctrl = expDoc["controller"];
    if (ctrl.compare(mThread.mpController->descriptor()) != 0)
    {
        return false;
    }

    auto required_sensors = expDoc["sensors"];

    for (auto required_sensor : required_sensors)
    {
        bool found = false;

        for (auto& sensor : mThread.mActiveSensors)
        {
            if (required_sensor == sensor->descriptor())
            {
                found = true;
                break;
            }
        }
    }

    if (mThread.mpController->loadExperiment(expDoc["experiment"]))
    {
        mExperimentTitle = static_cast<std::string>(expDoc["experiment_name"]);
        mExperimentDoc = to_string(expDoc);
    }

    return false;
}

void cDataModel::pauseExperiment()
{
    if (mThread.mpController)
        mThread.mpController->pauseExperiment();
}

void cDataModel::terminateExperiment()
{
    if (mThread.mpController)
        mThread.mpController->terminateExperiment();
}


void cDataModel::onExperimentStateChange(int s)
{
    using namespace experiment;

    auto state = to_state(s);

    switch (state)
    {
        case State::COMPLETED:
        case State::TERMINATED:
        {
            mThread.mpController->stopDataRecording();

            for (auto& sensor : mThread.mActiveSensors)
            {
                sensor->endDataRecording();
            }

            closeDataFile();

            mExperimentTitle.clear();
            mExperimentDoc.clear();

            emit experimentCompleted();

            break;
        }
    }
}

