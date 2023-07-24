
#include "HySpexExperimentStates.hpp"
#include "HySpexVNIR_3000N_PropertyPage_Remote.hpp"
#include "HySpexSWIR_384_PropertyPage_Remote.hpp"


/*******************************************************************/
/**           Base Class for HySpex Experiment States             **/
/*******************************************************************/

cHySpexCamera_Properties_Remote::cHySpexCamera_Properties_Remote()
	: cExperimentStateRemoteInterface()
{}

cHySpexCamera_Properties_Remote::~cHySpexCamera_Properties_Remote()
{}

bool cHySpexCamera_Properties_Remote::recording()
{
	return false;
}

void cHySpexCamera_Properties_Remote::configure(const nlohmann::json& stateDoc)
{
	mHasAcquisitionState = false;

	if (stateDoc.contains("average frames"))
	{
		mDesiredAverageFrames = stateDoc["average frames"];
		mHasAcquisitionState = true;
	}

	if (stateDoc.contains("frame period (us)"))
	{
		mDesiredFramePeriod_us = stateDoc["frame period(us)"];
		mHasAcquisitionState = true;
	}

	if (stateDoc.contains("integration time (us)"))
	{
		mDesiredIntegrationTime_us = stateDoc["integration time (us)"];
		mHasAcquisitionState = true;
	}

	if (stateDoc.contains("number of backgrounds"))
	{
		mDesiredNumBackgrounds = stateDoc["number of backgrounds"];
	}

	mDoBackground = stateDoc["background"];
}

void cHySpexCamera_Properties_Remote::cleanup()
{
	closeConnection();
	destroy();
}

void cHySpexCamera_Properties_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return;

	openConnection();
}

void cHySpexCamera_Properties_Remote::run()
{}

void cHySpexCamera_Properties_Remote::pause()
{}

void cHySpexCamera_Properties_Remote::stop()
{}

cExperimentState::eRESULT cHySpexCamera_Properties_Remote::finished()
{
	if (mState == eSTATE::ERROR)
		return eRESULT::ABORT;

	if (mState == eSTATE::COMPLETE)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}


/*******************************************************************/
/**            The HySpex VNIR-3000N Experiment State             **/
/*******************************************************************/

cHySpexVNIR_3000N_Properties_Remote::cHySpexVNIR_3000N_Properties_Remote(cHySpexVNIR_3000N_PropertyPage_Remote* property_page)
	: cHySpexCamera_Properties_Remote(), cHySpexVNIR_3000N_PropertiesNetEncoder(255)
{
	mHostname = property_page->getHostname();
	mPort = property_page->getPort();
	mLocalIpAddress = property_page->getLocalIpAddress();
	mUse_IpV6 = property_page->usingIpV6();
}

cHySpexVNIR_3000N_Properties_Remote::~cHySpexVNIR_3000N_Properties_Remote()
{}

QString cHySpexVNIR_3000N_Properties_Remote::getStatusStr()
{
	QString msg;
	if (mHasAcquisitionState && mDoBackground)
	{
		msg = "Updating VNIR-3000N acquisition parameters and collecting a background image...";
	}
	else if (mHasAcquisitionState)
	{
		msg = "Updating VNIR-3000N acquisition parameters...";
	}
	else if (mDoBackground)
	{
		msg = "Waiting for VNIR-3000N to collect background image...";
	}
	return msg;
}

void cHySpexVNIR_3000N_Properties_Remote::onCurrentState(bool valid, std::uint16_t average_frames,
	std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
	std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
	std::uint32_t num_backgrounds, const std::string& lens_name)
{
	mCurrentAverageFrames = average_frames;
	mCurrentFramePeriod_us = frame_period_us;
	mCurrentIntegrationTime_us = integration_time_us;
	mCurrentNumBackgrounds = num_backgrounds;

	if (mHasAcquisitionState)
	{
		sendAcquisitionParameters(mDesiredAverageFrames, mDesiredFramePeriod_us, mDesiredIntegrationTime_us);
		mHasAcquisitionState = false;
		mState = eSTATE::WAIT_FOR_STATE_UPDATE;
		sendQueryState();
		return;
	}

	if (mDoBackground)
	{
		if (mDesiredNumBackgrounds != mCurrentNumBackgrounds)
			sendNumOfBackgrounds(mDesiredNumBackgrounds);
		mState = eSTATE::WAIT_FOR_BACKGROUND;
		sendCalcBackground();
		return;
	}

	mState = eSTATE::COMPLETE;
}

