
#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>


class cExperimentSensorInfo
{
public:
	cExperimentSensorInfo() = default;
	virtual ~cExperimentSensorInfo();

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


class cExperimentSensorInfo_Dummy : public cExperimentSensorInfo
{
public:
	cExperimentSensorInfo_Dummy();
	virtual ~cExperimentSensorInfo_Dummy();

	static const char* type();

	std::string getType() const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;
};


class cExperimentSensorInfo_Ouster : public cExperimentSensorInfo
{
public:
	cExperimentSensorInfo_Ouster();
	virtual ~cExperimentSensorInfo_Ouster();

	static const char* type();

	std::string getType() const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

private:
/*
	"Manufacturer": "OUSTER",
	"Model" : "OS0-128",
	"Serial Number" : "992037000167",
	"azimuth window" : [135.0, 225.0] ,
	"mode" : "2048x10"
*/
};


class cExperimentSensorInfo_Septentrio : public cExperimentSensorInfo
{
public:
	cExperimentSensorInfo_Septentrio();
	virtual ~cExperimentSensorInfo_Septentrio();

	static const char* type();

	std::string getType() const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

private:
/*
	"Manufacturer": "Septentrio",
	"Model": "Altus-NR3",
	"Serial Number": "6106326"
*/
};


class cExperimentSensorInfo_AxisCommunications : public cExperimentSensorInfo
{
public:
	cExperimentSensorInfo_AxisCommunications();
	virtual ~cExperimentSensorInfo_AxisCommunications();

	static const char* type();

	std::string getType() const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

	int mDefaultCameraId = -1;
	int mDefaultImageWidth = 1;
	int mDefaultImageHeight = 1;
	int mDefaultFrameRate_fps = -1;
};



