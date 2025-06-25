
#include "ExperimentSensorInfo.hpp"

#include "DummySensorIDs.hpp"
#include "GPS/Septentrio/SsnxIDs.hpp"
#include "Lidar/Ouster/OusterIDs.hpp"
#include "RGB/AxisCommunications/AxisCommunicationsIDs.hpp"
#include "Hyperspectral/HySpex/HySpexIDs.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QIntValidator>


std::shared_ptr<cExperimentSensorInfo> createSensor(std::string type)
{
	if (type == cExperimentSensorInfo_Dummy::type())				return std::make_shared<cExperimentSensorInfo_Dummy>();
	if (type == cExperimentSensorInfo_Ouster::type())				return std::make_shared<cExperimentSensorInfo_Ouster>();
	if (type == cExperimentSensorInfo_Septentrio::type())			return std::make_shared<cExperimentSensorInfo_Septentrio>();
	if (type == cExperimentSensorInfo_AxisCommunications::type())	return std::make_shared<cExperimentSensorInfo_AxisCommunications>();
	if (type == cExperimentSensorInfo_VNIR3000N::type())			return std::make_shared<cExperimentSensorInfo_VNIR3000N>();
	if (type == cExperimentSensorInfo_SWIR384::type())				return std::make_shared<cExperimentSensorInfo_SWIR384>();

	return std::shared_ptr<cExperimentSensorInfo>();
}


cExperimentSensorInfo::~cExperimentSensorInfo()
{}

void cExperimentSensorInfo::clear()
{
	mDirty = false;
}

bool cExperimentSensorInfo::isDirty() const
{
	return mDirty;
}

QString cExperimentSensorInfo::getName() const
{
	return QString::fromStdString(mName);
}

void cExperimentSensorInfo::setName(const std::string& name)
{
	mDirty |= mName != name;
	mName = name;
}


cExperimentSensorInfo_Dummy::cExperimentSensorInfo_Dummy()
{
	mName = "Dummy";
}

cExperimentSensorInfo_Dummy::~cExperimentSensorInfo_Dummy()
{}

const char* cExperimentSensorInfo_Dummy::type() { return dummy_class_id; }
std::string cExperimentSensorInfo_Dummy::getType() const { return type(); }

class cSensorWidget_Dummy : public cSensorWidget
{
public:
	cSensorWidget_Dummy()
	{
		QVBoxLayout* pDummyInfo = new QVBoxLayout();

		QLabel* pText = new QLabel("No Parameters");
		pDummyInfo->addWidget(pText, 0);
		setLayout(pDummyInfo);
	}

	void accept() override {}
	void reset() override {}
};

cSensorWidget* cExperimentSensorInfo_Dummy::widget()
{
	return new cSensorWidget_Dummy();
}


void cExperimentSensorInfo_Dummy::load(const nlohmann::json& jdoc)
{}

void cExperimentSensorInfo_Dummy::save(nlohmann::json& jdoc)
{}



cExperimentSensorInfo_Ouster::cExperimentSensorInfo_Ouster()
{
	mName = "OS-0-128";
	mManufacturer = "OUSTER";
	mModel = "OS0-128";
	mSerialNumber = "992037000167";
	mMode = "1024x10";
}

cExperimentSensorInfo_Ouster::~cExperimentSensorInfo_Ouster()
{}

const char* cExperimentSensorInfo_Ouster::type() { return ouster_id; }
std::string cExperimentSensorInfo_Ouster::getType() const { return type(); }
const std::string& cExperimentSensorInfo_Ouster::getManufacturer() const { return mManufacturer; }
const std::string& cExperimentSensorInfo_Ouster::getModel() const { return mModel; }
const std::string& cExperimentSensorInfo_Ouster::getSerialNumber() const { return mSerialNumber; }
const std::string& cExperimentSensorInfo_Ouster::getMode() const { return mMode; }

