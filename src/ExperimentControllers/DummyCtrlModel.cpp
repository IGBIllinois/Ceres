
#include "DummyCtrlModel.hpp"


cDummyControlModel::cDummyControlModel(QObject* parent)
:
    cExperimentControlModel(parent)
{
}

cDummyControlModel::~cDummyControlModel()
{

}

bool cDummyControlModel::startCommunications()
{
    return true;
}

void cDummyControlModel::stopCommunications()
{
}

void cDummyControlModel::configure(const nlohmann::json& jsonCfg)
{
}

void cDummyControlModel::startExperiment()
{
    cExperimentControlModel::startExperiment();
}


void cDummyControlModel::writeDataHeader(cBlockDataFile& file)
{
}

void cDummyControlModel::stopDataRecording()
{
}


void cDummyControlModel::update()
{
    updateExperimentStateMachine();
}


