
#include "HySpexExperimentStates_Remote.hpp"
#include "HySpexVNIR_3000N_PropertyPage_Remote.hpp"
#include "HySpexSWIR_384_PropertyPage_Remote.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>

const long long NETWORK_DELAY_MS = 500;

/*******************************************************************/
/**       Base Class for Remote HySpex Experiment States          **/
/*******************************************************************/
cHySpexCamera_ExperimentState_Remote::cHySpexCamera_ExperimentState_Remote
	(const std::string& hostname, uint16_t port, const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: cExperimentState(parent), cExperimentStateRemoteInterface(/*parent*/)
{
	mHostname = hostname;
	mPort = port;
	mLocalIpAddress = localIpAddress;
	mUse_IpV6 = use_IpV6;
}

cHySpexCamera_ExperimentState_Remote::~cHySpexCamera_ExperimentState_Remote()
{}

bool cHySpexCamera_ExperimentState_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return false;

	return openConnection();
}

void cHySpexCamera_ExperimentState_Remote::cleanup()
{
	closeConnection();
	destroy();
}

bool cHySpexCamera_ExperimentState_Remote::recording()
{
	return false;
}


/*******************************************************************/
/**         HySpex Experiment States to Control Shutter           **/
/*******************************************************************/
cHySpexCamera_ShutterCtrl_Remote::cHySpexCamera_ShutterCtrl_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, eShutterState desired_state, QObject* parent)
	:
	cHySpexCamera_ExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent), cHySpexCamera_PropertiesNetEncoder(255),
	mDesiredState(desired_state)
{
	mShutterTimer.interval_sec(5);
	mShutterTimer.stop();
}

bool cHySpexCamera_ShutterCtrl_Remote::configure(const nlohmann::json& stateDoc)
{
	return true;
}

void cHySpexCamera_ShutterCtrl_Remote::run() 
{
	if (mShutterTimer.elapsed())
	{
		sendQueryShutterStateMessage();

		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

		mShutterTimer.reset();
	}
}

void cHySpexCamera_ShutterCtrl_Remote::pause() {}
void cHySpexCamera_ShutterCtrl_Remote::stop() {}

cExperimentState::eRESULT cHySpexCamera_ShutterCtrl_Remote::finished()
{
	if (mShutterState == eShutterState::ERROR)
		return eRESULT::ABORT;

	if (mShutterState == mDesiredState)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}

void cHySpexCamera_ShutterCtrl_Remote::onShutterStateMessage(eShutterState state)
{
	mShutterState = state;

	if ((mShutterState == eShutterState::ERROR) ||
		(mShutterState == mDesiredState))
			return;

	if (mDesiredState == eShutterState::CLOSED)
	{
		sendCloseShutterMessage();

		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

		sendQueryShutterStateMessage();
	}

	if (mDesiredState == eShutterState::OPEN)
	{
		sendOpenShutterMessage();

		// Sleep for 250 milliseconds
		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

		sendQueryShutterStateMessage();
	}
}

void cHySpexCamera_ShutterCtrl_Remote::onConnect()
{
	sendQueryShutterStateMessage();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

	mShutterTimer.reset();
}

void cHySpexCamera_ShutterCtrl_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexCamera_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexCamera_ShutterCtrl_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}

/*** Experimental State to Close Shutter ***/
cHySpexCamera_CloseShutter_Remote::cHySpexCamera_CloseShutter_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_ShutterCtrl_Remote(hostname, port, localIpAddress, use_IpV6, eShutterState::CLOSED, parent)
{}

QString cHySpexCamera_CloseShutter_Remote::getStatusStr()
{
	return "Closing Shutter...";
}

/*** Experimental State to Open Shutter ***/
cHySpexCamera_OpenShutter_Remote::cHySpexCamera_OpenShutter_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_ShutterCtrl_Remote(hostname, port, localIpAddress, use_IpV6, eShutterState::OPEN, parent)
{}

QString cHySpexCamera_OpenShutter_Remote::getStatusStr()
{
	return "Opening Shutter...";
}


/*******************************************************************/
/**       HySpex Experiment States to Control Acquisition         **/
/*******************************************************************/
cHySpexCamera_Acquisition_Remote::cHySpexCamera_Acquisition_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: 
	cHySpexCamera_ExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent), cHySpexCamera_PropertiesNetEncoder(255)
{}

cHySpexCamera_Acquisition_Remote::~cHySpexCamera_Acquisition_Remote()
{}

bool cHySpexCamera_Acquisition_Remote::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mHasAcquisitionState = false;

	try
	{
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
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cHySpexCamera_Acquisition_Remote::run() {}
void cHySpexCamera_Acquisition_Remote::pause() {}
void cHySpexCamera_Acquisition_Remote::stop() {}

void cHySpexCamera_Acquisition_Remote::onConnect()
{
	sendQueryStateMessage();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}

void cHySpexCamera_Acquisition_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexCamera_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexCamera_Acquisition_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}


/*** Experimental State to Adjust Acquisition Parameters ***/
cHySpexCamera_AcqParameters_Remote::cHySpexCamera_AcqParameters_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_Acquisition_Remote(hostname, port, localIpAddress, use_IpV6, parent) 
{}

QString cHySpexCamera_AcqParameters_Remote::getStatusStr()
{
	return "Updating acquisition parameters...";
}

