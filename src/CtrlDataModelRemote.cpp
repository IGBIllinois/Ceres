
#include "CtrlDataModelRemote.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"


cCtrlDataModelRemote::cCtrlDataModelRemote(QObject* parent)
:
    cCtrlDataModel(parent)
{
}

cCtrlDataModelRemote::~cCtrlDataModelRemote()
{
    stopDataThread();
}

bool cCtrlDataModelRemote::openDataFile(const QString& defaultPath)
{
    return false;
}

void cCtrlDataModelRemote::closeDataFile()
{
}

void cCtrlDataModelRemote::startExperiment()
{
}



