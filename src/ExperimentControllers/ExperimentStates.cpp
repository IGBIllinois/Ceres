
#include "ExperimentStates.hpp"
#include "../Utilities/Constants.hpp"


cExperimentState_Delay::cExperimentState_Delay()
	: mElapsedTime_sec(0), mWaitTime_sec(0)
{
}


void cExperimentState_Delay::configure(const nlohmann::json& stateDoc)
{
	mWaitTime_sec = stateDoc["wait (sec)"];
}

QString cExperimentState_Delay::getStatusStr()
{
	QString msg = "Delaying for ";
	msg.append(std::to_string(mWaitTime_sec).c_str());
	msg += " sec.";
	return msg;
}

bool cExperimentState_Delay::recording()
{
	return false;
}

void cExperimentState_Delay::initialize()
{
	mStart = std::chrono::steady_clock::now();
}

void cExperimentState_Delay::run()
{
	auto diff = std::chrono::steady_clock::now() - mStart;
	mElapsedTime_sec = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
}

bool cExperimentState_Delay::finished()
{
	return mElapsedTime_sec >= mWaitTime_sec;
}

