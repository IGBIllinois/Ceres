
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


/***********************************************************************/
/** Axis Communications Experiment State to Save Current Camera State **/
/***********************************************************************/

cAxisCommunications_SaveState_Remote::cAxisCommunications_SaveState_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cAxisCommunicationsExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{}

QString cAxisCommunications_SaveState_Remote::getStatusStr()
{
	return "Saving current camera state...";
}

void cAxisCommunications_SaveState_Remote::run() {}
void cAxisCommunications_SaveState_Remote::pause() {}
void cAxisCommunications_SaveState_Remote::stop() {}

cExperimentState::eRESULT cAxisCommunications_SaveState_Remote::finished() { return mResult; }

void cAxisCommunications_SaveState_Remote::onConnect()
{
	sendSaveState();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

	mResult = cExperimentState::eRESULT::DONE;
};


/***********************************************************************/
/**   Axis Communications Experiment State to Restore Camera State    **/
/***********************************************************************/

cAxisCommunications_RestoreState_Remote::cAxisCommunications_RestoreState_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cAxisCommunicationsExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{}

QString cAxisCommunications_RestoreState_Remote::getStatusStr()
{
	return "Restoring camera state...";
}

void cAxisCommunications_RestoreState_Remote::run() {}
void cAxisCommunications_RestoreState_Remote::pause() {}
void cAxisCommunications_RestoreState_Remote::stop() {}

cExperimentState::eRESULT cAxisCommunications_RestoreState_Remote::finished() { return mResult; }

void cAxisCommunications_RestoreState_Remote::onConnect()
{
	sendRestoreState();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

	mResult = cExperimentState::eRESULT::DONE;
};


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
	auto result = cAxisCommunicationsExperimentHelper_Configure::configure(stateDoc);

	mWaitingForMode = false;
	mWaitingForCameraID = false;
	mWaitingForResolution = false;
	mWaitingForFrameRate = false;
	mWaitingForInterval = false;

	return result;
}

void cAxisCommunications_Configure_Remote::run()
{
	if (!mConnected) return;

	if (mWaitingForConfiguration) return;
}

cExperimentState::eRESULT cAxisCommunications_Configure_Remote::finished()
{
	if (mWaitingForConfiguration)
		return cExperimentState::eRESULT::WAITING;

	if (mWaitingForMode || mWaitingForCameraID || mWaitingForResolution || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

void cAxisCommunications_Configure_Remote::onMode(uint8_t mode)
{
	mWaitingForMode = false;
}

void cAxisCommunications_Configure_Remote::onCameraId(uint8_t id)
{
	mWaitingForCameraID = false;
}

void cAxisCommunications_Configure_Remote::onImageSize(uint16_t width, uint16_t height)
{
	mWaitingForResolution = false;
}

void cAxisCommunications_Configure_Remote::onFrameRate(uint8_t fps)
{
	mWaitingForFrameRate = false;
}

void cAxisCommunications_Configure_Remote::onLapseInterval(uint32_t interval_ms)
{
	mWaitingForInterval = false;
}

void cAxisCommunications_Configure_Remote::onCurrentState(bool valid, uint8_t id, uint16_t width, uint16_t height, uint8_t fps)
{
	if (mCameraID >= 0)
		mWaitingForCameraID = id != mCameraID;

	if (mMode >= 0)
		mWaitingForMode = true;

	if (mFrameRate_fps > 0)
		mWaitingForFrameRate = mFrameRate_fps != fps;

	if (mLapseInterval_ms > 0)
		mWaitingForInterval = true;

	if (mImageWidth < 0)
		mImageWidth = width;

	if (mImageHeight < 0)
		mImageHeight = height;

	mWaitingForResolution = (mImageWidth != width) || (mImageHeight != height);

	if (mWaitingForMode)
		sendSetMode(static_cast<uint8_t>(mMode));

	if (mWaitingForResolution)
		sendSetImageSize(mImageWidth, mImageHeight);

	if (mWaitingForCameraID)
		sendSetCameraId(mCameraID);

	if (mWaitingForFrameRate)
		sendSetFrameRate_fps(mFrameRate_fps);

	if (mWaitingForInterval)
		sendSetLapseInterval_ms(mLapseInterval_ms);

	mWaitingForConfiguration = false;
}

void cAxisCommunications_Configure_Remote::onCurrentState(bool valid, uint8_t active_id, uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id)
{
	if (mCameraID >= 0)
		mWaitingForCameraID = active_id != mCameraID;

	if (mMode >= 0)
		mWaitingForMode = true;

	if (mFrameRate_fps > 0)
		mWaitingForFrameRate = mFrameRate_fps != fps;

	if (mLapseInterval_ms > 0)
		mWaitingForInterval = true;

	if (mImageWidth < 0)
		mImageWidth = width;

	if (mImageHeight < 0)
		mImageHeight = height;

	mWaitingForResolution = (mImageWidth != width) || (mImageHeight != height);

	if (mWaitingForMode)
		sendSetMode(static_cast<uint8_t>(mMode));

	if (mWaitingForResolution)
		sendSetImageSize(mImageWidth, mImageHeight);

	if (mWaitingForCameraID)
		sendSetCameraId(mCameraID);

	if (mWaitingForFrameRate)
		sendSetFrameRate_fps(mFrameRate_fps);

	if (mWaitingForInterval)
		sendSetLapseInterval_ms(mLapseInterval_ms);

	mWaitingForConfiguration = false;
}

void cAxisCommunications_Configure_Remote::onCurrentState(bool valid, uint8_t mode, uint8_t active_id, uint16_t width, uint16_t height, uint8_t fps, uint32_t interval_ms,
	uint8_t min_id, uint8_t max_id, std::optional<double> min_fps, std::optional<double> max_fps)
{
	if (mCameraID >= 0)
		mWaitingForCameraID = active_id != mCameraID;

	if (mMode >= 0)
		mWaitingForMode = mode != mMode;

	if (mFrameRate_fps > 0)
		mWaitingForFrameRate = mFrameRate_fps != fps;

	if (mLapseInterval_ms > 0)
		mWaitingForInterval = mLapseInterval_ms != interval_ms;

	if (mImageWidth < 0)
		mImageWidth = width;

	if (mImageHeight < 0)
		mImageHeight = height;

	mWaitingForResolution = (mImageWidth != width) || (mImageHeight != height);

	if (mWaitingForMode)
		sendSetMode(static_cast<uint8_t>(mMode));

	if (mWaitingForResolution)
		sendSetImageSize(mImageWidth, mImageHeight);

	if (mWaitingForCameraID)
		sendSetCameraId(mCameraID);

	if (mWaitingForFrameRate)
		sendSetFrameRate_fps(mFrameRate_fps);

	if (mWaitingForInterval)
		sendSetLapseInterval_ms(mLapseInterval_ms);

	mWaitingForConfiguration = false;
}

void cAxisCommunications_Configure_Remote::onConnect()
{
	sendQueryState();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

	mConnected = true;
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
	return cAxisCommunicationsExperimentHelper_TakePhoto::configure(stateDoc);
}

cExperimentState::eRESULT cAxisCommunications_TakePhoto_Remote::finished()
{
	return mResult;
}

void cAxisCommunications_TakePhoto_Remote::onTakePhotoReply(bool error)
{
	mResult = cExperimentState::eRESULT::DONE;
};

void cAxisCommunications_TakePhoto_Remote::onConnect()
{
	sendTakePhoto(mUpdateView);

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}