void cHySpexVNIR_3000N_Properties_Remote::onLensNames(const std::vector<std::string>& names)
{}

void cHySpexVNIR_3000N_Properties_Remote::onBackgroundReply(eBackgroundReply reply)
{
	if (reply == eBackgroundReply::GOOD)
		mState = eSTATE::COMPLETE;
	else
		mState = eSTATE::ERROR;
}

void cHySpexVNIR_3000N_Properties_Remote::onConnect()
{
	mState = eSTATE::WAIT_FOR_STATE;
	sendQueryState();
}

void cHySpexVNIR_3000N_Properties_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexVNIR_3000N_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexVNIR_3000N_Properties_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}



/*******************************************************************/
/**             The HySpex SWIR-384 Experiment State              **/
/*******************************************************************/

cHySpexSWIR_384_Properties_Remote::cHySpexSWIR_384_Properties_Remote(cHySpexSWIR_384_PropertyPage_Remote* property_page)
	: cHySpexCamera_Properties_Remote(), cHySpexSWIR_384_PropertiesNetEncoder(255)
{
	mHostname = property_page->getHostname();
	mPort = property_page->getPort();
	mLocalIpAddress = property_page->getLocalIpAddress();
	mUse_IpV6 = property_page->usingIpV6();
}

cHySpexSWIR_384_Properties_Remote::~cHySpexSWIR_384_Properties_Remote()
{}

QString cHySpexSWIR_384_Properties_Remote::getStatusStr()
{
	QString msg;
	if (mHasAcquisitionState && mDoBackground)
	{
		msg = "Updating SWIR-384 acquisition parameters and collecting a background image...";
	}
	else if (mHasAcquisitionState)
	{
		msg = "Updating SWIR-384 acquisition parameters...";
	}
	else if (mDoBackground)
	{
		msg = "Waiting for SWIR-384 to collect background image...";
	}
	return msg;
}

void cHySpexSWIR_384_Properties_Remote::onCurrentState(bool valid, std::uint16_t average_frames,
	std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
	std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
	std::uint32_t num_backgrounds, const std::string& lens_name)
{
	mCurrentAverageFrames = average_frames;
	mCurrentFramePeriod_us = frame_period_us;
	mCurrentIntegrationTime_us = integration_time_us;
	mCurrentNumBackgrounds = num_backgrounds;

	if (mHasAcquisitionState)
	{
		sendAcquisitionParameters(mDesiredAverageFrames, mDesiredFramePeriod_us, mDesiredIntegrationTime_us);
		mHasAcquisitionState = false;
		mState = eSTATE::WAIT_FOR_STATE_UPDATE;
		sendQueryState();
		return;
	}

	if (mDoBackground)
	{
		if (mDesiredNumBackgrounds != mCurrentNumBackgrounds)
			sendNumOfBackgrounds(mDesiredNumBackgrounds);
		mState = eSTATE::WAIT_FOR_BACKGROUND;
		sendCalcBackground();
		return;
	}

	mState = eSTATE::COMPLETE;
}

void cHySpexSWIR_384_Properties_Remote::onLensNames(const std::vector<std::string>& names)
{}

void cHySpexSWIR_384_Properties_Remote::onBackgroundReply(eBackgroundReply reply)
{
	if (reply == eBackgroundReply::GOOD)
		mState = eSTATE::COMPLETE;
	else
		mState = eSTATE::ERROR;
}

void cHySpexSWIR_384_Properties_Remote::onConnect()
{
	mState = eSTATE::WAIT_FOR_STATE;
	sendQueryState();
}

void cHySpexSWIR_384_Properties_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexSWIR_384_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexSWIR_384_Properties_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}

