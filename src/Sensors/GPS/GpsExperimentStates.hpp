
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"
#include "GpsPropertiesNetDecoder.hpp"
#include "GpsPropertiesNetEncoder.hpp"

#include "Timers.hpp"


// Forward Declarations
class cGpsPropertyPage_Remote;

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
		const std::string& localIpAddress, bool use_IpV6, eShutterState desired_state, QObject* parent = nullptr);

	bool configure(const nlohmann::json& stateDoc) override;

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

	QString getStatusStr() override;

protected:

	void onCurrentState(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override {};

	void onLensNames(const std::vector<std::string>& names) override {};

	void onCommandReply(eCommandReply reply) override {};
	void onBackgroundReply(eBackgroundReply reply) override {};

	void onShutterState(eShutterState state) override;

	void onConnect() override;
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

protected:
	eShutterState mShutterState = eShutterState::UNKNOWN;
	const eShutterState mDesiredState;

	cIntervalTimer mShutterTimer;
};


/*******************************************************************/
/**       HySpex Experiment States to Control Acquisition         **/
/*******************************************************************/

/*
class cHySpexCamera_Acquisition_Remote : public cHySpexCamera_ExperimentState_Remote,
	protected cHySpexCamera_PropertiesNetDecoder, protected cHySpexCamera_PropertiesNetEncoder
{
	Q_OBJECT

public:
	cHySpexCamera_Acquisition_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);
	~cHySpexCamera_Acquisition_Remote();

	bool configure(const nlohmann::json& stateDoc) override;

	void run() override;
	void pause() override;
	void stop() override;

protected:
	void onLensNames(const std::vector<std::string>& names) override {};
	void onShutterState(eShutterState state) override {};

	void onConnect() override;
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

protected:
	std::optional<std::uint16_t> mDesiredAverageFrames;
	std::optional<std::uint32_t> mDesiredFramePeriod_us;
	std::optional<std::uint32_t> mDesiredIntegrationTime_us;

	std::uint16_t mCurrentAverageFrames = 0;
	std::uint32_t mCurrentFramePeriod_us = 0;
	std::uint32_t mCurrentIntegrationTime_us = 0;

	bool mHasAcquisitionState = false;
};
*/