void cExperimentSensorInfo_Ouster::setMode(const std::string& mode)
{
	mDirty |= mMode != mode;
	mMode = mode;
}

class cSensorWidget_Ouster : public cSensorWidget
{
public:
	cSensorWidget_Ouster(cExperimentSensorInfo_Ouster* parent) : mpParent(parent)
	{
		mpLidarModes = new QComboBox(this);
		mpLidarModes->addItem("512x10");
		mpLidarModes->addItem("512x20");
		mpLidarModes->addItem("1024x10");
		mpLidarModes->addItem("1024x20");
		mpLidarModes->addItem("2048x10");

		mpLidarModes->setCurrentText(QString::fromStdString(mpParent->getMode()));

		QLabel* pText = nullptr;

		QVBoxLayout* pMainLayout = new QVBoxLayout();

		QGridLayout* pGridLayout = new QGridLayout();

		pText = new QLabel("Name");
		pGridLayout->addWidget(pText, 0, 0);
		mpName = new QLineEdit(mpParent->getName());
		pGridLayout->addWidget(mpName, 0, 1);

		pText = new QLabel("Manufacturer");
		pGridLayout->addWidget(pText, 1, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getManufacturer()));
		pGridLayout->addWidget(pText, 1, 1);

		pText = new QLabel("Model");
		pGridLayout->addWidget(pText, 2, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getModel()));
		pGridLayout->addWidget(pText, 2, 1);

		pText = new QLabel("Serial Number");
		pGridLayout->addWidget(pText, 3, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(pText, 3, 1);

		pText = new QLabel("Lidar Mode:");
		pGridLayout->addWidget(pText, 5, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(mpLidarModes, 5, 1);

		pMainLayout->addLayout(pGridLayout, 0);

		setLayout(pMainLayout);
	}

	void accept() override
	{
		std::string name = mpName->text().toStdString();
		mpParent->setName(name);
			
		std::string mode = mpLidarModes->currentText().toStdString();
		mpParent->setMode(mode);
	}

	void reset() override
	{
		mpName->setText(mpParent->getName());
		mpLidarModes->setCurrentText(QString::fromStdString(mpParent->getMode()));
	}

private:
	cExperimentSensorInfo_Ouster* const mpParent;

	QLineEdit* mpName = nullptr;
	QComboBox* mpLidarModes = nullptr;

};

cSensorWidget* cExperimentSensorInfo_Ouster::widget()
{
	return new cSensorWidget_Ouster(this);
}

void cExperimentSensorInfo_Ouster::load(const nlohmann::json& jdoc)
{
	mMode = jdoc["mode"];

	if (jdoc.contains("Name"))
		mName = jdoc["Name"];
}

void cExperimentSensorInfo_Ouster::save(nlohmann::json& jdoc)
{
	nlohmann::json ouster;
	ouster["Name"] = mName;
	ouster["Manufacturer"] = mManufacturer;
	ouster["Model"] = mModel;
	ouster["Serial Number"] = mSerialNumber;

	nlohmann::json azimuth_window;
	azimuth_window.push_back(135.0);
	azimuth_window.push_back(225.0);
	ouster["azimuth window"] = azimuth_window;

	ouster["mode"] = mMode;

	jdoc[type()] = ouster;

	mDirty = false;
}



cExperimentSensorInfo_Septentrio::cExperimentSensorInfo_Septentrio()
{
	mName = "Altus-NR3";
	mManufacturer = "Septentrio";
	mModel = "Altus-NR3";
	mSerialNumber = "6106326";
}

cExperimentSensorInfo_Septentrio::~cExperimentSensorInfo_Septentrio()
{}
	
const char* cExperimentSensorInfo_Septentrio::type() {return ssnx_id; }
std::string cExperimentSensorInfo_Septentrio::getType() const {	return type(); }
const std::string& cExperimentSensorInfo_Septentrio::getManufacturer() const { return mManufacturer; }
const std::string& cExperimentSensorInfo_Septentrio::getModel() const { return mModel; }
const std::string& cExperimentSensorInfo_Septentrio::getSerialNumber() const { return mSerialNumber; }

