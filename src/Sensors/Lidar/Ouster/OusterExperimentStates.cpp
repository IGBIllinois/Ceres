
#include "OusterExperimentStates.hpp"


/*******************************************************************/
/**           Base Class for Ouster Experiment States             **/
/*******************************************************************/

cOuster_Properties_Remote::cOuster_Properties_Remote()
	: cExperimentStateRemoteInterface()
{}

cOuster_Properties_Remote::~cOuster_Properties_Remote()
{}

bool cOuster_Properties_Remote::recording()
{
	return false;
}

bool cOuster_Properties_Remote::configure(const nlohmann::json& stateDoc)
{
	return true;
}

void cOuster_Properties_Remote::cleanup()
{
	closeConnection();
	destroy();
}

bool cOuster_Properties_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return false;

	return openConnection();
}

void cOuster_Properties_Remote::run()
{}

void cOuster_Properties_Remote::pause()
{}

void cOuster_Properties_Remote::stop()
{}

cExperimentState::eRESULT cOuster_Properties_Remote::finished()
{
	return eRESULT::DONE;

/*
	if (mState == eSTATE::ERROR)
		return eRESULT::ABORT;

	if (mState == eSTATE::COMPLETE)
		return eRESULT::DONE;

	return eRESULT::WAITING;
*/
}


