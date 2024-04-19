
#include "ExperimentSensorInfo.hpp"

#include "DummySensorFactory.hpp"
#include "GPS/Septentrio/SsnxFactory.hpp"
#include "Lidar/Ouster/OusterFactory.hpp"
#include "RGB/AxisCommunications/AxisCommunicationsFactory.hpp"


cExperimentSensorInfo::~cExperimentSensorInfo()
{}

void cExperimentSensorInfo::clear()
{
	mDirty = false;
}

bool cExperimentSensorInfo::isDirty() const
{
	return mDirty;
}



cExperimentSensorInfo_Dummy::cExperimentSensorInfo_Dummy()
{}

cExperimentSensorInfo_Dummy::~cExperimentSensorInfo_Dummy()
{}

const char* cExperimentSensorInfo_Dummy::type() { return dummy_id; }
std::string cExperimentSensorInfo_Dummy::getType() const { return type(); }

void cExperimentSensorInfo_Dummy::load(const nlohmann::json& jdoc)
{}

void cExperimentSensorInfo_Dummy::save(nlohmann::json& jdoc)
{}



cExperimentSensorInfo_Ouster::cExperimentSensorInfo_Ouster()
{}

cExperimentSensorInfo_Ouster::~cExperimentSensorInfo_Ouster()
{}

const char* cExperimentSensorInfo_Ouster::type() { return ouster_id; }
std::string cExperimentSensorInfo_Ouster::getType() const { return type(); }

void cExperimentSensorInfo_Ouster::load(const nlohmann::json& jdoc)
{
}

void cExperimentSensorInfo_Ouster::save(nlohmann::json& jdoc)
{
	nlohmann::json ouster;
	ouster["Manufacturer"] = "OUSTER";
	ouster["Model"] = "OS0-128";
	ouster["Serial Number"] = "992037000167";

	nlohmann::json azimuth_window;
	azimuth_window.push_back(135.0);
	azimuth_window.push_back(225.0);
	ouster["azimuth window"] = azimuth_window;

	ouster["mode"] = "2048x10";

	jdoc[type()] = ouster;

	mDirty = false;
}



cExperimentSensorInfo_Septentrio::cExperimentSensorInfo_Septentrio()
{}

cExperimentSensorInfo_Septentrio::~cExperimentSensorInfo_Septentrio()
{}
	
const char* cExperimentSensorInfo_Septentrio::type() {return ssnx_id; }
std::string cExperimentSensorInfo_Septentrio::getType() const {	return type(); }

void cExperimentSensorInfo_Septentrio::load(const nlohmann::json& jdoc)
{}

void cExperimentSensorInfo_Septentrio::save(nlohmann::json& jdoc)
{
	nlohmann::json ssnx;
	ssnx["Manufacturer"] = "Septentrio";
	ssnx["Model"] = "Altus-NR3";
	ssnx["Serial Number"] = "6106326";
	ssnx["host"] = "172.16.1.3";
	ssnx["port"] = 10000;

	jdoc[type()] = ssnx;

	mDirty = false;
}



cExperimentSensorInfo_AxisCommunications::cExperimentSensorInfo_AxisCommunications()
{}

cExperimentSensorInfo_AxisCommunications::~cExperimentSensorInfo_AxisCommunications()
{}

const char* cExperimentSensorInfo_AxisCommunications::type() { return axis_communications_id; }
std::string cExperimentSensorInfo_AxisCommunications::getType() const { return type(); }

void cExperimentSensorInfo_AxisCommunications::load(const nlohmann::json& jdoc)
{}

void cExperimentSensorInfo_AxisCommunications::save(nlohmann::json& jdoc)
{
	nlohmann::json axis;

	jdoc[type()] = axis;

	mDirty = false;
}



