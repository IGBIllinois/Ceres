
#include "DataThread.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"


cDataThread::cDataThread()
:
    mpController(nullptr)
{
}

cDataThread::~cDataThread()
{
    stop();
}

void cDataThread::start()
{
    if (!isRunning()) 
    {
        mAbort = false;
        QThread::start(TimeCriticalPriority);
    }
}

void cDataThread::stop()
{
    mAbort = true;

    wait();
}

/*
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
*/

void cDataThread::run()
{
    // Start the network communications so that communication are tied to this thread
    if (!mpController->startCommunications())
    {
        goto cleanup;
    }
    for (auto& sensor : mActiveSensors)
    {
        if (!sensor->startCommunications())
        {
            goto cleanup;
        }
    }

    for (auto& sensor : mActiveSensors)
    {
        QObject::connect(mpController, &cExperimentControlModel::recordingStateChanged, sensor, &cSensorModel::recordingStateUpdated);
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
        QObject::disconnect(mpController, &cExperimentControlModel::recordingStateChanged, sensor, &cSensorModel::recordingStateUpdated);
    }

cleanup:
    // Shutdown the network communications that are tied to this thread
    for (auto& sensor : mActiveSensors)
    {
        sensor->stopCommunications();
    }

    mpController->stopCommunications();
}
