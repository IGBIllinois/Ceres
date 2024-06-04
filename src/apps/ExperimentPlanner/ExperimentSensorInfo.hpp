
#pragma once

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QString>

#include <string>
#include <vector>
#include <memory>


class cExperimentSensorInfo;

std::shared_ptr<cExperimentSensorInfo> createSensor(std::string type);


class cSensorWidget : public QWidget
{
public:
	virtual void accept() = 0;
	virtual void reset() = 0;
};

class cExperimentSensorInfo
{
public:
	cExperimentSensorInfo() = default;
	virtual ~cExperimentSensorInfo();

	virtual void clear();

	bool isDirty() const;

	virtual std::string getType() const = 0;

	virtual QString getName() const = 0;
	virtual cSensorWidget* widget() = 0;

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

	QString getName() const override;
	cSensorWidget* widget() override;

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

	const std::string& getManufacturer() const;
	const std::string& getModel() const;
	const std::string& getSerialNumber() const;

	const std::string& getMode() const;
	void setMode(const std::string& mode);

	QString getName() const override;
	cSensorWidget* widget() override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

private:
	std::string mManufacturer;
	std::string mModel;
	std::string mSerialNumber;
	std::string mMode;

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

	const std::string& getManufacturer() const;
	const std::string& getModel() const;
	const std::string& getSerialNumber() const;

	QString getName() const override;
	cSensorWidget* widget() override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

private:
	std::string mManufacturer;
	std::string mModel;
	std::string mSerialNumber;
};


class cExperimentSensorInfo_AxisCommunications : public cExperimentSensorInfo
{
public:
	cExperimentSensorInfo_AxisCommunications();
	virtual ~cExperimentSensorInfo_AxisCommunications();

	static const char* type();

	std::string getType() const override;

	const std::string& getManufacturer() const;
	const std::string& getModel() const;
	const std::string& getSerialNumber() const;

	int getCameraId() const;
	int getImageWidth() const;
	int getImageHeight() const;
	int getFrameRate_fps() const;

	void setCameraId(int id);
	void setImageSize(int width, int height);
	void setFrameRate_fps(int fps);

	QString getName() const override;
	cSensorWidget* widget() override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

	std::string mManufacturer;
	std::string mModel;
	std::string mSerialNumber;

	int mCameraId = 1;
	int mImageWidth = 640;
	int mImageHeight = 480;
	int mFrameRate_fps = 1;
};


class cExperimentSensorInfo_VNIR3000N : public cExperimentSensorInfo
{
public:
	cExperimentSensorInfo_VNIR3000N();
	virtual ~cExperimentSensorInfo_VNIR3000N();

	static const char* type();

	std::string getType() const override;

	const std::string& getManufacturer() const;
	const std::string& getModel() const;
	const std::string& getSerialNumber() const;

	QString getName() const override;
	cSensorWidget* widget() override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

private:
	std::string mManufacturer;
	std::string mModel;
	std::string mSerialNumber;
};


class cExperimentSensorInfo_SWIR384 : public cExperimentSensorInfo
{
public:
	cExperimentSensorInfo_SWIR384();
	virtual ~cExperimentSensorInfo_SWIR384();

	static const char* type();

	std::string getType() const override;

	const std::string& getManufacturer() const;
	const std::string& getModel() const;
	const std::string& getSerialNumber() const;

	QString getName() const override;
	cSensorWidget* widget() override;

protected:
	void load(const nlohmann::json& jdoc) override;
	void save(nlohmann::json& jdoc) override;

private:
	std::string mManufacturer;
	std::string mModel;
	std::string mSerialNumber;
};



