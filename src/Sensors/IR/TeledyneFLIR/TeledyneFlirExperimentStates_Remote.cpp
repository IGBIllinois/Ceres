
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
	return cTeledyneFlirExperimentHelper_Configure::configure(stateDoc);
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
	return cTeledyneFlirExperimentHelper_TakePhoto::configure(stateDoc);
}

cExperimentState::eRESULT cTeledyneFlirCamera_TakePhoto_Remote::finished()
{
	return mResult;
}

void cTeledyneFlirCamera_TakePhoto_Remote::onTakePhotoReplyMessage(bool error)
{
	--mNumOfPhotos;

	if (mNumOfPhotos < 1)
		mResult = cExperimentState::eRESULT::DONE;
	else
	{
		sendTakePhotoMessage(mUpdateView, true);

		// Sleep for 250 milliseconds
		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
	}
};

void cTeledyneFlirCamera_TakePhoto_Remote::onConnect()
{
	sendTakePhotoMessage(mUpdateView, true);

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}



