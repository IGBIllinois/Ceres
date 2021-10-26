
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
    std::string ctrl = expDoc["controller"];
    if (ctrl.compare(mpController->descriptor()) != 0)
    {

    }
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
    forever
    {
        if (mAbort)
            return;

        mpController->run();

        for (auto& sensor : mActiveSensors)
        {
            sensor->run();
        }

    }
}
