
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

void cExperimentCtrlInfo_SpiderCam::load(const nlohmann::json& jdoc)
{}

void cExperimentCtrlInfo_SpiderCam::save(nlohmann::json& jdoc)
{
	nlohmann::json spidercam;

	spidercam["c2_ip"] = "172.16.144.144";
	spidercam["c3_ip"] = "172.16.1.3";
	spidercam["port"] = 10203;
	spidercam["min X position (m)"] = 12.4;
	spidercam["max X position (m)"] = 178.0;
	spidercam["min Y position (m)"] = 12.3;
	spidercam["max Y position (m)"] = 177.8;
	spidercam["min height (m)"] = 1.0;
	spidercam["max height (m)"] = 9.0;
	spidercam["max speed (m/s)"] = 2.0;
	spidercam["update interval (ms)"] = 250;
	spidercam["position tolerance (cm)"] = 1.0;

	jdoc[type()] = spidercam;

	mDirty = false;
}



