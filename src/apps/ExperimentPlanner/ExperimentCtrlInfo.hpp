
#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>


class cExperimentCtrlInfo
{
public:
	cExperimentCtrlInfo() = default;
	virtual ~cExperimentCtrlInfo();

	virtual void clear();

	bool isDirty() const;

	virtual std::string getType() const = 0;

protected:
	virtual void load(const nlohmann::json& jdoc) = 0;
	virtual void save(nlohmann::json& jdoc) = 0;

protected:
	bool mDirty = false;

	friend class cExperimentFile;
};


class cExperimentCtrlInfo_Dummy : public cExperimentCtrlInfo
{
public:
	cExperimentCtrlInfo_Dummy();
	virtual ~cExperimentCtrlInfo_Dummy();

	static const char* type();

	std::string getType() const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;
};


class cExperimentCtrlInfo_SpiderCam : public cExperimentCtrlInfo
{
public:
	cExperimentCtrlInfo_SpiderCam();
	virtual ~cExperimentCtrlInfo_SpiderCam();

	static const char* type();

	std::string getType() const override;

	void clear() override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;
};



