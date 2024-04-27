
#pragma once

#include "ExperimentMetaInfo.hpp"
#include "ExperimentCtrlInfo.hpp"
#include "ExperimentSensorInfo.hpp"

#include <nlohmann/json.hpp>

#include <QtWidgets>
#include <QDialog>

#include <filesystem>
#include <vector>
#include <list>
#include <string>
#include <memory>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
QT_END_NAMESPACE

// Forward Declarations
class cExperimentStep;

std::shared_ptr<cExperimentSensorInfo> createSensor(std::string type);

enum eExperimentStep {delay, pause, movement};


class cExperimentFile
{
public:
	typedef std::list<cExperimentStep*> Experiment_t;

	typedef Experiment_t::iterator			iterator;
	typedef Experiment_t::const_iterator	const_iterator;

public:
	cExperimentFile();
	~cExperimentFile();

	const std::string& getFileName() const;
	void setFileName(const std::string& filename);

	const std::string& getExperimentName() const;
	void setExperimentName(const std::string& name);

	const std::string& getLayoutName() const;
	void setLayoutName(const std::string& name);

	bool isDirty() const;

	void clear();
	void clearSteps();

	void open(const std::string& file_name);

	void save();

	void save_as(const std::string& file_name);

	bool empty() const;
	std::size_t size() const;

	const cExperimentMetaInfo& getMetaData() const;
	cExperimentMetaInfo& getMetaData();

	cExperimentCtrlInfo* const getController() const;
	void setController(std::unique_ptr<cExperimentCtrlInfo> controller);

	const std::vector<std::shared_ptr<cExperimentSensorInfo>>& getSensors() const;
	void setSensors(const std::vector<std::shared_ptr<cExperimentSensorInfo>>& sensors);
	void addSensor(std::shared_ptr<cExperimentSensorInfo> sensor);

	const cExperimentStep& front() const;
	cExperimentStep& front();

	iterator  begin();
	iterator  end();

	const_iterator	begin() const;
	const_iterator	end() const;

	void insertBefore(int index, cExperimentStep* step);
	void insertAfter(int index, cExperimentStep* step);

	bool removeStep(int index);

	void appendStep(std::unique_ptr<cExperimentStep> step);

	const cExperimentStep& operator[](int index) const;
	cExperimentStep& operator[](int index);

private:
	void buildDocument(nlohmann::json& jdoc);

private:
	std::filesystem::path	mExperimentPath;
	std::string	mFileName;

	bool mDirty = false;

	std::string	mExperimentName;
	std::string	mLayoutName;

	cExperimentMetaInfo mMetaInfo;

	std::unique_ptr<cExperimentCtrlInfo> mpController;

	std::vector<std::shared_ptr<cExperimentSensorInfo>> mSensors;

	std::list<cExperimentStep*> mSteps;
};


