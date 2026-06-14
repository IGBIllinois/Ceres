
#include "ExperimentFile.hpp"
#include "MeasurementSteps.hpp"
#include "MeasurementSteps_HySpex.hpp"
#include "MeasurementSteps_Ssnx.hpp"
#include "MeasurementSteps_FLIR.hpp"

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



cExperimentFile::~cExperimentFile()
{
	clear();
}

cExperimentFile& cExperimentFile::operator=(const cExperimentFile& rhs)
{
	mExperimentPath = rhs.mExperimentPath;
	mFileName = rhs.mFileName;

	mDirty = rhs.mDirty;

	mExperimentName = rhs.mExperimentName;
	mExperimentType = rhs.mExperimentType;
	mMeasurementName = rhs.mMeasurementName;

	mMetaInfo = rhs.mMetaInfo;

	mpController = copy(rhs.mpController);

	mSensors = rhs.mSensors;
	mSteps = rhs.mSteps;

	return *this;
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

const std::string& cExperimentFile::getMeasurementName() const
{
	// For backward compatibility, we will return the experiment name if there
	// is no measurement name
	if (mMeasurementName.empty())
		return mExperimentName;

	return mMeasurementName;
}

void cExperimentFile::setMeasurementName(const std::string& name)
{
	mDirty = mMeasurementName != name;
	mMeasurementName = name;
}

const eExperimentType cExperimentFile::getExperimentType() const
{
	return mExperimentType;
}

void cExperimentFile::setExperimentType(eExperimentType type)
{
	mDirty = mExperimentType != type;
	mExperimentType = type;
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
	mExperimentName.clear();
	mMeasurementName.clear();

	mExperimentType = eExperimentType::UNKNOWN;

	mMetaInfo.clear();
	mpController.reset();
	mSensors.clear();

	clearSteps();

	mDirty = false;
}

void cExperimentFile::clearSteps()
{
//	for (auto step : mSteps)
//	{
//		delete step;
//	}
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

	fs::path fn = file_name;
	std::string path = fn.parent_path().string();

	mFileName = file_name;

	if (configDoc.contains("measurement name"))
	{
		mMeasurementName = configDoc["measurement name"];
	}
	else if (configDoc.contains("measurement_name"))
	{
		mMeasurementName = configDoc["measurement_name"];
	}

	if (configDoc.contains("experiment name"))
		mExperimentName = configDoc["experiment name"];
	else
		mExperimentName = configDoc["experiment_name"];

	if (mMeasurementName.empty() && !mExperimentName.empty())
	{
		mMeasurementName = mExperimentName;

		auto pos = mExperimentName.find("_Pass");
		if (pos != std::string::npos)
		{
			mExperimentName.erase(pos);
		}
	}

	if (!mMeasurementName.empty() && mExperimentName.empty())
	{
		auto pos = mMeasurementName.find("_Pass");

		if (pos == std::string::npos)
			mExperimentName = mMeasurementName;
		else
			mExperimentName = mMeasurementName.substr(0, pos);
	}

	if (configDoc.contains("experiment_type"))
	{
		std::string type = configDoc["experiment_type"];
		if (type == "lidar")
			mExperimentType = eExperimentType::LIDAR;
		else if (type == "hyperspectral")
			mExperimentType = eExperimentType::HYPERSPECTRAL;
		else if (type == "time-of-flight")
			mExperimentType = eExperimentType::TOF;
		else if (type == "thermal")
			mExperimentType = eExperimentType::THERMAL;
		else
			mExperimentType = eExperimentType::UNKNOWN;
	}
	else
		mExperimentType = eExperimentType::UNKNOWN;

	if (configDoc.contains("experiment_meta_info"))
	{
		std::string meta_file_name = configDoc["experiment_meta_info"];

		if (!meta_file_name.empty())
		{
			fs::path meta_path = fn;
			meta_path.replace_filename(meta_file_name);

			std::ifstream in;
			in.open(meta_path);
			if (in.is_open())
			{
				nlohmann::json metaDoc;
				try
				{
					in >> metaDoc;
				}
				catch (const nlohmann::json::parse_error& e)
				{
					metaDoc.clear();
				}
				catch (const std::exception& e)
				{
					metaDoc.clear();
				}

				if (!metaDoc.empty())
				{
					mMetaInfo.load(metaDoc);
				}
			}
		}
	}

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
			std::shared_ptr<cMeasurementStep> step;

			if (entry.contains("include"))
			{
				auto inc_step = std::make_shared<cMeasurementStep_Include>();
				inc_step->setDefaultPath(path);

				step = inc_step;

				if (step)
				{
					step->load(entry);
					mSteps.push_back(step);
				}

				continue;
			}

			std::string type = entry["type"];

			step = basic::create_step(type);

			if (!step)
				step = hyspex::create_step(type, entry);

			if (!step)
				step = ssnx::create_step(type, entry);

			if (!step)
				step = flir::create_step(type, entry);

			if (step)
			{
				step->load(entry);
				mSteps.push_back(step);
			}
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

	if (mExperimentType != eExperimentType::UNKNOWN)
	{
		switch (mExperimentType)
		{
		case eExperimentType::LIDAR:
			configDoc["experiment_type"] = "lidar";
			break;
		case eExperimentType::HYPERSPECTRAL:
			configDoc["experiment_type"] = "hyperspectral";
			break;
		case eExperimentType::TOF:
			configDoc["experiment_type"] = "time-of-flight";
			break;
		case eExperimentType::THERMAL:
			configDoc["experiment_type"] = "thermal";
			break;
		}
	}

	if (!mMeasurementName.empty())
		configDoc["measurement_name"] = mMeasurementName;

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

void cExperimentFile::setMetaData(const cExperimentMetaInfo& meta_info)
{
	mDirty |= mMetaInfo != meta_info;
	mMetaInfo = meta_info;
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

void cExperimentFile::clearSensors()
{
	mSensors.clear();
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

const cMeasurementStep& cExperimentFile::front() const { return *(mSteps.front()); }
cMeasurementStep& cExperimentFile::front() { return *(mSteps.front()); }


cExperimentFile::iterator cExperimentFile::begin() { return mSteps.begin(); }
cExperimentFile::iterator cExperimentFile::end() { return mSteps.end(); }

cExperimentFile::const_iterator	cExperimentFile::begin() const { return mSteps.cbegin(); }
cExperimentFile::const_iterator	cExperimentFile::end() const { return mSteps.cend(); }

void cExperimentFile::insertBefore(int index, std::unique_ptr<cMeasurementStep> step)
{
	if (index <= 0)
	{
		mSteps.push_front(std::move(step));
		return;
	}

	if (index >= mSteps.size())
	{
		mSteps.push_back(std::move(step));
		return;
	}

	auto it = mSteps.begin();
	std::advance(it, index);

	mSteps.insert(it, std::move(step));
}

void cExperimentFile::insertAfter(int index, std::unique_ptr<cMeasurementStep> step)
{
	++index;

	if (index <= 0)
	{
		mSteps.push_front(std::move(step));
		return;
	}

	if (index >= mSteps.size())
	{
		mSteps.push_back(std::move(step));
		return;
	}

	auto it = mSteps.begin();
	std::advance(it, index);

	mSteps.insert(it, std::move(step));
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

void cExperimentFile::appendStep(std::unique_ptr<cMeasurementStep> step)
{
	if (step)
	{
		std::shared_ptr<cMeasurementStep> shared = std::move(step);
		mSteps.push_back(shared);
//		step.release();
		mDirty = true;
	}
}

const cMeasurementStep& cExperimentFile::operator[](int index) const
{
	if (index < 0)
		return *(mSteps.front());

	if (index >= mSteps.size())
		return *(mSteps.back());

	auto it = mSteps.begin();
	std::advance(it, index);
	return *(*it);
}

cMeasurementStep& cExperimentFile::operator[](int index)
{
	if (index < 0)
		return *(mSteps.front());

	if (index >= mSteps.size())
		return *(mSteps.back());

	auto it = mSteps.begin();
	std::advance(it, index);
	return *(*it);
}



