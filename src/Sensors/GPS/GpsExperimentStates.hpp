
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"
#include "GpsPropertiesNetDecoder.hpp"
#include "GpsPropertiesNetEncoder.hpp"


// Forward Declarations


#include <optional>


/*******************************************************************/
/**           Base Class for GPS Experiment States             **/
/*******************************************************************/

class cGpsExperimentState_Remote : public cExperimentStateRemoteInterface, public cExperimentState
{
	Q_OBJECT

public:
	cGpsExperimentState_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);
	virtual ~cGpsExperimentState_Remote();

	void cleanup() override;

	bool recording() override;
	bool initialize() override;

protected:
	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;
	uint16_t   mPort = 0;
};

/*******************************************************************/
/**   GPS Experiment States to Control Reference Acquisition      **/
/*******************************************************************/

class cGpsReferenceAcquisition_Remote : public cGpsExperimentState_Remote,
	protected cGpsPropertiesNetDecoder, private cGpsPropertiesNetEncoder
{
	Q_OBJECT

public:
	cGpsReferenceAcquisition_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);
	virtual ~cGpsReferenceAcquisition_Remote();

	bool configure(const nlohmann::json& stateDoc) override;

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

	QString getStatusStr() override;

protected:

	void onReferenceParameters(bool valid, uint16_t min_integration_time_sec, 
		uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm) override;

	void onReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
		double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid) override;

	void onReferenceCommandReply(eReferenceReply reply) override;

	void onConnect() override;
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

protected:
	std::optional<std::uint16_t> mDesiredMinIntegrationTime_sec;
	std::optional<std::uint16_t> mDesiredMaxIntegrationTime_sec;
	std::optional<std::uint16_t> mDesiredRefErrorThreshold_mm;

	std::uint16_t mCurrentMinIntegrationTime_sec = 0;
	std::uint16_t mCurrentMaxIntegrationTime_sec = 0;
	std::uint16_t mCurrentRefErrorThreshold_mm = 0;

	bool mHasReferenceParameters = false;

	enum class eSTATE { WAIT_FOR_CONNECT, WAIT_FOR_STATE, WAIT_FOR_STATE_UPDATE, WAIT_FOR_REFERENCE, COMPLETE, ERROR };
	eSTATE mState = eSTATE::WAIT_FOR_CONNECT;

private:
	void processPacket(gps::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override {};
};

