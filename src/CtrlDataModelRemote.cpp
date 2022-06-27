
#include "DataModelRemote.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"


cDataModelRemote::cDataModelRemote(QObject* parent)
:
    cDataModel(parent)
{
}

cDataModelRemote::~cDataModelRemote()
{
    stopDataThread();
}

bool cDataModelRemote::openDataFile(const QString& defaultPath)
{
    return false;
}

void cDataModelRemote::closeDataFile()
{
}

void cDataModelRemote::startExperiment()
{
}



