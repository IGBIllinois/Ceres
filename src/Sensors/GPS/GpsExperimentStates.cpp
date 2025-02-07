
#include "GpsExperimentStates.hpp"
#include "GpsPropertyPage_Remote.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>

const long long NETWORK_DELAY_MS = 500;

/*******************************************************************/
/**       Base Class for Remote GPS Experiment States          **/
/*******************************************************************/
cGpsExperimentState_Remote::cGpsExperimentState_Remote
	(const std::string& hostname, uint16_t port, const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: cExperimentStateRemoteInterface(parent)
{
	mHostname = hostname;
	mPort = port;
	mLocalIpAddress = localIpAddress;
	mUse_IpV6 = use_IpV6;
}

cGpsExperimentState_Remote::~cGpsExperimentState_Remote()
{}

bool cGpsExperimentState_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return false;

	return openConnection();
}

void cGpsExperimentState_Remote::cleanup()
{
	closeConnection();
	destroy();
}

bool cGpsExperimentState_Remote::recording()
{
	return false;
}


/*******************************************************************/
/**   GPS Experiment States to Control Reference Acquisition      **/
/*******************************************************************/
cGpsReferenceAcquisition_Remote::cGpsReferenceAcquisition_Remote(const std::string& hostname, uint16_t port,
	const std::string& localIpAddress, bool use_IpV6, QObject* parent)
	: 
	cGpsExperimentState_Remote(hostname, port, localIpAddress, use_IpV6, parent), cGpsPropertiesNetEncoder(255)
{}

cGpsReferenceAcquisition_Remote::~cGpsReferenceAcquisition_Remote()
{}

bool cGpsReferenceAcquisition_Remote::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mHasReferenceParameters = false;

	try
	{
		if (stateDoc.contains("min integration time (sec)"))
		{
			mDesiredMinIntegrationTime_sec = stateDoc["min integration time (us)"];
			mHasReferenceParameters = true;
		}

		if (stateDoc.contains("max integration time (sec)"))
		{
			mDesiredMaxIntegrationTime_sec = stateDoc["max integration time (us)"];
			mHasReferenceParameters = true;
		}

		if (stateDoc.contains("error threshold (mm)"))
		{
			mDesiredRefErrorThreshold_mm = stateDoc["error threshold (mm)"];
			mHasReferenceParameters = true;
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "GPS Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "GPS Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "GPS Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cGpsReferenceAcquisition_Remote::run() {}
void cGpsReferenceAcquisition_Remote::pause() {}
void cGpsReferenceAcquisition_Remote::stop() {}

cExperimentState::eRESULT cGpsReferenceAcquisition_Remote::finished()
{
	if (mState == eSTATE::ERROR)
		return eRESULT::ABORT;

	if (mState == eSTATE::COMPLETE)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}

QString cGpsReferenceAcquisition_Remote::getStatusStr()
{
	QString msg;
	if (mHasReferenceParameters)
	{
		msg = "Updating reference parameters and collecting a reference point...";
	}
	else
	{
		msg = "Waiting for GPS to collect reference point...";
	}
	return msg;
}

void cGpsReferenceAcquisition_Remote::onReferenceParameters(bool valid, uint16_t min_integration_time_sec,
	uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm)
{
	mCurrentMinIntegrationTime_sec = min_integration_time_sec;
	mCurrentMaxIntegrationTime_sec = max_integration_time_sec;
	mCurrentRefErrorThreshold_mm = ref_error_threshold_mm;

	if (mHasReferenceParameters)
	{
		std::uint16_t min_time_sec = mDesiredMinIntegrationTime_sec.has_value() ? mDesiredMinIntegrationTime_sec.value() : mCurrentMinIntegrationTime_sec;
		std::uint32_t max_time_sec = mDesiredMaxIntegrationTime_sec.has_value() ? mDesiredMaxIntegrationTime_sec.value() : mCurrentMaxIntegrationTime_sec;
		std::uint32_t threshold_mm = mDesiredRefErrorThreshold_mm.has_value() ? mDesiredRefErrorThreshold_mm.value() : mCurrentRefErrorThreshold_mm;

		sendReferenceParameters(min_time_sec, max_time_sec, threshold_mm);

		// Sleep for 250 milliseconds
		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

		mHasReferenceParameters = false;
		mState = eSTATE::WAIT_FOR_STATE_UPDATE;
		sendQueryReferenceParameters();

		// Sleep for 250 milliseconds
		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

		return;
	}

	mState = eSTATE::WAIT_FOR_REFERENCE;
	sendCalcReference();;
}

void cGpsReferenceAcquisition_Remote::onReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
	double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{
}

void cGpsReferenceAcquisition_Remote::onReferenceCommandReply(eReferenceReply reply)
{
	switch (reply)
	{
	case eReferenceReply::GOOD:
		mState = eSTATE::COMPLETE;
		break;
	case eReferenceReply::ABORTED:
	case eReferenceReply::FAILED:
		mState = eSTATE::ERROR;
		break;
	}
}


void cGpsReferenceAcquisition_Remote::onConnect()
{
	mState = eSTATE::WAIT_FOR_STATE;
	sendQueryReferenceParameters();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}

void cGpsReferenceAcquisition_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cGpsPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cGpsReferenceAcquisition_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}




