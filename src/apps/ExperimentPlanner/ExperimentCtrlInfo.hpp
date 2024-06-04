
#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <memory>

class cExperimentCtrlInfo;
std::unique_ptr<cExperimentCtrlInfo> copy(const std::unique_ptr<cExperimentCtrlInfo>& rhs);

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

	cExperimentCtrlInfo_Dummy(const cExperimentCtrlInfo_Dummy& rhs);

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

	cExperimentCtrlInfo_SpiderCam(const cExperimentCtrlInfo_SpiderCam& rhs);

	static const char* type();

	std::string getType() const override;

	void clear() override;

	int getUpdateInterval_ms() const;
	float getPositionTolerance_cm() const;

	void setUpdateInterval_ms(int updateInterval_ms);
	void setPositionTolerance_cm(float positionTolerance_cm);

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

private:
	int mUpdateInterval_ms = 250;
	float mPositionTolerance_cm = 1.0;
};