class cSensorWidget_Septentrio : public cSensorWidget
{
public:
	cSensorWidget_Septentrio(cExperimentSensorInfo_Septentrio* parent) : mpParent(parent)
	{
		QLabel* pText = nullptr;

		QVBoxLayout* pMainLayout = new QVBoxLayout();

		QGridLayout* pGridLayout = new QGridLayout();

		pText = new QLabel("Name");
		pGridLayout->addWidget(pText, 0, 0);
		mpName = new QLineEdit(mpParent->getName());
		pGridLayout->addWidget(mpName, 0, 1);

		pText = new QLabel("Manufacturer");
		pGridLayout->addWidget(pText, 1, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getManufacturer()));
		pGridLayout->addWidget(pText, 1, 1);

		pText = new QLabel("Model");
		pGridLayout->addWidget(pText, 2, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getModel()));
		pGridLayout->addWidget(pText, 2, 1);

		pText = new QLabel("Serial Number");
		pGridLayout->addWidget(pText, 3, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(pText, 3, 1);

		pMainLayout->addLayout(pGridLayout, 0);

		setLayout(pMainLayout);
	}

	void accept() override
	{
		std::string name = mpName->text().toStdString();
		mpParent->setName(name);
	}

	void reset() override 
	{
		mpName->setText(mpParent->getName());
	}

private:
	QLineEdit* mpName = nullptr;

	cExperimentSensorInfo_Septentrio* const mpParent;
};

cSensorWidget* cExperimentSensorInfo_Septentrio::widget()
{
	return new cSensorWidget_Septentrio(this);
}

void cExperimentSensorInfo_Septentrio::load(const nlohmann::json& jdoc)
{
	if (jdoc.contains("Name"))
		mName = jdoc["Name"];
}

void cExperimentSensorInfo_Septentrio::save(nlohmann::json& jdoc)
{
	nlohmann::json ssnx;
	ssnx["Name"] = mName;
	ssnx["Manufacturer"] = mManufacturer;
	ssnx["Model"] = mModel;
	ssnx["Serial Number"] = mSerialNumber;

	jdoc[type()] = ssnx;

	mDirty = false;
}



cExperimentSensorInfo_AxisCommunications::cExperimentSensorInfo_AxisCommunications()
{
	mName = "Axis F44 Webcam";
	mManufacturer = "Axis Communications";
	mModel = "AXIS F44 DUAL AUDO INPUT";
	mSerialNumber = "B8A44F1E88FB";
}

cExperimentSensorInfo_AxisCommunications::~cExperimentSensorInfo_AxisCommunications()
{}

const char* cExperimentSensorInfo_AxisCommunications::type() { return axis_communications_id; }
std::string cExperimentSensorInfo_AxisCommunications::getType() const { return type(); }
const std::string& cExperimentSensorInfo_AxisCommunications::getManufacturer() const { return mManufacturer; }
const std::string& cExperimentSensorInfo_AxisCommunications::getModel() const { return mModel; }
const std::string& cExperimentSensorInfo_AxisCommunications::getSerialNumber() const { return mSerialNumber; }

int cExperimentSensorInfo_AxisCommunications::getCameraId() const { return mCameraId; }
int cExperimentSensorInfo_AxisCommunications::getImageWidth() const { return mImageWidth; }
int cExperimentSensorInfo_AxisCommunications::getImageHeight() const { return mImageHeight; }
int cExperimentSensorInfo_AxisCommunications::getFrameRate_fps() const { return mFrameRate_fps; }

void cExperimentSensorInfo_AxisCommunications::setCameraId(int id)
{
	mDirty |= mCameraId != id;
	mCameraId = id;
}

