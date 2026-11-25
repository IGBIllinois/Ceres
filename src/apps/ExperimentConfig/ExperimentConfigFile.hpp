
#pragma once

#include "ExperimentInfo.hpp"

#include <vector>
#include <string>
#include <memory>
#include <set>


class cExperimentConfigFile
{
public:
//	typedef std::vector<cPlotConfigScan> PlotScan_t;

//	typedef PlotScan_t::iterator				iterator;
//	typedef PlotScan_t::const_iterator			const_iterator;

public:
	cExperimentConfigFile();
	~cExperimentConfigFile();

	const std::string& getFileName() const;
	const std::string& getTemporaryFileName() const;

	bool isDirty() const;

	void clear();

	bool open(const std::string& file_name);

	void save();

	void save_as(const std::string& file_name);

	bool open_temporary_file(const std::string& file_name);
	void save_temporary_file();

	bool empty() const;
	std::size_t size() const;

	const cExperimentInfo& getMetaData() const;
	cExperimentInfo& getMetaData();
	void setMetaData(const cExperimentInfo& meta_info);

private:
	void buildDocument(nlohmann::json& jdoc);
	void clearDirtyFlag();

private:
	std::string mFileName;
	std::string mTmpFileName;

	cExperimentInfo mMetaInfo;

	bool mIsDirty = false;

};

