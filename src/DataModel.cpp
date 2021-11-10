
#include "DataModel.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"


cDataModel::cDataModel(QObject* parent)
:
    QThread(parent),
    mpController(nullptr)
{
}

cDataModel::~cDataModel()
{
    stopDataThread();
}

void cDataModel::onStatusUpdate(QString msg)
{
    emit statusMessage(msg);
}

void cDataModel::addExperimentControlModel(cExperimentControlModel* pControlModel)
{
    if (pControlModel)
        mpController = pControlModel;
}


void cDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
        mActiveSensors.push_back(pSensor);
}

void cDataModel::startDataThread()
{
    if (!isRunning()) 
    {
        start(TimeCriticalPriority);
    }
}

void cDataModel::stopDataThread()
{
    mMutex.lock();
    mAbort = true;
    mMutex.unlock();

    wait();

    if (mFile.isOpen())
    {
        mpController->stopDataRecording();

        for (auto& sensor : mActiveSensors)
        {
            sensor->stopDataRecording();
        }

        mFile.close();
    }
}

void cDataModel::loadExperiment(const nlohmann::json& expDoc)
{
    if (!expDoc.contains("experiment"))
    {
        return;
    }

    std::string ctrl = expDoc["controller"];
    if (ctrl.compare(mpController->descriptor()) != 0)
    {
        return;
    }

    auto required_sensors = expDoc["sensors"];

    for (auto required_sensor : required_sensors)
    {
        bool found = false;

        for (auto& sensor : mActiveSensors)
        {
            if (required_sensor == sensor->descriptor())
            {
                found = true;
                break;
            }
        }
    }

    mpController->loadExperiment(expDoc["experiment"]);
}

void cDataModel::startExperiment()
{
    mpController->startExperiment();
}

void cDataModel::terminateExperiment()
{
    mpController->terminateExperiment();
}

void cDataModel::startDataRecording(const std::string& filename)
{
    mFile.open(filename);

    for (auto& sensor : mActiveSensors)
    {
        sensor->writeDataHeader(mFile);
    }

    for (auto& sensor : mActiveSensors)
    {
        sensor->startDataRecording(mFile);
    }
}

void cDataModel::stopDataRecording()
{
    for (auto& sensor : mActiveSensors)
    {
        sensor->stopDataRecording();
    }

    mFile.close();

}

void cDataModel::run()
{
    for (auto& sensor : mActiveSensors)
    {
        QObject::connect(mpController, &cExperimentControlModel::updateRecordingState, sensor, &cSensorModel::recordingStateUpdated);
    }

    forever
    {
        if (mAbort)
            return;

        mpController->update();

        for (auto& sensor : mActiveSensors)
        {
            sensor->update();
        }
    }

    for (auto& sensor : mActiveSensors)
    {
        QObject::disconnect(mpController, &cExperimentControlModel::updateRecordingState, sensor, &cSensorModel::recordingStateUpdated);
    }
}
