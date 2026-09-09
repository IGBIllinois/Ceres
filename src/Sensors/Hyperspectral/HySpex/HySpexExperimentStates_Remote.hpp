
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"
#include "HySpexVNIR_3000N_PropertiesNetDecoder.hpp"
#include "HySpexVNIR_3000N_PropertiesNetEncoder.hpp"
#include "HySpexSWIR_384_PropertiesNetDecoder.hpp"
#include "HySpexSWIR_384_PropertiesNetEncoder.hpp"

#include "Timers.hpp"


// Forward Declarations
class cHySpexSWIR_384_PropertyPage_Remote;
class cHySpexVNIR_3000N_PropertyPage_Remote;

#include <optional>


/*******************************************************************/
/**           Base Class for HySpex Experiment States             **/
/*******************************************************************/

class cHySpexCamera_ExperimentState_Remote : public cExperimentState, public cExperimentStateRemoteInterface
{
	Q_OBJECT

public:
	cHySpexCamera_ExperimentState_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);
	virtual ~cHySpexCamera_ExperimentState_Remote();

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
/**         HySpex Experiment States to Control Shutter           **/
/*******************************************************************/

class cHySpexCamera_ShutterCtrl_Remote : public cHySpexCamera_ExperimentState_Remote,
	protected cHySpexCamera_PropertiesNetDecoder, private cHySpexCamera_PropertiesNetEncoder
{
	Q_OBJECT

public:
	cHySpexCamera_ShutterCtrl_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, eShutterState desired_state, QObject* parent = nullptr);

	bool configure(const nlohmann::json& stateDoc) override;

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

protected:

	void onCurrentStateMessage(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override {};

	void onLensNamesMessage(const std::vector<std::string>& names) override {};

	void onLensInfoMessage(const std::string& name, double working_distance_cm, double fov_deg) override {};

	void onCommandReplyMessage(eCommandReply reply) override {};
	void onBackgroundReplyMessage(eBackgroundReply reply) override {};

	void onShutterStateMessage(eShutterState state) override;

	void onConnect() override;
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

protected:
	eShutterState mShutterState = eShutterState::UNKNOWN;
	const eShutterState mDesiredState;

	cIntervalTimer mShutterTimer;
};

/*** Experimental State to Close Shutter ***/
class cHySpexCamera_CloseShutter_Remote : public cHySpexCamera_ShutterCtrl_Remote
{
public:
	cHySpexCamera_CloseShutter_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;
};

/*** Experimental State to Open Shutter ***/
class cHySpexCamera_OpenShutter_Remote : public cHySpexCamera_ShutterCtrl_Remote
{
public:
	cHySpexCamera_OpenShutter_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;
};


/*******************************************************************/
/**       HySpex Experiment States to Control Acquisition         **/
/*******************************************************************/

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
	void onLensNamesMessage(const std::vector<std::string>& names) override {};
	void onLensInfoMessage(const std::string& name, double working_distance_cm, double fov_deg) override {};
	void onShutterStateMessage(eShutterState state) override {};

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


/*** Experimental State to Adjust Acquisition Parameters ***/
class cHySpexCamera_AcqParameters_Remote : public cHySpexCamera_Acquisition_Remote
{
public:
	cHySpexCamera_AcqParameters_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool initialize() override;

	eRESULT finished() override;

protected:
	void onCurrentStateMessage(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override;

	void onBackgroundReplyMessage(eBackgroundReply reply) override {};
};


/*** Experimental State to Do Background Measurement ***/
class cHySpexCamera_Background_Remote : public cHySpexCamera_Acquisition_Remote
{
public:
	cHySpexCamera_Background_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	bool configure(const nlohmann::json& stateDoc) override;

	QString getStatusStr() override;

	eRESULT finished() override;

protected:
	void onCurrentStateMessage(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override;

	void onCommandReplyMessage(eCommandReply reply) override;
	void onBackgroundReplyMessage(eBackgroundReply reply) override;

	void onConnect() override;

protected:
	std::optional<std::uint32_t> mDesiredNumBackgrounds;
	std::uint32_t mCurrentNumBackgrounds = 0;

	enum class eSTATE { WAIT_FOR_CONNECT, WAIT_FOR_STATE, WAIT_FOR_STATE_UPDATE, WAIT_FOR_BACKGROUND, COMPLETE, ERROR };
	eSTATE mState = eSTATE::WAIT_FOR_CONNECT;
};






#if 0
/*******************************************************************/
/**            The HySpex VNIR-3000N Experiment States            **/
/*******************************************************************/

class cHySpexVNIR_3000N_Properties_Remote : public cHySpexCamera_Properties_Remote,
	private cHySpexVNIR_3000N_PropertiesNetDecoder, private cHySpexVNIR_3000N_PropertiesNetEncoder
{
	Q_OBJECT

public:
	cHySpexVNIR_3000N_Properties_Remote(cHySpexVNIR_3000N_PropertyPage_Remote* property_page);
	~cHySpexVNIR_3000N_Properties_Remote();

	QString getStatusStr() override;

protected:
	void onCurrentStateMessage(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override;

	void onLensNamesMessage(const std::vector<std::string>& names) override;

	void onLensInfoMessage(const std::string& name, double working_distance_cm, double fov_deg) override;

	void onBackgroundReplyMessage(eBackgroundReply reply) override;

	void onShutterStateMessage(eShutterState state) override;

	void onConnect() override;
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

private:
	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;
	uint16_t   mPort = 0;
};


/*******************************************************************/
/**            The HySpex SWIR-384 Experiment State               **/
/*******************************************************************/

class cHySpexSWIR_384_Properties_Remote : public cHySpexCamera_Properties_Remote,
	private cHySpexSWIR_384_PropertiesNetDecoder, private cHySpexSWIR_384_PropertiesNetEncoder
{
	Q_OBJECT

public:
	cHySpexSWIR_384_Properties_Remote(cHySpexSWIR_384_PropertyPage_Remote* property_page);
	~cHySpexSWIR_384_Properties_Remote();

	QString getStatusStr() override;

protected:
	void onCurrentStateMessage(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override;

	void onLensNamesMessage(const std::vector<std::string>& names) override;

	void onLensInfoMessage(const std::string& name, double working_distance_cm, double fov_deg) override;

	void onBackgroundReplyMessage(eBackgroundReply reply) override;

	void onShutterStateMessage(eShutterState state) override;

	void onConnect() override;
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

private:
	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;
	uint16_t   mPort = 0;
};
#endif


