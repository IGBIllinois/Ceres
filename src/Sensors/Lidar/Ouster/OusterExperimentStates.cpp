
#include "OusterExperimentStates.hpp"
#include "OusterController.hpp"
#include "Constants.hpp"

#include <thread>

cOusterExperimentState_State::cOusterExperimentState_State(cOusterPropertyPage& propertyPage)
{
}

void cOusterExperimentState_State::configure(const nlohmann::json& stateDoc)
{
}

QString cOusterExperimentState_State::getStatusStr()
{
	QString msg = "Moving to: ";
	return msg;
}

bool cOusterExperimentState_State::recording()
{
	return false;
}

void cOusterExperimentState_State::initialize()
{
}

void cOusterExperimentState_State::run()
{
}

void cOusterExperimentState_State::pause()
{
}

void cOusterExperimentState_State::stop()
{
}

cExperimentState::eRESULT cOusterExperimentState_State::finished()
{
	return eRESULT::DONE;
		
	return eRESULT::WAITING;
}