void cExperimentSensorInfo_AxisCommunications::setImageSize(int width, int height)
{
	mDirty |= (mImageWidth != width) || (mImageHeight != height);
	mImageWidth = width;
	mImageHeight = height;
}

void cExperimentSensorInfo_AxisCommunications::setFrameRate_fps(int fps)
{
	mDirty |= mFrameRate_fps != fps;
	mFrameRate_fps = fps;
}


class cSensorWidget_AxisCommunications : public cSensorWidget
{
public:
	cSensorWidget_AxisCommunications(cExperimentSensorInfo_AxisCommunications* parent) : mpParent(parent)
	{
		mpCameraId = new QLineEdit(this);
		mpCameraId->setValidator(new QIntValidator(1, 2));
		mpCameraId->setText(QString::number(mpParent->getCameraId()));

		mpImageSizes = new QComboBox(this);
		mpImageSizes->addItem("1920x1080");
		mpImageSizes->addItem("1280x720");
		mpImageSizes->addItem("1024x768");
		mpImageSizes->addItem("1024x640");
		mpImageSizes->addItem("800x600");
		mpImageSizes->addItem("640x480");

		QString resolution = QString::number(mpParent->getImageWidth());
		resolution += "x";
		resolution += QString::number(mpParent->getImageHeight());
		mpImageSizes->setCurrentText(resolution);

		mpFrameRate_fps = new QLineEdit(this);
		mpFrameRate_fps->setValidator(new QIntValidator(1, 10));
		mpFrameRate_fps->setText(QString::number(mpParent->getFrameRate_fps()));

		QLabel* pText = nullptr;
		QLineEdit* pEdit = nullptr;

		QVBoxLayout* pMainLayout = new QVBoxLayout();

		QGridLayout* pGridLayout = new QGridLayout();

		pText = new QLabel("Name");
		pGridLayout->addWidget(pText, 0, 0);
		mpName = new QLineEdit(mpParent->getName());
		pGridLayout->addWidget(mpName, 0, 1);

		pText = new QLabel("Manufacturer");
		pGridLayout->addWidget(pText, 1, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getManufacturer()));
		pGridLayout->addWidget(pText, 1, 1);

