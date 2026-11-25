
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
cHySpexCamera_ExperimentState_Remote::cHySpexCamera_ExperimentState_Remote(std::string_view camera_name, const std::string& hostname, 
																uint16_t port, const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: 
	cExperimentState(parent), mCameraName(camera_name), cExperimentStateRemoteInterface()
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
cHySpexCamera_ShutterCtrl_Remote::cHySpexCamera_ShutterCtrl_Remote(std::string_view camera_name, const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, eShutterState desired_state, QObject* parent)
	:
	cHySpexCamera_ExperimentState_Remote(camera_name, hostname, port, localIpAddress, use_IpV6, parent), cHySpexCamera_PropertiesNetEncoder(255),
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

	mShutterTimer.start();
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
cHySpexCamera_CloseShutter_Remote::cHySpexCamera_CloseShutter_Remote(std::string_view camera_name, const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_ShutterCtrl_Remote(camera_name, hostname, port, localIpAddress, use_IpV6, eShutterState::CLOSED, parent)
{}

QString cHySpexCamera_CloseShutter_Remote::getStatusStr()
{
	if (mCameraName.empty())
		return "Closing Shutter...";

	QString msg = QString::fromStdString(mCameraName);
	msg += ": Closing Shutter...";
	return msg;
}

/*** Experimental State to Open Shutter ***/
cHySpexCamera_OpenShutter_Remote::cHySpexCamera_OpenShutter_Remote(std::string_view camera_name, const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_ShutterCtrl_Remote(camera_name, hostname, port, localIpAddress, use_IpV6, eShutterState::OPEN, parent)
{}

QString cHySpexCamera_OpenShutter_Remote::getStatusStr()
{
	if (mCameraName.empty())
		return "Opening Shutter...";

	QString msg = QString::fromStdString(mCameraName);
	msg += ": Opening Shutter...";
	return msg;
}


/*******************************************************************/
/**       HySpex Experiment States to Control Acquisition         **/
/*******************************************************************/
cHySpexCamera_Acquisition_Remote::cHySpexCamera_Acquisition_Remote(std::string_view camera_name, const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: 
	cHySpexCamera_ExperimentState_Remote(camera_name, hostname, port, localIpAddress, use_IpV6, parent), cHySpexCamera_PropertiesNetEncoder(255)
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
cHySpexCamera_AcqParameters_Remote::cHySpexCamera_AcqParameters_Remote(std::string_view camera_name, const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_Acquisition_Remote(camera_name, hostname, port, localIpAddress, use_IpV6, parent)
{}

QString cHySpexCamera_AcqParameters_Remote::getStatusStr()
{
	if (mCameraName.empty())
		return "Updating acquisition parameters...";

	QString msg = QString::fromStdString(mCameraName);
	msg += ": Updating acquisition parameters...";
	return msg;
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
cHySpexCamera_Background_Remote::cHySpexCamera_Background_Remote(std::string_view camera_name, const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	:
	cHySpexCamera_Acquisition_Remote(camera_name, hostname, port, localIpAddress, use_IpV6, parent)
{
	mCurrentStateCheck.interval_sec(5);
	mStartBackgroundCheck.interval_sec(5);
	mBackgroundStatusCheck.interval_sec(10);
}


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

	if (mCameraName.empty())
		msg = "";
	else
	{
		msg = QString::fromStdString(mCameraName);
		msg += ": ";
	}

	if (mHasAcquisitionState)
	{
		msg += "Updating acquisition parameters and collecting a background image...";
	}
	else
	{
		msg += "Waiting for camera to collect background image...";
	}
	return msg;
}

void cHySpexCamera_Background_Remote::run()
{
	cHySpexCamera_Acquisition_Remote::run();

	if (mCurrentStateCheck.elapsed())
		sendQueryStateMessage();

	if (mStartBackgroundCheck.elapsed())
		sendQueryBackgroundStateMessage();

	if (mBackgroundStatusCheck.elapsed())
		sendQueryBackgroundStateMessage();
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
	mCurrentStateCheck.stop();

	mCurrentAverageFrames = average_frames;
	mCurrentFramePeriod_us = frame_period_us;
	mCurrentIntegrationTime_us = integration_time_us;
	mCurrentNumBackgrounds = num_backgrounds;

	double background_time_ms = average_frames * (frame_period_us / 1'000.0) * num_backgrounds;

	mStartBackgroundCheck.interval_ms(static_cast<uint32_t>(2 * background_time_ms));

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

		mCurrentStateCheck.start();

		return;
	}

	if ((mDesiredNumBackgrounds.has_value()) && (mDesiredNumBackgrounds.value() != mCurrentNumBackgrounds))
	{
		sendNumOfBackgroundsMessage(mDesiredNumBackgrounds.value());
	}

	mState = eSTATE::WAIT_FOR_BACKGROUND;
	sendCalcBackgroundMessage();
	mStartBackgroundCheck.start();
}

void cHySpexCamera_Background_Remote::onCommandReplyMessage(eCommandReply reply)
{
	switch (reply)
	{
	case eCommandReply::CALC_BACKGROUND:
		mStartBackgroundCheck.stop();
		mBackgroundStatusCheck.start();
		break;

	case eCommandReply::STOP_BACKGROUND:
		mStartBackgroundCheck.stop();
		mBackgroundStatusCheck.stop();
		mState = eSTATE::ERROR;
		break;

	case eCommandReply::UNKNOWN:
	default:
		break;
	}
}

void cHySpexCamera_Background_Remote::onBackgroundReplyMessage(eBackgroundReply reply)
{
	if (mState == eSTATE::ERROR)
		return;

	switch (reply)
	{
	case eBackgroundReply::GOOD:
		mState = eSTATE::COMPLETE;
		break;
	case eBackgroundReply::PENDING:
		mState = eSTATE::WAIT_FOR_BACKGROUND;
		break;
	default:
	{
		QString msg;

		if (mCameraName.empty())
			msg = "HySpex Compute Background Failed!";
		else
		{
			msg = QString::fromStdString(mCameraName);
			msg += ": HySpex Compute Background Failed!";
		}

		emit errorUpdate(msg);

		mState = eSTATE::ERROR;
	}
	}
}

void cHySpexCamera_Background_Remote::onConnect()
{
	cHySpexCamera_Acquisition_Remote::onConnect();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

	mState = eSTATE::WAIT_FOR_STATE;
	mCurrentStateCheck.start();
}

