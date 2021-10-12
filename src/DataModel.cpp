
#include "DataModel.hpp"
#include "SensorModel.hpp"


cDataModel::cDataModel(QObject* parent)
:
    QThread(parent)
{
}

cDataModel::~cDataModel()
{
    stopDataCollection();
}

void cDataModel::onStatusUpdate(QString msg)
{
    emit statusMessage(msg);
}


void cDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
        mActiveSensors.push_back(pSensor);
}

void cDataModel::startDataCollection()
{
    if (!isRunning()) 
    {
        start(TimeCriticalPriority);
    }
}

void cDataModel::stopDataCollection()
{
    mMutex.lock();
    mAbort = true;
    mMutex.unlock();

    wait();

    if (mFile.isOpen())
    {
        for (auto& sensor : mActiveSensors)
        {
            sensor->stopDataRecording();
        }

        mFile.close();
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

        for (auto& sensor : mActiveSensors)
        {
            sensor->run();
        }

    }
}
