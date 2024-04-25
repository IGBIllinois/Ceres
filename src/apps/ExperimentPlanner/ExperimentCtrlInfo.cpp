
#include "ExperimentCtrlInfo.hpp"

#include "Spidercam/SpidercamCtrlFactory.hpp"
#include "DummyCtrlFactory.hpp"

cExperimentCtrlInfo::~cExperimentCtrlInfo()
{}

void cExperimentCtrlInfo::clear()
{
	mDirty = false;
}

bool cExperimentCtrlInfo::isDirty() const
{
	return mDirty;
}



cExperimentCtrlInfo_Dummy::cExperimentCtrlInfo_Dummy()
{}

cExperimentCtrlInfo_Dummy::~cExperimentCtrlInfo_Dummy()
{}

const char* cExperimentCtrlInfo_Dummy::type() { return dummy_id; }
std::string cExperimentCtrlInfo_Dummy::getType() const { return type(); }

void cExperimentCtrlInfo_Dummy::load(const nlohmann::json& jdoc)
{}

void cExperimentCtrlInfo_Dummy::save(nlohmann::json& jdoc)
{}


cExperimentCtrlInfo_SpiderCam::cExperimentCtrlInfo_SpiderCam()
{}

cExperimentCtrlInfo_SpiderCam::~cExperimentCtrlInfo_SpiderCam()
{}

const char* cExperimentCtrlInfo_SpiderCam::type() { return spidercam_id; }
std::string cExperimentCtrlInfo_SpiderCam::getType() const { return type(); }

void cExperimentCtrlInfo_SpiderCam::clear()
{}

int cExperimentCtrlInfo_SpiderCam::getUpdateInterval_ms() const
{
	return mUpdateInterval_ms;
}

float cExperimentCtrlInfo_SpiderCam::getPositionTolerance_cm() const
{
	return mPositionTolerance_cm;
}

void cExperimentCtrlInfo_SpiderCam::setUpdateInterval_ms(int updateInterval_ms)
{
	mDirty |= mUpdateInterval_ms != updateInterval_ms;
	mUpdateInterval_ms = updateInterval_ms;
}

void cExperimentCtrlInfo_SpiderCam::setPositionTolerance_cm(float positionTolerance_cm)
{
	mDirty |= mPositionTolerance_cm != positionTolerance_cm;
	mPositionTolerance_cm = positionTolerance_cm;
}

void cExperimentCtrlInfo_SpiderCam::load(const nlohmann::json& jdoc)
{
	if (jdoc.contains("update interval (ms)"))
		mUpdateInterval_ms = jdoc["update interval (ms)"];

	if (jdoc.contains("position tolerance (cm)"))
		mPositionTolerance_cm = jdoc["position tolerance (cm)"];
}

void cExperimentCtrlInfo_SpiderCam::save(nlohmann::json& jdoc)
{
	nlohmann::json spidercam;

	spidercam["update interval (ms)"] = 250;
	spidercam["position tolerance (cm)"] = 1.0;

	jdoc[type()] = spidercam;

	mDirty = false;
}



