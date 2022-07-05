
#include "CtrlDataModel.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"


cCtrlDataModel::cCtrlDataModel(QObject* parent)
:
    cDataModel(parent)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cCtrlDataModel::onStatusUpdate);
}

cCtrlDataModel::~cCtrlDataModel()
{
}


void cCtrlDataModel::addExperimentControlModel(cExperimentControlModel* pControlModel)
{
    if (pControlModel)
    {
        mThread.mpController = pControlModel;
        QObject::connect(mThread.mpController, &cExperimentControlModel::experimentStateChanged, this, &cCtrlDataModel::onExperimentStateChange);
        mThread.mpController->moveToThread(&mThread);
    }
}


void cCtrlDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        pSensor->moveToThread(&mThread);
        mThread.mSensors.push_back(pSensor);
    }
}

void cCtrlDataModel::startDataThread()
{
    mThread.start();
}

void cCtrlDataModel::stopDataThread()
{
    mThread.stop();
}

std::string cCtrlDataModel::experimentTitle() const
{
    return mExperimentTitle;
}

bool cCtrlDataModel::isExperimentRunning()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentRunning();
}

bool cCtrlDataModel::isExperimentPaused()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentPaused();
}

bool cCtrlDataModel::isExperimentLoaded() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->hasExperiment();
}

bool cCtrlDataModel::loadExperiment(const nlohmann::json& expDoc)
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

void cCtrlDataModel::pauseExperiment()
{
    if (mThread.mpController)
        mThread.mpController->pauseExperiment();
}

void cCtrlDataModel::terminateExperiment()
{
    if (mThread.mpController)
        mThread.mpController->terminateExperiment();
}


void cCtrlDataModel::onExperimentStateChange(int s)
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

