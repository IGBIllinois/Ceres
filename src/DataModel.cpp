
#include "DataModel.hpp"
#include "SensorModel.hpp"


cDataModel::cDataModel()
:
    QObject()
{
}

cDataModel::~cDataModel()
{
}

void cDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
        mActiveSensors.push_back(pSensor);
}

void cDataModel::startDataCollection()
{
    mFile.open("test.data");

    for (auto& sensor : mActiveSensors)
    {
        sensor->writeDataHeader(mFile);
    }

    for (auto& sensor : mActiveSensors)
    {
        sensor->startDataRecording(mFile);
    }
}

void cDataModel::stopDataCollection()
{
    for (auto& sensor : mActiveSensors)
    {
        sensor->stopDataRecording();
    }

    mFile.close();
}
