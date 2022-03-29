
#include "DataModel.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"


cDataModel::cDataModel(QObject* parent)
:
    QObject(parent),
    mSerializer(4096)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cDataModel::onStatusUpdate);
}

cDataModel::~cDataModel()
{
    stopDataThread();
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
        QObject::connect(mThread.mpController, &cExperimentControlModel::experimentTerminated, this, &cDataModel::onExperimentTerminated);
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

    if (mFile.isOpen())
    {
        mThread.mpController->stopDataRecording();

        for (auto& sensor : mThread.mActiveSensors)
        {
            sensor->endDataRecording();
        }

        mFile.close();
    }
}

bool cDataModel::openDataFile(const std::string& filename)
{
    if (mFile.isOpen())
        return false;

    mFile.open(filename);

    mSerializer.attach(&mFile);

    return mFile.isOpen();
}

void cDataModel::closeDataFile()
{
    mSerializer.detach();
    mFile.close();
}

bool cDataModel::isExperimentRunning()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentRunning();
}


bool cDataModel::loadExperiment(const nlohmann::json& expDoc)
{
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

    return mThread.mpController->loadExperiment(expDoc["experiment"]);
}

void cDataModel::startExperiment(const nlohmann::json& expDoc)
{
    mThread.mpController->writeDataHeader(mFile);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataHeader(mFile);
    }

    mSerializer.startTime(time(0));
    mThread.mpController->startExperiment();
}

void cDataModel::terminateExperiment()
{
    mThread.mpController->terminateExperiment();
}


void cDataModel::onExperimentTerminated()
{
    mThread.mpController->stopDataRecording();

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->endDataRecording();
    }

    mSerializer.endTime(time(0));
    closeDataFile();
}

