
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"



cExperimentControlModel::cExperimentControlModel(QObject* parent)
:
    cExperimentStateMachine(parent)
{
}

void cExperimentControlModel::handleStateErrorMessage(const QString& msg)
{
    emit errorMessage("Measurement State Error", msg);
}


void cExperimentControlModel::emitStatusMessage(const QString& msg)
{
    emit statusMessage(msg);
}

void cExperimentControlModel::emitMeasurementName(const QString& name)
{
    emit measurementName(name);
}


