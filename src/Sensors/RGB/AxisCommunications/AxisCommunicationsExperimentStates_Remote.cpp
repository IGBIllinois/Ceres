
#include "AxisCommunicationsExperimentStates_Remote.hpp"
#include "AxisCommunicationsModel.hpp"
#include "AxisCommunicationsPropertyPage_Remote.hpp"
#include "StringUtils.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>

const long long NETWORK_DELAY_MS = 500;


/***********************************************************************************/
/**         Base Class for Axis Communications Remote Experiment States           **/
/***********************************************************************************/
cAxisCommunicationsExperimentState_Remote::cAxisCommunicationsExperimentState_Remote
	(const std::string& hostname, uint16_t port, const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: cExperimentStateRemoteInterface(parent), cAxisPropertiesNetEncoder(255)
{
	mHostname = hostname;
	mPort = port;
	mLocalIpAddress = localIpAddress;
	mUse_IpV6 = use_IpV6;
}

cAxisCommunicationsExperimentState_Remote::~cAxisCommunicationsExperimentState_Remote()
{}

bool cAxisCommunicationsExperimentState_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return false;

	return openConnection();
}

void cAxisCommunicationsExperimentState_Remote::cleanup()
{
	closeConnection();
	destroy();
}

bool cAxisCommunicationsExperimentState_Remote::recording()
{
	return false;
}

void cAxisCommunicationsExperimentState_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cAxisPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cAxisCommunicationsExperimentState_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}


/***************************************************************************/
/**     Axis Communications Experiment States to Configure Camera         **/
/***************************************************************************/

cAxisCommunications_Configure_Remote::cAxisCommunications_Configure_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
:
	cAxisCommunicationsExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{
}

QString cAxisCommunications_Configure_Remote::getStatusStr()
{
	return "Configuring Axis Communications camera...";
}

bool cAxisCommunications_Configure_Remote::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("mode"))
		{
			auto mode = stateDoc["mode"];
			if (nStringUtils::iequal(mode, "photo"))
			{
				mMode = static_cast<int>(cAxisCommunicationsModel::eMode::SINGLE);
			}
			else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
			{
				mMode = static_cast<int>(cAxisCommunicationsModel::eMode::TIME_LAPSE);
			}
			else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
			{
				mMode = static_cast<int>(cAxisCommunicationsModel::eMode::CONTINUOUS);
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

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

cExperimentState::eRESULT cAxisCommunications_Configure_Remote::finished()
{
	if (mWaitingForConfiguration)
		return cExperimentState::eRESULT::WAITING;

	if (mWaitingForMode || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

/*
void cLucidVisionLabsRgbCamera_Configure_Remote::onMode(uint8_t id)
{
	mWaitingForMode = false;
}

void cLucidVisionLabsRgbCamera_Configure_Remote::onFrameRate(double fps)
{
	mWaitingForFrameRate = false;
}

void cLucidVisionLabsRgbCamera_Configure_Remote::onLapseInterval(uint32_t interval_ms)
{
	mWaitingForInterval = false;
}

void cLucidVisionLabsRgbCamera_Configure_Remote::onCurrentState(bool valid, uint8_t mode,
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
		sendSetMode(static_cast<uint8_t>(mMode));

	if (mWaitingForFrameRate)
		sendSetFrameRate_fps(mFrameRate_fps);

	if (mWaitingForInterval)
		sendSetLapseInterval_ms(mLapseInterval_ms);

	mWaitingForConfiguration = false;
}
*/

void cAxisCommunications_Configure_Remote::onConnect()
{
	sendQueryState();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}


/****************************************************************************/
/**         Axis Communications Experiment States to Take Photo            **/
/****************************************************************************/

cAxisCommunications_TakePhoto_Remote::cAxisCommunications_TakePhoto_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
:
	cAxisCommunicationsExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{
}

QString cAxisCommunications_TakePhoto_Remote::getStatusStr()
{
	if (mUpdateView)
		return "Taking Photo and updating view...";

	return "Taking Photo...";
}

bool cAxisCommunications_TakePhoto_Remote::configure(const nlohmann::json& stateDoc)
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

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

cExperimentState::eRESULT cAxisCommunications_TakePhoto_Remote::finished()
{
	return mResult;
}

/*
void cLucidVisionLabsRgbCamera_TakePhoto_Remote::onTakePhotoReply(bool error)
{
	mResult = cExperimentState::eRESULT::DONE;
};

void cLucidVisionLabsRgbCamera_TakePhoto_Remote::onMode(uint8_t mode)
{
	if (mode == cLucidVisionLabsRgbModel::eMode::SINGLE)
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
*/

void cAxisCommunications_TakePhoto_Remote::onConnect()
{
//	sendQueryMode();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}



