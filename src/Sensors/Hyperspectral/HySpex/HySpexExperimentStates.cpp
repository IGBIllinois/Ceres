
#include "HySpexExperimentStates.hpp"
#include "HySpexVNIR_3000N_PropertyPage_Remote.hpp"

#include <QThread>


cHySpexVNIR_3000N_Properties_Remote::cHySpexVNIR_3000N_Properties_Remote(cHySpexVNIR_3000N_PropertyPage_Remote* property_page)
	: cExperimentStateRemoteInterface(), cHySpexVNIR_3000N_PropertiesNetEncoder(255)
{
	mHostname = property_page->getHostname();
	mPort = property_page->getPort();
	mLocalIpAddress = property_page->getLocalIpAddress();
	mUse_IpV6 = property_page->usingIpV6();
}

cHySpexVNIR_3000N_Properties_Remote::~cHySpexVNIR_3000N_Properties_Remote()
{
	closeConnection();
}

void cHySpexVNIR_3000N_Properties_Remote::configure(const nlohmann::json& stateDoc)
{
}

QString cHySpexVNIR_3000N_Properties_Remote::getStatusStr()
{
	QString msg;
	if (mWaitingForAcquisitionState)
		msg = "Waiting for VNIR-3000N acquisition parameters...";
	return msg;
}

bool cHySpexVNIR_3000N_Properties_Remote::recording()
{
	return false;
}

void cHySpexVNIR_3000N_Properties_Remote::initialize()
{
	if (!cExperimentStateRemoteInterface::initialize(mHostname, mPort, mUse_IpV6, mLocalIpAddress))
		return;

	openConnection();
}

void cHySpexVNIR_3000N_Properties_Remote::run()
{
	if (mConnected)
	{
		if (!mHasAcquisitionState && !mWaitingForAcquisitionState)
		{
			mWaitingForAcquisitionState = true;
			sendQueryState();
		}
	}
}

void cHySpexVNIR_3000N_Properties_Remote::pause()
{
}

void cHySpexVNIR_3000N_Properties_Remote::stop()
{
}

cExperimentState::eRESULT cHySpexVNIR_3000N_Properties_Remote::finished()
{
	return eRESULT::WAITING;

	if (!mConnected)
		return eRESULT::ABORT;

	if (mWaitingForAcquisitionState)
		return eRESULT::WAITING;

	return eRESULT::DONE;
}

void cHySpexVNIR_3000N_Properties_Remote::onCurrentState(bool valid, std::uint16_t average_frames,
	std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
	std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
	std::uint32_t num_backgrounds, const std::string& lens_name)
{
	mHasAcquisitionState = true;
	mWaitingForAcquisitionState = false;
	mCurrentAverageFrames = average_frames;
	mCurrentFramePeriod_us = frame_period_us;
	mCurrentIntegrationTime_us = integration_time_us;
	mCurrentNumBackgrounds = num_backgrounds;
}

void cHySpexVNIR_3000N_Properties_Remote::onLensNames(const std::vector<std::string>& names)
{}

void cHySpexVNIR_3000N_Properties_Remote::onBackgroundReply(eBackgroundReply reply)
{}

void cHySpexVNIR_3000N_Properties_Remote::onConnect()
{}

void cHySpexVNIR_3000N_Properties_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexVNIR_3000N_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexVNIR_3000N_Properties_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cExperimentStateRemoteInterface::sendOutgoingData(data, len);
}

