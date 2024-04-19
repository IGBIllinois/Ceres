
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
class cExerimentStep;


class cExperimentFile
{
public:
	typedef std::list<cExerimentStep*> Experiment_t;

	typedef Experiment_t::iterator			iterator;
	typedef Experiment_t::const_iterator	const_iterator;

public:
	cExperimentFile();
	~cExperimentFile();

	const std::string& getFileName() const;
	void setFileName(const std::string& filename);

	const std::string& getLayoutName() const;
	void setLayoutName(const std::string& filename);

	bool isDirty() const;

	void clear();
	void clearSteps();

	void open(const std::string& file_name);

	void save();

	void save_as(const std::string& file_name);

	bool empty() const;
	std::size_t size() const;

	cExperimentCtrlInfo* const getController() const;
	void setController(cExperimentCtrlInfo* controller);

	const std::vector<cExperimentSensorInfo*>& getSensors() const;
	void addSensor(cExperimentSensorInfo* sensor);

	//	bool contains(const std::string& name);

	const cExerimentStep& front() const;
	cExerimentStep& front();

	iterator		begin();
	iterator		end();

	const_iterator	begin() const;
	const_iterator	end() const;

	void insertBefore(int index, cExerimentStep* step);
	void insertAfter(int index, cExerimentStep* step);

//	const_iterator	find(const std::string& name) const;
//	iterator		find(const std::string& name);

//	cPlotConfigScan& add(const std::string& name);
//	void remove(const std::string& name);

	const cExerimentStep& operator[](int index) const;
	cExerimentStep& operator[](int index);

private:
	void buildDocument(nlohmann::json& jdoc);

private:
	std::filesystem::path	mExperimentPath;
	std::string	mFileName;

	std::string	mLayoutName;

	bool mDirty = false;

	cExperimentMetaInfo mMetaInfo;

	std::unique_ptr<cExperimentCtrlInfo> mpController;

	std::vector<cExperimentSensorInfo*> mSensors;

	std::list<cExerimentStep*> mSteps;
};


