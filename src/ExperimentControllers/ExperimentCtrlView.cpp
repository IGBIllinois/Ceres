
#include "ExperimentCtrlView.hpp"


void cExperimentControlView::configure(const nlohmann::json& jsonCfg)
{
}

void cExperimentControlView::experimentStateChanging(experiment::eState state)
{}

void cExperimentControlView::experimentStatusUpdating(QString msg)
{
	emit statusMessage(msg);
}

void cExperimentControlView::experimentNameUpdating(QString name)
{
}
