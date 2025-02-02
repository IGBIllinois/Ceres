
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

	mHasReferenceState = false;

	try
	{
		if (stateDoc.contains("integration time (sec)"))
		{
			mDesiredIntegrationTime_sec = stateDoc["integration time (us)"];
			mHasReferenceState = true;
		}

		if (stateDoc.contains("max integration time (sec)"))
		{
			mDesiredMaxIntegrationTime_sec = stateDoc["max integration time (us)"];
			mHasReferenceState = true;
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
	if (mHasReferenceState)
	{
		msg = "Updating reference parameters and collecting a reference point...";
	}
	else
	{
		msg = "Waiting for GPS to collect reference point...";
	}
	return msg;
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




