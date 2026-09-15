
#include "GpsExperimentStates_Local.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>

const long long NETWORK_DELAY_MS = 500;

/*******************************************************************/
/**       Base Class for Local GPS Experiment States          **/
/*******************************************************************/
cGpsExperimentState_Local::cGpsExperimentState_Local(cGpsModel* pModel, QObject* parent)
	: cExperimentState(parent), mpModel(pModel)
{
}

cGpsExperimentState_Local::~cGpsExperimentState_Local()
{}

bool cGpsExperimentState_Local::initialize()
{
	return true;
}

void cGpsExperimentState_Local::cleanup()
{}


/*******************************************************************/
/**   GPS Experiment States to Control Reference Acquisition      **/
/*******************************************************************/
cGpsReferenceAcquisition_Local::cGpsReferenceAcquisition_Local(cGpsModel* pModel, QObject* parent)
	: 
	cGpsExperimentState_Local(pModel, parent)
{}

cGpsReferenceAcquisition_Local::~cGpsReferenceAcquisition_Local()
{}

bool cGpsReferenceAcquisition_Local::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mHasReferenceParameters = false;

	try
	{
		if (stateDoc.contains("min integration time (sec)"))
		{
			mDesiredMinIntegrationTime_sec = stateDoc["min integration time (sec)"];
			mHasReferenceParameters = true;
		}

		if (stateDoc.contains("max integration time (sec)"))
		{
			mDesiredMaxIntegrationTime_sec = stateDoc["max integration time (sec)"];
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

	if (mDesiredMinIntegrationTime_sec.has_value() && mDesiredMaxIntegrationTime_sec.has_value())
	{
		auto minTime = mDesiredMinIntegrationTime_sec.value();
		auto maxTime = mDesiredMaxIntegrationTime_sec.value();

		if (maxTime < minTime)
		{
			mDesiredMinIntegrationTime_sec = maxTime;
			mDesiredMaxIntegrationTime_sec = minTime;
		}
	}

	if (mDesiredRefErrorThreshold_mm.has_value())
	{
		if (mDesiredRefErrorThreshold_mm.value() < 0)
			mDesiredRefErrorThreshold_mm.reset();
	}

	return true;
}

void cGpsReferenceAcquisition_Local::run() {}
void cGpsReferenceAcquisition_Local::pause() {}
void cGpsReferenceAcquisition_Local::stop() {}

cExperimentState::eRESULT cGpsReferenceAcquisition_Local::finished()
{
	if (mState == eSTATE::ERROR)
		return eRESULT::ABORT;

	if (mState == eSTATE::COMPLETE)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}

QString cGpsReferenceAcquisition_Local::getStatusStr()
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

void referenceParametersUpdated(int min_integration_time_sec, int max_integration_time_sec, int ref_error_threshold_mm);

void cGpsReferenceAcquisition_Local::onReferenceComplete()
{}

void cGpsReferenceAcquisition_Local::referenceStateUpdated(::gps::eReferenceState state)
{
	if (mState == eSTATE::ERROR)
		return;

	switch (state)
	{
		// The reference position has completed with the position tolerence
	case ::gps::eReferenceState::COMPLETE_GOOD:
		mState = eSTATE::COMPLETE;
		break;
		// The reference position has completed but outside the position tolerence
	case ::gps::eReferenceState::COMPLETE_FAILED:
		mState = eSTATE::COMPLETE;
		break;
		// The reference position has been stopped!
	case ::gps::eReferenceState::ABORT:
	{
		QString msg = "GPS failed to collect reference position!\n";
		msg += "Possible causes include: stuck in CONNECTED state, or poor satellite coverage.";
		emit errorUpdate(msg);
		mState = eSTATE::ERROR;
		break;
	}
	}
}

void cGpsReferenceAcquisition_Local::referenceParametersUpdated(int min_integration_time_sec, int max_integration_time_sec, int ref_error_threshold_mm)
{}


/*
void cGpsReferenceAcquisition_Local::onReferenceParameters(bool valid, uint16_t min_integration_time_sec,
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

void cGpsReferenceAcquisition_Local::onReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
	double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{
}

void cGpsReferenceAcquisition_Local::onReferenceCommandReply(eReferenceReply reply)
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


void cGpsReferenceAcquisition_Local::onConnect()
{
	mState = eSTATE::WAIT_FOR_STATE;
	sendQueryReferenceParameters();

	// Sleep for 250 milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));
}

void cGpsReferenceAcquisition_Local::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cGpsPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cGpsReferenceAcquisition_Local::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}
*/