		pText = new QLabel("Model");
		pGridLayout->addWidget(pText, 2, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getModel()));
		pGridLayout->addWidget(pText, 2, 1);

		pText = new QLabel("Serial Number");
		pGridLayout->addWidget(pText, 3, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(pText, 3, 1);

		pText = new QLabel("Camera ID:");
		pGridLayout->addWidget(pText, 5, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(mpCameraId, 5, 1);

		pText = new QLabel("Image Size (w x h):");
		pGridLayout->addWidget(pText, 6, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(mpImageSizes, 6, 1);

		pText = new QLabel("Frames per Second:");
		pGridLayout->addWidget(pText, 7, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(mpFrameRate_fps, 7, 1);

		pMainLayout->addLayout(pGridLayout, 0);

		setLayout(pMainLayout);
	}

	void accept() override
	{
		std::string name = mpName->text().toStdString();
		mpParent->setName(name);

		mpParent->setCameraId(mpCameraId->text().toInt());

		auto resolution = mpImageSizes->currentText().split('x');

		if (resolution.size() == 2)
		{
			int width = resolution[0].toInt();
			int height = resolution[1].toInt();
			mpParent->setImageSize(width, height);
		}

		mpParent->setFrameRate_fps(mpFrameRate_fps->text().toInt());
	}

	void reset() override 
	{
		mpName->setText(mpParent->getName());

		mpCameraId->setText(QString::number(mpParent->getCameraId()));

		QString resolution = QString::number(mpParent->getImageWidth());
		resolution += "x";
		resolution += QString::number(mpParent->getImageHeight());
		mpImageSizes->setCurrentText(resolution);

		mpFrameRate_fps->setText(QString::number(mpParent->getFrameRate_fps()));
	}

private:
	cExperimentSensorInfo_AxisCommunications* const mpParent;

	QLineEdit* mpName = nullptr;
	QLineEdit* mpCameraId = nullptr;
	QComboBox* mpImageSizes = nullptr;
	QLineEdit* mpFrameRate_fps = nullptr;
};

cSensorWidget* cExperimentSensorInfo_AxisCommunications::widget()
{
	return new cSensorWidget_AxisCommunications(this);
}

void cExperimentSensorInfo_AxisCommunications::load(const nlohmann::json& jdoc)
{
	if (jdoc.contains("Name"))
		mName = jdoc["Name"];

	if (jdoc.contains("camera id"))
		mCameraId = jdoc["camera id"];

	if (jdoc.contains("resolution"))
	{
		std::string resolution = jdoc["resolution"];

		auto x = resolution.find('x');

		if (x != std::string::npos)
		{
			mImageWidth = std::stoi(resolution.substr(0, x));
			mImageHeight = std::stoi(resolution.substr(x + 1));
		}
	}

	if (jdoc.contains("frames per second"))
		mFrameRate_fps = jdoc["frames per second"];
}

void cExperimentSensorInfo_AxisCommunications::save(nlohmann::json& jdoc)
{
	nlohmann::json axis;

	axis["Name"] = mName;
	axis["Manufacturer"] = mManufacturer;
	axis["Model"] = mModel;
	axis["Serial Number"] = mSerialNumber;

	axis["camera id"] = mCameraId;

	std::string resolution = std::to_string(mImageWidth);
	resolution += "x";
	resolution += std::to_string(mImageHeight);
	axis["resolution"] = resolution;

	axis["frames per second"] = mFrameRate_fps;

	jdoc[type()] = axis;

	mDirty = false;
}



cExperimentSensorInfo_VNIR3000N::cExperimentSensorInfo_VNIR3000N()
{
	mName = "VNIR-3000N";
	mManufacturer = "HySpex";
	mModel = "VNIR-3000N";
	mSerialNumber = "202184";
}

cExperimentSensorInfo_VNIR3000N::~cExperimentSensorInfo_VNIR3000N()
{}

const char* cExperimentSensorInfo_VNIR3000N::type() { return vnir_3000N_id; }
std::string cExperimentSensorInfo_VNIR3000N::getType() const { return type(); }
const std::string& cExperimentSensorInfo_VNIR3000N::getManufacturer() const { return mManufacturer; }
const std::string& cExperimentSensorInfo_VNIR3000N::getModel() const { return mModel; }
const std::string& cExperimentSensorInfo_VNIR3000N::getSerialNumber() const { return mSerialNumber; }

class cSensorWidget_VNIR3000N : public cSensorWidget
{
public:
	cSensorWidget_VNIR3000N(cExperimentSensorInfo_VNIR3000N* parent) : mpParent(parent)
	{
		QLabel* pText = nullptr;

		QVBoxLayout* pMainLayout = new QVBoxLayout();

		QGridLayout* pGridLayout = new QGridLayout();

		pText = new QLabel("Name");
		pGridLayout->addWidget(pText, 0, 0);
		mpName = new QLineEdit(mpParent->getName());
		pGridLayout->addWidget(mpName, 0, 1);

		pText = new QLabel("Manufacturer");
		pGridLayout->addWidget(pText, 1, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getManufacturer()));
		pGridLayout->addWidget(pText, 1, 1);

		pText = new QLabel("Model");
		pGridLayout->addWidget(pText, 2, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getModel()));
		pGridLayout->addWidget(pText, 2, 1);

		pText = new QLabel("Serial Number");
		pGridLayout->addWidget(pText, 3, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(pText, 3, 1);

		pMainLayout->addLayout(pGridLayout, 0);

		setLayout(pMainLayout);
	}

	void accept() override
	{
		std::string name = mpName->text().toStdString();
		mpParent->setName(name);
	}
	
	void reset() override
	{
		mpName->setText(mpParent->getName());
	}

private:
	QLineEdit* mpName = nullptr;

	cExperimentSensorInfo_VNIR3000N* const mpParent;
};

cSensorWidget* cExperimentSensorInfo_VNIR3000N::widget()
{
	return new cSensorWidget_VNIR3000N(this);
}

void cExperimentSensorInfo_VNIR3000N::load(const nlohmann::json& jdoc)
{
	if (jdoc.contains("Name"))
		mName = jdoc["Name"];
}

void cExperimentSensorInfo_VNIR3000N::save(nlohmann::json& jdoc)
{
	nlohmann::json vnir;
	vnir["Name"] = mName;
	vnir["Manufacturer"] = mManufacturer;
	vnir["Model"] = mModel;
	vnir["Serial Number"] = mSerialNumber;

	jdoc[type()] = vnir;

	mDirty = false;
}



cExperimentSensorInfo_SWIR384::cExperimentSensorInfo_SWIR384()
{
	mName = "SWIR-384";
	mManufacturer = "HySpex";
	mModel = "SWIR-384";
	mSerialNumber = "200918";
}

cExperimentSensorInfo_SWIR384::~cExperimentSensorInfo_SWIR384()
{}

const char* cExperimentSensorInfo_SWIR384::type() { return swir_384_id; }
std::string cExperimentSensorInfo_SWIR384::getType() const { return type(); }
const std::string& cExperimentSensorInfo_SWIR384::getManufacturer() const { return mManufacturer; }
const std::string& cExperimentSensorInfo_SWIR384::getModel() const { return mModel; }
const std::string& cExperimentSensorInfo_SWIR384::getSerialNumber() const { return mSerialNumber; }

class cSensorWidget_SWIR384 : public cSensorWidget
{
public:
	cSensorWidget_SWIR384(cExperimentSensorInfo_SWIR384* parent) : mpParent(parent)
	{
		QLabel* pText = nullptr;

		QVBoxLayout* pMainLayout = new QVBoxLayout();

		QGridLayout* pGridLayout = new QGridLayout();

		pText = new QLabel("Name");
		pGridLayout->addWidget(pText, 0, 0);
		mpName = new QLineEdit(mpParent->getName());
		pGridLayout->addWidget(mpName, 0, 1);

		pText = new QLabel("Manufacturer");
		pGridLayout->addWidget(pText, 1, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getManufacturer()));
		pGridLayout->addWidget(pText, 1, 1);

		pText = new QLabel("Model");
		pGridLayout->addWidget(pText, 2, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getModel()));
		pGridLayout->addWidget(pText, 2, 1);

		pText = new QLabel("Serial Number");
		pGridLayout->addWidget(pText, 3, 0);
		pText = new QLabel(QString::fromStdString(mpParent->getSerialNumber()));
		pGridLayout->addWidget(pText, 3, 1);

		pMainLayout->addLayout(pGridLayout, 0);

		setLayout(pMainLayout);
	}

	void accept() override
	{
		std::string name = mpName->text().toStdString();
		mpParent->setName(name);
	}

	void reset() override 
	{
		mpName->setText(mpParent->getName());
	}

private:
	QLineEdit* mpName = nullptr;

	cExperimentSensorInfo_SWIR384* const mpParent;
};

cSensorWidget* cExperimentSensorInfo_SWIR384::widget()
{
	return new cSensorWidget_SWIR384(this);
}

void cExperimentSensorInfo_SWIR384::load(const nlohmann::json& jdoc)
{
	if (jdoc.contains("Name"))
		mName = jdoc["Name"];
}

void cExperimentSensorInfo_SWIR384::save(nlohmann::json& jdoc)
{
	nlohmann::json swir;
	swir["Name"] = mName;
	swir["Manufacturer"] = mManufacturer;
	swir["Model"] = mModel;
	swir["Serial Number"] = mSerialNumber;

	jdoc[type()] = swir;

	mDirty = false;
}



