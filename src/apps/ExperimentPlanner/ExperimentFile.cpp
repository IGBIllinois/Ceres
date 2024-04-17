
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

bool cExperimentFile::isDirty() const
{
	for (const auto step : mSteps)
	{
		if (step->isDirty())
			return true;
	}

	return false;
}

void cExperimentFile::clear()
{
	mFileName.clear();

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

//	mOptions.load(configDoc);

	if (configDoc.contains("experiment"))
	{
		const auto& steps = configDoc["experiment"];

		for (const auto& entry : steps)
		{
			cExerimentStep* step = nullptr;

			std::string type = entry["type"];

			if (type == "delay")
			{
				step = new cExerimentStep_Delay();
			}
			else if (type == "pause")
			{
				step = new cExerimentStep_Pause();
			}
			else if (type == "movement")
			{
				step = new cExerimentStep_Movement();
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

/*
	configDoc["options"] = mOptions.save();
*/

	nlohmann::json stepsDoc;

	for (auto step : mSteps)
	{
		stepsDoc.push_back(step->save());
	}

	if (!stepsDoc.is_null())
		configDoc["experiment"] = stepsDoc;

	std::ofstream out;
	out.open(mFileName, std::ios::trunc);
	if (!out.is_open())
		return;

	out << std::setw(4) << configDoc << std::endl;
}

void cExperimentFile::save_as(const std::string& file_name)
{
	nlohmann::json configDoc;

//	configDoc["options"] = mOptions.save();

	nlohmann::json stepsDoc;

	for (auto step : mSteps)
	{
		stepsDoc.push_back(step->save());
	}

	configDoc["experiment"] = stepsDoc;

	std::ofstream out;
	out.open(file_name);
	if (!out.is_open())
		return;

	out << std::setw(4) << configDoc << std::endl;

	mFileName = file_name;
}

bool cExperimentFile::empty() const
{
	return mSteps.empty();
}

std::size_t cExperimentFile::size() const
{
	return mSteps.size();
}

/*
bool cExperimentFile::contains(const std::string& name)
{
	for (const auto& scan : mScans)
	{
		if (scan.getExperimentName() == name)
			return true;
	}

	return false;
}
*/

const cExerimentStep& cExperimentFile::front() const { return *(mSteps.front()); }
cExerimentStep& cExperimentFile::front() { return *(mSteps.front()); }


cExperimentFile::iterator cExperimentFile::begin() { return mSteps.begin(); }
cExperimentFile::iterator cExperimentFile::end() { return mSteps.end(); }

cExperimentFile::const_iterator	cExperimentFile::begin() const { return mSteps.cbegin(); }
cExperimentFile::const_iterator	cExperimentFile::end() const { return mSteps.cend(); }

/*
cExperimentFile::const_iterator cExperimentFile::find(const std::string& name) const
{
	for (auto it = mScans.cbegin(); it != mScans.cend(); ++it)
	{
		if (it->getExperimentName() == name)
			return it;
	}

	return mScans.cend();
}
*/

/*
cExperimentFile::iterator cExperimentFile::find(const std::string& name)
{
	for (auto it = mScans.begin(); it != mScans.end(); ++it)
	{
		if (it->getExperimentName() == name)
			return it;
	}

	return mScans.end();
}
*/

/*
cPlotConfigScan& cExperimentFile::add(const std::string& name)
{
	for (auto& scan : mScans)
	{
		if (scan.getExperimentName() == name)
			return scan;
	}

	cPlotConfigScan scan;

	scan.setExperimentName(name);

	mScans.push_back(std::move(scan));

	return mScans.back();
}
*/

/*
void cExperimentFile::remove(const std::string& name)
{

}
*/

const cExerimentStep& cExperimentFile::operator[](int index) const
{
	if (index < 0)
		return *(mSteps.front());

	if (index >= mSteps.size())
		return *(mSteps.back());

	auto it = mSteps.begin();
	std::advance(it, index);
	return *(*it);
}

cExerimentStep& cExperimentFile::operator[](int index)
{
	if (index < 0)
		return *(mSteps.front());

	if (index >= mSteps.size())
		return *(mSteps.back());

	auto it = mSteps.begin();
	std::advance(it, index);
	return *(*it);
}



