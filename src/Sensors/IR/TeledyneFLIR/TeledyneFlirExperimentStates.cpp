
#include "TeledyneFlirExperimentStates.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirPropertyPage_Remote.hpp"
#include "StringUtils.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>

const long long NETWORK_DELAY_MS = 500;

/*******************************************************************/
/**  Base Class for Teledyne FLIR Local HySpex Experiment States  **/
/*******************************************************************/
cTeledyneFlirCameraExperimentState_Local::cTeledyneFlirCameraExperimentState_Local(cTeledyneFlirCameraModel* pModel, QObject* parent)
	: QObject(parent), mpModel(pModel)
{}

cTeledyneFlirCameraExperimentState_Local::~cTeledyneFlirCameraExperimentState_Local()
{
}

bool cTeledyneFlirCameraExperimentState_Local::initialize()
{
	return true;
}

void cTeledyneFlirCameraExperimentState_Local::cleanup()
{}


/*******************************************************************/
/**     Base Class for Teledyne FLIR Remote Experiment States     **/
/*******************************************************************/
cTeledyneFlirCameraExperimentState_Remote::cTeledyneFlirCameraExperimentState_Remote
	(const std::string& hostname, uint16_t port, const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: cExperimentStateRemoteInterface(parent), cTeledyneFlirPropertiesNetEncoder(255)
{
	mHostname = hostname;
	mPort = port;
	mLocalIpAddress = localIpAddress;
	mUse_IpV6 = use_IpV6;
}

cTeledyneFlirCameraExperimentState_Remote::~cTeledyneFlirCameraExperimentState_Remote()
{}

bool cTeledyneFlirCameraExperimentState_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return false;

	return openConnection();
}

void cTeledyneFlirCameraExperimentState_Remote::cleanup()
{
	closeConnection();
	destroy();
}

bool cTeledyneFlirCameraExperimentState_Remote::recording()
{
	return false;
}

void cTeledyneFlirCameraExperimentState_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cTeledyneFlirPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cTeledyneFlirCameraExperimentState_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}


/*******************************************************************/
/**    Teledyne FLIR Experiment States to Configure Camera        **/
/*******************************************************************/

cTeledyneFlirCamera_Configure_Local::cTeledyneFlirCamera_Configure_Local(cTeledyneFlirCameraModel* pModel, QObject* parent)
	: cTeledyneFlirCameraExperimentState_Local(pModel, parent)
{
}

QString cTeledyneFlirCamera_Configure_Local::getStatusStr()
{
	return "Configuring FLIR camera...";
}