bool cHySpexCamera_AcqParameters_Remote::initialize()
{
	if (!mHasAcquisitionState)
	{
		return true;
	}

	return cHySpexCamera_Acquisition_Remote::initialize();
}

cExperimentState::eRESULT cHySpexCamera_AcqParameters_Remote::finished()
{
	if (!mHasAcquisitionState)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}

void cHySpexCamera_AcqParameters_Remote::onCurrentStateMessage(bool valid, std::uint16_t average_frames,
	std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
	std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
	std::uint32_t num_backgrounds, const std::string& lens_name)
{
	mCurrentAverageFrames = average_frames;
	mCurrentFramePeriod_us = frame_period_us;
	mCurrentIntegrationTime_us = integration_time_us;

	if (mHasAcquisitionState)
	{
		std::uint16_t averageFrames = mDesiredAverageFrames.has_value() ? mDesiredAverageFrames.value() : mCurrentAverageFrames;
		std::uint32_t framePeriod_us = mDesiredFramePeriod_us.has_value() ? mDesiredFramePeriod_us.value() : mCurrentFramePeriod_us;
		std::uint32_t integrationTime_us = mDesiredIntegrationTime_us.has_value() ? mDesiredIntegrationTime_us.value() : mCurrentIntegrationTime_us;

		sendAcquisitionParametersMessage(averageFrames, framePeriod_us, integrationTime_us);
		mHasAcquisitionState = false;
		return;
	}
}


/*** Experimental State to Do Background Measurement ***/
cHySpexCamera_Background_Remote::cHySpexCamera_Background_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_Acquisition_Remote(hostname, port, localIpAddress, use_IpV6, parent)
{}


bool cHySpexCamera_Background_Remote::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("number of backgrounds"))
		{
			mDesiredNumBackgrounds = static_cast<std::uint32_t>(stateDoc["number of backgrounds"]);
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return cHySpexCamera_Acquisition_Remote::configure(stateDoc);
}

QString cHySpexCamera_Background_Remote::getStatusStr()
{
	QString msg;
	if (mHasAcquisitionState)
	{
		msg = "Updating acquisition parameters and collecting a background image...";
	}
	else
	{
		msg = "Waiting for camera to collect background image...";
	}
	return msg;
}

cExperimentState::eRESULT cHySpexCamera_Background_Remote::finished()
{
	if (mState == eSTATE::ERROR)
		return eRESULT::ABORT;

	if (mState == eSTATE::COMPLETE)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}

void cHySpexCamera_Background_Remote::onCurrentStateMessage(bool valid, std::uint16_t average_frames,
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
		std::uint16_t averageFrames = mDesiredAverageFrames.has_value() ? mDesiredAverageFrames.value() : mCurrentAverageFrames;
		std::uint32_t framePeriod_us = mDesiredFramePeriod_us.has_value() ? mDesiredFramePeriod_us.value() : mCurrentFramePeriod_us;
		std::uint32_t integrationTime_us = mDesiredIntegrationTime_us.has_value() ? mDesiredIntegrationTime_us.value() : mCurrentIntegrationTime_us;

		sendAcquisitionParametersMessage(averageFrames, framePeriod_us, integrationTime_us);

		// Sleep for 250 milliseconds
		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

		mHasAcquisitionState = false;
		mState = eSTATE::WAIT_FOR_STATE_UPDATE;
		sendQueryStateMessage();

		// Sleep for 250 milliseconds
		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

		return;
	}

	if ((mDesiredNumBackgrounds.has_value()) && (mDesiredNumBackgrounds.value() != mCurrentNumBackgrounds))
	{
		sendNumOfBackgroundsMessage(mDesiredNumBackgrounds.value());
	}

	mState = eSTATE::WAIT_FOR_BACKGROUND;
	sendCalcBackgroundMessage();
}

void cHySpexCamera_Background_Remote::onCommandReplyMessage(eCommandReply reply)
{}

void cHySpexCamera_Background_Remote::onBackgroundReplyMessage(eBackgroundReply reply)
{
	switch (reply)
	{
	case eBackgroundReply::GOOD:
		mState = eSTATE::COMPLETE;
		break;
	case eBackgroundReply::PENDING:
		mState = eSTATE::WAIT_FOR_BACKGROUND;
		break;
	default:
		mState = eSTATE::ERROR;
	}
}

void cHySpexCamera_Background_Remote::onConnect()
{
	mState = eSTATE::WAIT_FOR_STATE;
	cHySpexCamera_Acquisition_Remote::onConnect();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}


































#if 0
cHySpexCamera_Properties_Remote::cHySpexCamera_Properties_Remote()
	: cExperimentStateRemoteInterface()
{}

cHySpexCamera_Properties_Remote::~cHySpexCamera_Properties_Remote()
{}

bool cHySpexCamera_Properties_Remote::recording()
{
	return false;
}

bool cHySpexCamera_Properties_Remote::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mHasAcquisitionState = false;

	try
	{
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

		mDoBackground = true; // stateDoc["background"];
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cHySpexCamera_Properties_Remote::cleanup()
{
	closeConnection();
	destroy();
}

bool cHySpexCamera_Properties_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return false;

	return openConnection();
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

void cHySpexVNIR_3000N_Properties_Remote::onShutterState(eShutterState state)
{}

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

void cHySpexSWIR_384_Properties_Remote::onShutterState(eShutterState state)
{}

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
#endif
