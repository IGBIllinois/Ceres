
#include "TeledyneFlirExperimentStates_Remote.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirPropertyPage_Remote.hpp"
#include "StringUtils.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>

const long long NETWORK_DELAY_MS = 500;


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


/***********************************************************************/
/**    Teledyne FLIR Experiment State to Save Current Camera State    **/
/***********************************************************************/

cTeledyneFlirCamera_SaveState_Remote::cTeledyneFlirCamera_SaveState_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cTeledyneFlirCameraExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{}

QString cTeledyneFlirCamera_SaveState_Remote::getStatusStr()
{
	return "Saving current camera state...";
}

void cTeledyneFlirCamera_SaveState_Remote::run() {}
void cTeledyneFlirCamera_SaveState_Remote::pause() {}
void cTeledyneFlirCamera_SaveState_Remote::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_SaveState_Remote::finished() { return mResult; }

void cTeledyneFlirCamera_SaveState_Remote::onConnect()
{
	sendSaveStateMessage();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

	eRESULT mResult = cExperimentState::eRESULT::DONE;
};


/***********************************************************************/
/**      Teledyne FLIR Experiment State to Restore Camera State       **/
/***********************************************************************/

cTeledyneFlirCamera_RestoreState_Remote::cTeledyneFlirCamera_RestoreState_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cTeledyneFlirCameraExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{}

QString cTeledyneFlirCamera_RestoreState_Remote::getStatusStr()
{
	return "Restoring camera state...";
}

void cTeledyneFlirCamera_RestoreState_Remote::run() {}
void cTeledyneFlirCamera_RestoreState_Remote::pause() {}
void cTeledyneFlirCamera_RestoreState_Remote::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_RestoreState_Remote::finished() { return mResult; }

void cTeledyneFlirCamera_RestoreState_Remote::onConnect()
{
	sendRestoreStateMessage();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

	eRESULT mResult = cExperimentState::eRESULT::DONE;
};


/*******************************************************************/
/**    Teledyne FLIR Experiment States to Configure Camera        **/
/*******************************************************************/

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

		int32_t lapse_interval_ms = -1;

		if (stateDoc.contains("time-lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["time-lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("time-lapse interval (ms)"))
			lapse_interval_ms = stateDoc["time-lapse interval (ms)"].get<int32_t>();

		else if (stateDoc.contains("time lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["time lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("time lapse interval (ms)"))
			lapse_interval_ms = stateDoc["time lapse interval (ms)"].get<int32_t>();

		else if (stateDoc.contains("lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("lapse interval (ms)"))
			lapse_interval_ms = stateDoc["lapse interval (ms)"].get<int32_t>();

		else if (stateDoc.contains("interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("interval (ms)"))
			lapse_interval_ms = stateDoc["interval (ms)"].get<int32_t>();

		if (lapse_interval_ms > 0)
		{
			mLapseInterval_ms = lapse_interval_ms;
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

cExperimentState::eRESULT cTeledyneFlirCamera_Configure_Remote::finished()
{
	if (mWaitingForConfiguration)
		return cExperimentState::eRESULT::WAITING;

	if (mWaitingForMode || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

void cTeledyneFlirCamera_Configure_Remote::onModeMessage(uint8_t id)
{
	mWaitingForMode = false;
}

void cTeledyneFlirCamera_Configure_Remote::onFrameRateMessage(double fps)
{
	mWaitingForFrameRate = false;
}

void cTeledyneFlirCamera_Configure_Remote::onLapseIntervalMessage(uint32_t interval_ms)
{
	mWaitingForInterval = false;
}

void cTeledyneFlirCamera_Configure_Remote::onCurrentStateMessage(bool valid, uint8_t mode,
	uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
	std::optional<double> min_fps, std::optional<double> max_fps,
	std::optional<float> min_K, std::optional<float> max_K)
{
	if (mMode >= 0)
		mWaitingForMode = mode != mMode;

	if (mFrameRate_fps > 0)
		mWaitingForFrameRate = mFrameRate_fps != fps;

	if (mLapseInterval_ms > 0)
		mWaitingForInterval = mLapseInterval_ms != interval_ms;

	if (mWaitingForMode)
		sendSetModeMessage(static_cast<uint8_t>(mMode));

	if (mWaitingForFrameRate)
		sendSetFrameRateMessage(mFrameRate_fps);

	if (mWaitingForInterval)
		sendSetLapseIntervalMessage(mLapseInterval_ms);

	mWaitingForConfiguration = false;
}

void cTeledyneFlirCamera_Configure_Remote::onConnect()
{
	sendQueryStateMessage();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}


/*******************************************************************/
/**       Teledyne FLIR Experiment States to Take Photo           **/
/*******************************************************************/

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

cExperimentState::eRESULT cTeledyneFlirCamera_TakePhoto_Remote::finished()
{
	return mResult;
}

void cTeledyneFlirCamera_TakePhoto_Remote::onTakePhotoReplyMessage(bool error)
{
	mResult = cExperimentState::eRESULT::DONE;
};

void cTeledyneFlirCamera_TakePhoto_Remote::onModeMessage(uint8_t mode)
{
	if (mode == cTeledyneFlirCameraModel::eMode::SINGLE)
	{
		sendTakePhotoMessage(mUpdateView, true);
	}
	else
	{
		if (mUpdateView)
			sendGrabImageMessage();

		mResult = cExperimentState::eRESULT::DONE;
	}
}

void cTeledyneFlirCamera_TakePhoto_Remote::onConnect()
{
	sendQueryModeMessage();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}