bool cTeledyneFlirCamera_Configure_Local::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("mode"))
		{
			auto mode = stateDoc["mode"];
			if (nStringUtils::iequal(mode, "photo"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::SINGLE);
			}
			else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::TIME_LAPSE);
			}
			else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::CONTINUOUS);
			}

			mWaitingForMode = mMode != static_cast<int>(mpModel->mode());
		}

		if (stateDoc.contains("frame rate (hz)"))
		{
			mFrameRate_fps = stateDoc["frame rate (hz)"];

			mWaitingForFrameRate = mFrameRate_fps != mpModel->frameRate_Hz();
		}

		int32_t frame_interval_ms = -1;

		if (stateDoc.contains("frame interval (s)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["frame interval (s)"] * 1000);
		else if (stateDoc.contains("frame interval (ms)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["frame interval (s)"]);

		if (stateDoc.contains("interval (s)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["interval (s)"] * 1000);
		else if (stateDoc.contains("interval (ms)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["interval (ms)"]);

		if (frame_interval_ms > 0)
		{
			mFrameInterval_ms = frame_interval_ms;

			mWaitingForInterval = mFrameInterval_ms != mpModel->frameInterval_ms();
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cTeledyneFlirCamera_Configure_Local::run()
{
	if (mUpdateConfiguration)
	{
		if (mWaitingForMode)
			emit requestMode(mMode);

		if (mWaitingForFrameRate)
			emit requestFrameRate_Hz(mFrameRate_fps);

		if (mWaitingForInterval)
			emit requestFrameInterval_ms(mFrameInterval_ms);

		mUpdateConfiguration = false;
	}
}

void cTeledyneFlirCamera_Configure_Local::pause() {}
void cTeledyneFlirCamera_Configure_Local::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_Configure_Local::finished()
{
	if (mWaitingForMode || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

void cTeledyneFlirCamera_Configure_Local::modeChanged(int mode)
{
	mWaitingForMode = false;
}

void cTeledyneFlirCamera_Configure_Local::frameIntervalChanged(int interval_ms)
{
	mWaitingForInterval = false;
}

void cTeledyneFlirCamera_Configure_Local::frameRateChanged(double rate_fps)
{
	mWaitingForFrameRate = false;
}

/*** Remote Version ***/

cTeledyneFlirCamera_Configure_Remote::cTeledyneFlirCamera_Configure_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
:
	cTeledyneFlirCameraExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{
}

QString cTeledyneFlirCamera_Configure_Remote::getStatusStr()
{
	return "Configuring FLIR camera...";
}

bool cTeledyneFlirCamera_Configure_Remote::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("mode"))
		{
			auto mode = stateDoc["mode"];
			if (nStringUtils::iequal(mode, "photo"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::SINGLE);
			}
			else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::TIME_LAPSE);
			}
			else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::CONTINUOUS);
			}
		}

		if (stateDoc.contains("frame rate (hz)"))
		{
			mFrameRate_fps = stateDoc["frame rate (hz)"];
		}

		int32_t frame_interval_ms = -1;

		if (stateDoc.contains("frame interval (s)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["frame interval (s)"] * 1000);
		else if (stateDoc.contains("frame interval (ms)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["frame interval (s)"]);

		if (stateDoc.contains("interval (s)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["interval (s)"] * 1000);
		else if (stateDoc.contains("interval (ms)"))
			frame_interval_ms = static_cast<int32_t>(stateDoc["interval (ms)"]);

		if (frame_interval_ms > 0)
		{
			mFrameInterval_ms = frame_interval_ms;
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cTeledyneFlirCamera_Configure_Remote::run() {}
void cTeledyneFlirCamera_Configure_Remote::pause() {}
void cTeledyneFlirCamera_Configure_Remote::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_Configure_Remote::finished()
{
	if (mWaitingForMode || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

void cTeledyneFlirCamera_Configure_Remote::onMode(uint8_t id)
{
	mWaitingForMode = false;
}

void cTeledyneFlirCamera_Configure_Remote::onFrameRate(double fps)
{
	mWaitingForFrameRate = false;
}

void cTeledyneFlirCamera_Configure_Remote::onFrameInterval(uint32_t interval_ms)
{
	mWaitingForInterval = false;
}

void cTeledyneFlirCamera_Configure_Remote::onCurrentState(bool valid, uint8_t mode,
	uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
	std::optional<double> min_fps, std::optional<double> max_fps,
	std::optional<float> min_K, std::optional<float> max_K)
{
	if (mMode >= 0)
		mWaitingForMode = mode != mMode;

	if (mFrameRate_fps > 0)
		mWaitingForFrameRate = mFrameRate_fps != fps;

	if (mFrameInterval_ms > 0)
		mWaitingForInterval = mFrameInterval_ms != interval_ms;

	if (mWaitingForMode)
		sendSetMode(static_cast<uint8_t>(mMode));

	if (mWaitingForFrameRate)
		sendSetFrameRate_fps(mFrameRate_fps);

	if (mWaitingForInterval)
		sendSetFrameInterval_ms(mFrameInterval_ms);
}



void cTeledyneFlirCamera_Configure_Remote::onConnect()
{
	sendQueryState();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}


/*******************************************************************/
/**       Teledyne FLIR Experiment States to Take Photo           **/
/*******************************************************************/

cTeledyneFlirCamera_TakePhoto_Local::cTeledyneFlirCamera_TakePhoto_Local(cTeledyneFlirCameraModel* pModel, QObject* parent)
	: cTeledyneFlirCameraExperimentState_Local(pModel, parent)
{
}

QString cTeledyneFlirCamera_TakePhoto_Local::getStatusStr()
{
	if (mUpdateView)
		return "Taking Photo and updating view...";

	return "Taking Photo...";
}

bool cTeledyneFlirCamera_TakePhoto_Local::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mUpdateView = false;

	try
	{
		if (stateDoc.contains("update view"))
		{
			mUpdateView = stateDoc["update view"];
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cTeledyneFlirCamera_TakePhoto_Local::run() 
{
	if (mTriggerPhoto)
	{
		if (mpModel->mode() == cTeledyneFlirCameraModel::eMode::SINGLE)
		{
			emit takePhoto(mUpdateView);
		}
		else
		{
			if (mUpdateView)
				emit updateView();

			mResult = cExperimentState::eRESULT::DONE;
		}

		mTriggerPhoto = false;
	}
}

void cTeledyneFlirCamera_TakePhoto_Local::pause() {}
void cTeledyneFlirCamera_TakePhoto_Local::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_TakePhoto_Local::finished()
{
	return mResult;
}

void cTeledyneFlirCamera_TakePhoto_Local::onPhotoTaken()
{
	mResult = cExperimentState::eRESULT::DONE;
};

/*** Remote Version ***/

cTeledyneFlirCamera_TakePhoto_Remote::cTeledyneFlirCamera_TakePhoto_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
:
	cTeledyneFlirCameraExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{
}

QString cTeledyneFlirCamera_TakePhoto_Remote::getStatusStr()
{
	if (mUpdateView)
		return "Taking Photo and updating view...";

	return "Taking Photo...";
}

bool cTeledyneFlirCamera_TakePhoto_Remote::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mUpdateView = false;

	try
	{
		if (stateDoc.contains("update view"))
		{
			mUpdateView = stateDoc["update view"];
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cTeledyneFlirCamera_TakePhoto_Remote::run() {}
void cTeledyneFlirCamera_TakePhoto_Remote::pause() {}
void cTeledyneFlirCamera_TakePhoto_Remote::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_TakePhoto_Remote::finished()
{
	return mResult;
}

void cTeledyneFlirCamera_TakePhoto_Remote::onTakePhotoReply(bool error)
{
	mResult = cExperimentState::eRESULT::DONE;
};

void cTeledyneFlirCamera_TakePhoto_Remote::onMode(uint8_t mode)
{
	if (mode == cTeledyneFlirCameraModel::eMode::SINGLE)
	{
		sendTakePhoto(mUpdateView);
	}
	else
	{
		if (mUpdateView)
			sendGrabImage();

		mResult = cExperimentState::eRESULT::DONE;
	}
}

void cTeledyneFlirCamera_TakePhoto_Remote::onConnect()
{
	sendQueryMode();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}



