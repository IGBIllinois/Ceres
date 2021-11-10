
#include "DataModel.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"


cDataModel::cDataModel(QObject* parent)
:
    QObject(parent)
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
    {
        mThread.mpController = pControlModel;
        mThread.mpController->moveToThread(&mThread);
    }
}


void cDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        pSensor->moveToThread(&mThread);
        mThread.mActiveSensors.push_back(pSensor);
    }
}

void cDataModel::startDataThread()
{
    mThread.start();
}

void cDataModel::stopDataThread()
{
    mThread.stop();

    if (mFile.isOpen())
    {
        mThread.mpController->stopDataRecording();

        for (auto& sensor : mThread.mActiveSensors)
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
    if (ctrl.compare(mThread.mpController->descriptor()) != 0)
    {
        return;
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

    mThread.mpController->loadExperiment(expDoc["experiment"]);
}

void cDataModel::startExperiment()
{
    mThread.mpController->startExperiment();
}

void cDataModel::terminateExperiment()
{
    mThread.mpController->terminateExperiment();
}

void cDataModel::startDataRecording(const std::string& filename)
{
    mFile.open(filename);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataHeader(mFile);
    }

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->startDataRecording(mFile);
    }
}

void cDataModel::stopDataRecording()
{
    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->stopDataRecording();
    }

    mFile.close();

}

