
#pragma once

#include "ExperimentMetaInfo.hpp"
#include "ExperimentCtrlInfo.hpp"
#include "ExperimentSensorInfo.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <vector>
#include <list>
#include <string>
#include <memory>

// Forward Declarations
class cExperimentStep;


enum eMeasurementStep {delay, pause, movement, hyspex_command, reference_point};


class cExperimentFile
{
public:
	enum class eExperimentType { UNKNOWN, LIDAR, HYPERSPECTRAL, TOF, };

	typedef std::list<std::shared_ptr<cExperimentStep>> Experiment_t;

	typedef Experiment_t::iterator			iterator;
	typedef Experiment_t::const_iterator	const_iterator;

public:
	cExperimentFile() = default;
	~cExperimentFile();

	cExperimentFile& operator=(const cExperimentFile& rhs);

	const std::string& getFileName() const;
	void setFileName(const std::string& filename);

	const std::string& getExperimentName() const;
	void setExperimentName(const std::string& name);

	const std::string& getMeasurementName() const;
	void setMeasurementName(const std::string& name);

	const eExperimentType getExperimentType() const;
	void setExperimentType(eExperimentType type);

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
	void setMetaData(const cExperimentMetaInfo& meta_info);

	cExperimentCtrlInfo* const getController() const;
	void setController(std::unique_ptr<cExperimentCtrlInfo> controller);

	void clearSensors();
	const std::vector<std::shared_ptr<cExperimentSensorInfo>>& getSensors() const;
	void setSensors(const std::vector<std::shared_ptr<cExperimentSensorInfo>>& sensors);
	void addSensor(std::shared_ptr<cExperimentSensorInfo> sensor);

	const cExperimentStep& front() const;
	cExperimentStep& front();

	iterator  begin();
	iterator  end();

	const_iterator	begin() const;
	const_iterator	end() const;

	void insertBefore(int index, std::unique_ptr<cExperimentStep> step);
	void insertAfter(int index, std::unique_ptr<cExperimentStep> step);

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
	std::string	mMeasurementName;
	std::string	mLayoutName;

	eExperimentType mExperimentType = eExperimentType::UNKNOWN;

	cExperimentMetaInfo mMetaInfo;

	std::unique_ptr<cExperimentCtrlInfo> mpController;

	std::vector<std::shared_ptr<cExperimentSensorInfo>> mSensors;

	Experiment_t mSteps;
};


