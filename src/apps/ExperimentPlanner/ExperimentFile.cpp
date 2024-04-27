
#include "ExperimentFile.hpp"
#include "ExperimentSteps.hpp"

#include <QLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QMenu>

#include <string>
#include <fstream>

namespace fs = std::filesystem;


std::shared_ptr<cExperimentSensorInfo> createSensor(std::string type)
{
	if (type == cExperimentSensorInfo_Dummy::type())				return std::make_shared<cExperimentSensorInfo_Dummy>();
	if (type == cExperimentSensorInfo_Ouster::type())				return std::make_shared<cExperimentSensorInfo_Ouster>();
	if (type == cExperimentSensorInfo_Septentrio::type())			return std::make_shared<cExperimentSensorInfo_Septentrio>();
	if (type == cExperimentSensorInfo_AxisCommunications::type())	return std::make_shared<cExperimentSensorInfo_AxisCommunications>();
	if (type == cExperimentSensorInfo_VNIR3000N::type())			return std::make_shared<cExperimentSensorInfo_VNIR3000N>();
	if (type == cExperimentSensorInfo_SWIR384::type())				return std::make_shared<cExperimentSensorInfo_SWIR384>();

	return std::shared_ptr<cExperimentSensorInfo>();
}


cExperimentFile::cExperimentFile()
{
}

cExperimentFile::~cExperimentFile()
{
	clear();
}


const std::string& cExperimentFile::getFileName() const
{
	return mFileName;
}

void cExperimentFile::setFileName(const std::string& filename)
{
	mFileName = filename;
}

const std::string& cExperimentFile::getExperimentName() const
{
	return mExperimentName;
}

void cExperimentFile::setExperimentName(const std::string& name)
{
	mDirty = mExperimentName != name;
	mExperimentName = name;
}

const std::string& cExperimentFile::getLayoutName() const
{
	return mLayoutName;
}

void cExperimentFile::setLayoutName(const std::string& name)
{
	mDirty = mLayoutName != name;
	mLayoutName = name;
}

bool cExperimentFile::isDirty() const
{
	if (mMetaInfo.isDirty())
		return true;

	if (mpController && mpController->isDirty())
		return true;

	for (const auto sensor : mSensors)
	{
		if (sensor->isDirty())
			return true;
	}

	for (const auto step : mSteps)
	{
		if (step->isDirty())
			return true;
	}

	return mDirty;
}

void cExperimentFile::clear()
{
	mFileName.clear();
	mLayoutName.clear();
	mExperimentName.clear();

	mMetaInfo.clear();
	mpController.reset();
	mSensors.clear();

	clearSteps();

	mDirty = false;
}

void cExperimentFile::clearSteps()
{
	for (auto step : mSteps)
	{
		delete step;
	}
	mSteps.clear();
}

void cExperimentFile::open(const std::string& file_name)
{
	if (file_name.empty()) return;

	std::ifstream in;
	in.open(file_name);
	if (!in.is_open())
		return;

	nlohmann::json configDoc;
	try
	{
		in >> configDoc;
	}
	catch (const nlohmann::json::parse_error& e)
	{
		auto msg = e.what();
		return;
	}
	catch (const std::exception& e)
	{
		return;
	}

	mFileName = file_name;

	if (configDoc.contains("experiment name"))
		mExperimentName = configDoc["experiment name"];
	else
		mExperimentName = configDoc["experiment_name"];

	if (configDoc.contains("layout name"))
		mLayoutName = configDoc["layout name"];

	mMetaInfo.load(configDoc);

	std::string controller = configDoc["controller"];

	if (controller == cExperimentCtrlInfo_SpiderCam::type())
	{
		mpController = std::make_unique<cExperimentCtrlInfo_SpiderCam>();
		mpController->load(configDoc[cExperimentCtrlInfo_SpiderCam::type()]);
	}
	else
		mpController = std::make_unique<cExperimentCtrlInfo_Dummy>();

	auto sensors = configDoc["sensors"];

	std::shared_ptr<cExperimentSensorInfo> pSensor;
	for (std::string sensor : sensors)
	{
		pSensor = createSensor(sensor);

		if (!pSensor)
			continue;

		if (configDoc.contains(pSensor->getType()))
			pSensor->load(configDoc[pSensor->getType()]);

		mSensors.push_back(std::move(pSensor));
	}

	if (configDoc.contains("experiment"))
	{
		const auto& steps = configDoc["experiment"];

		for (const auto& entry : steps)
		{
			cExperimentStep* step = nullptr;

			std::string type = entry["type"];

			if (type == "delay")
			{
				step = new cExperimentStep_Delay();
			}
			else if (type == "pause")
			{
				step = new cExperimentStep_Pause();
			}
			else if (type == "movement")
			{
				step = new cExperimentStep_Movement();
			}

			step->load(entry);

			mSteps.push_back(step);
		}
	}
}

