
#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>


class cExperimentMetaInfo
{
public:
	cExperimentMetaInfo() = default;
	~cExperimentMetaInfo();

	void clear();

	bool isDirty() const;

	const std::string& getPrincipalInvestigator() const;
	const std::vector<std::string>& getResearchers() const;
	const std::vector<std::string>& getComments() const;

	const std::string& getSpecies() const;
	const std::string& getCultivar() const;
	const std::vector<std::string>& getEvents() const;
	const std::string& getConstructName() const;
	const std::vector<std::string>& getTreatments() const;

	const std::string& getFieldDesign() const;

	std::string getPlantingDate_ymd() const;
	const std::string& getPlantingDay() const;
	const std::string& getPlantingMonth() const;
	const std::string& getPlantingYear() const;

	std::string getTargetHarvestDate_ymd() const;
	const std::string& getTargetHarvestDay() const;
	const std::string& getTargetHarvestMonth() const;
	const std::string& getTargetHarvestYear() const;

	const std::string& getPermitInfo() const;



	void setSavePlotsInSingleFile(bool option);
	void setSaveFrameIds(bool option);
	void setSavePixelInfo(bool option);

	void setSavePlyFiles(bool option);
	void setPlysUseBinaryFormat(bool option);

protected:
	void load(const nlohmann::json& jdoc);
	void save(nlohmann::json& jdoc);

private:
	bool mDirty = false;

	std::string mPrincipalInvestigator;
	std::vector<std::string> mResearchers;
	std::vector<std::string> mComments;

	std::string mSpecies;
	std::string mCultivar;
	std::vector<std::string> mEvents;
	std::string mConstructName;
	std::vector<std::string> mTreatments;

	std::string mFieldDesign;

	std::string mPlantingDay;
	std::string mPlantingMonth;
	std::string mPlantingYear;

	std::string mTargetHarvestDay;
	std::string mTargetHarvestMonth;
	std::string mTargetHarvestYear;

	std::string mPermitInfo;

	friend class cExperimentFile;
};