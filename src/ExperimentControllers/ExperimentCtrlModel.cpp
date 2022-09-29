
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"



cExperimentControlModel::cExperimentControlModel(QObject* parent)
:
    cExperimentStateMachine(parent)
{
}

void cExperimentControlModel::emitStatusMessage(const QString& msg)
{
    emit statusMessage(msg);
}