void cExperimentFile::save()
{
	if (mFileName.empty()) return;

	nlohmann::json configDoc;

	buildDocument(configDoc);

	std::ofstream out;
	out.open(mFileName, std::ios::trunc);
	if (!out.is_open())
		return;

	out << std::setw(4) << configDoc << std::endl;

	mDirty = false;
}

void cExperimentFile::save_as(const std::string& file_name)
{
	nlohmann::json configDoc;

	buildDocument(configDoc);

	std::ofstream out;
	out.open(file_name);
	if (!out.is_open())
		return;

	out << std::setw(4) << configDoc << std::endl;

	mFileName = file_name;

	mDirty = false;
}

void cExperimentFile::buildDocument(nlohmann::json& configDoc)
{
	configDoc["experiment_name"] = mExperimentName;

	if (!mLayoutName.empty())
		configDoc["layout name"] = mLayoutName;

	mMetaInfo.save(configDoc);

	if (mpController)
	{
		configDoc["controller"] = mpController->getType();
	}
	else
	{
		configDoc["controller"] = "dummy";
	}

	nlohmann::json sensorList;
	if (mSensors.empty())
	{
		sensorList.push_back("dummy");
	}
	else
	{
		for (auto sensor : mSensors)
		{
			sensorList.push_back(sensor->getType());
		}
	}

	configDoc["sensors"] = sensorList;

	if (mpController)
	{
		mpController->save(configDoc);
	}

	for (auto sensor : mSensors)
	{
		sensor->save(configDoc);
	}

	nlohmann::json stepsDoc;

	for (auto step : mSteps)
	{
		stepsDoc.push_back(step->save());
	}

	configDoc["experiment"] = stepsDoc;
}

bool cExperimentFile::empty() const
{
	return mSteps.empty();
}

std::size_t cExperimentFile::size() const
{
	return mSteps.size();
}

const cExperimentMetaInfo& cExperimentFile::getMetaData() const
{
	return mMetaInfo;
}

cExperimentMetaInfo& cExperimentFile::getMetaData()
{
	return mMetaInfo;
}

cExperimentCtrlInfo* const cExperimentFile::getController() const
{
	return mpController.get();
}

void cExperimentFile::setController(std::unique_ptr<cExperimentCtrlInfo> controller)
{
	mDirty |= mpController != controller;
	mpController = std::move(controller);
}

const std::vector<std::shared_ptr<cExperimentSensorInfo>>& cExperimentFile::getSensors() const
{
	return mSensors;
}

void cExperimentFile::setSensors(const std::vector<std::shared_ptr<cExperimentSensorInfo>>& sensors)
{
	mDirty |= mSensors != sensors;
	mSensors = sensors;
}

void cExperimentFile::addSensor(std::shared_ptr<cExperimentSensorInfo> sensor)
{
	mSensors.push_back(sensor);
	mDirty = true;
}

const cExperimentStep& cExperimentFile::front() const { return *(mSteps.front()); }
cExperimentStep& cExperimentFile::front() { return *(mSteps.front()); }


cExperimentFile::iterator cExperimentFile::begin() { return mSteps.begin(); }
cExperimentFile::iterator cExperimentFile::end() { return mSteps.end(); }

cExperimentFile::const_iterator	cExperimentFile::begin() const { return mSteps.cbegin(); }
cExperimentFile::const_iterator	cExperimentFile::end() const { return mSteps.cend(); }

void cExperimentFile::insertBefore(int index, cExperimentStep* step)
{
	if (index <= 0)
	{
		mSteps.push_front(step);
		return;
	}

	if (index >= mSteps.size())
	{
		mSteps.push_back(step);
		return;
	}

	auto it = mSteps.begin();
	std::advance(it, index);

	mSteps.insert(it, step);
}

void cExperimentFile::insertAfter(int index, cExperimentStep* step)
{
	++index;

	if (index <= 0)
	{
		mSteps.push_front(step);
		return;
	}

	if (index >= mSteps.size())
	{
		mSteps.push_back(step);
		return;
	}

	auto it = mSteps.begin();
	std::advance(it, index);

	mSteps.insert(it, step);
}

bool cExperimentFile::removeStep(int index)
{
	if ((index < 0) || (index >= mSteps.size()))
	{
		return false;
	}

	auto it = mSteps.begin();
	std::advance(it, index);

	mSteps.erase(it);

	return true;
}

void cExperimentFile::appendStep(std::unique_ptr<cExperimentStep> step)
{
	if (step)
	{
		mSteps.push_back(step.get());
		step.release();
		mDirty = true;
	}
}

const cExperimentStep& cExperimentFile::operator[](int index) const
{
	if (index < 0)
		return *(mSteps.front());

	if (index >= mSteps.size())
		return *(mSteps.back());

	auto it = mSteps.begin();
	std::advance(it, index);
	return *(*it);
}

cExperimentStep& cExperimentFile::operator[](int index)
{
	if (index < 0)
		return *(mSteps.front());

	if (index >= mSteps.size())
		return *(mSteps.back());

	auto it = mSteps.begin();
	std::advance(it, index);
	return *(*it);
}



