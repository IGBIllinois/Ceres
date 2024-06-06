
#include "ExperimentCtrlInfo.hpp"

#include "Spidercam/SpidercamCtrlFactory.hpp"
#include "DummyCtrlFactory.hpp"


std::unique_ptr<cExperimentCtrlInfo> copy(const std::unique_ptr<cExperimentCtrlInfo>& rhs)
{
	if (!rhs)
		return std::unique_ptr<cExperimentCtrlInfo>();

	auto type = rhs->getType();
	
	if (type == cExperimentCtrlInfo_SpiderCam::type())
	{
		return std::make_unique<cExperimentCtrlInfo_SpiderCam>(static_cast<cExperimentCtrlInfo_SpiderCam&>(*rhs.get()));
	}

	return std::make_unique<cExperimentCtrlInfo_Dummy>(static_cast<cExperimentCtrlInfo_Dummy&>(*rhs.get()));
}

std::unique_ptr<cExperimentCtrlInfo> copy(const cExperimentCtrlInfo* rhs)
{
	if (!rhs)
		return std::unique_ptr<cExperimentCtrlInfo>();

	auto type = rhs->getType();

	if (type == cExperimentCtrlInfo_SpiderCam::type())
	{
		return std::make_unique<cExperimentCtrlInfo_SpiderCam>(*(static_cast<const cExperimentCtrlInfo_SpiderCam*>(rhs)));
	}

	return std::make_unique<cExperimentCtrlInfo_Dummy>(*(static_cast<const cExperimentCtrlInfo_Dummy*>(rhs)));
}


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

cExperimentCtrlInfo_Dummy::cExperimentCtrlInfo_Dummy(const cExperimentCtrlInfo_Dummy& rhs)
{
	mDirty = rhs.mDirty;
}

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

cExperimentCtrlInfo_SpiderCam::cExperimentCtrlInfo_SpiderCam(const cExperimentCtrlInfo_SpiderCam& rhs)
{
	mDirty = rhs.mDirty;
	mUpdateInterval_ms = rhs.mUpdateInterval_ms;
	mPositionTolerance_cm = rhs.mPositionTolerance_cm;
}

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



