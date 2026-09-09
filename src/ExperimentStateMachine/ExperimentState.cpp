
#include "ExperimentState.hpp"


cExperimentState::cExperimentState(QObject* parent) : QObject(parent)
{}

cExperimentState::~cExperimentState()
{}

void cExperimentState::attachVariableTable(std::weak_ptr<cExperimentVariableTable> vars)
{
	mVariables = vars;
}


bool cExperimentState::needsDataFile()
{
	return recording(); 
}
