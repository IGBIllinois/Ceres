
#pragma once

#include "ExperimentState.hpp"

#include <QObject>

#include <optional>


// Forward Declarations
class cHySpexSWIR_384_Model;
class cHySpexVNIR_3000N_Model;


/*******************************************************************/
/**           Base Class for HySpex Experiment States             **/
/*******************************************************************/

class cHySpexCamera_ExperimentState_Local : public cExperimentState
{
	Q_OBJECT

public:
	cHySpexCamera_ExperimentState_Local(QObject* parent = nullptr);
	virtual ~cHySpexCamera_ExperimentState_Local();

	void cleanup() override;

	bool recording() override;
	bool initialize() override;
};

/*******************************************************************/
/**         HySpex Experiment States to Control Shutter           **/
/*******************************************************************/

class cHySpexCamera_ShutterCtrl_Local : public cHySpexCamera_ExperimentState_Local
{
	Q_OBJECT

protected:
	enum class eShutterState { UNKNOWN, OPEN, CLOSED, PENDING_OPEN, PENDING_CLOSED, ERROR };

//	enum class eCommandReply { UNKNOWN, CALC_BACKGROUND, STOP_BACKGROUND };

//	enum class eBackgroundReply { GOOD, FAILED, ABORTED, PENDING };

public:
	cHySpexCamera_ShutterCtrl_Local(eShutterState desired_state, QObject* parent = nullptr);

	bool configure(const nlohmann::json& stateDoc) override;

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

protected:
	eShutterState mShutterState = eShutterState::UNKNOWN;
	const eShutterState mDesiredState;

//	cIntervalTimer mShutterTimer;
};

/*** Experimental State to Close Shutter ***/
class cHySpexCamera_CloseShutter_Local : public cHySpexCamera_ShutterCtrl_Local
{
public:
	cHySpexCamera_CloseShutter_Local(QObject* parent = nullptr);

	QString getStatusStr() override;
};

/*** Experimental State to Open Shutter ***/
class cHySpexCamera_OpenShutter_Local : public cHySpexCamera_ShutterCtrl_Local
{
public:
	cHySpexCamera_OpenShutter_Local(QObject* parent = nullptr);

	QString getStatusStr() override;
};


/*******************************************************************/
/**       HySpex Experiment States to Control Acquisition         **/
/*******************************************************************/

class cHySpexCamera_Acquisition_Local : public cHySpexCamera_ExperimentState_Local
{
	Q_OBJECT

public:
	cHySpexCamera_Acquisition_Local(QObject* parent = nullptr);
	~cHySpexCamera_Acquisition_Local();

	bool configure(const nlohmann::json& stateDoc) override;

	void run() override;
	void pause() override;
	void stop() override;

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
class cHySpexCamera_AcqParameters_Local : public cHySpexCamera_Acquisition_Local
{
public:
	cHySpexCamera_AcqParameters_Local(QObject* parent = nullptr);

	QString getStatusStr() override;

	bool initialize() override;

	eRESULT finished() override;

};


/*** Experimental State to Do Background Measurement ***/
class cHySpexCamera_Background_Local : public cHySpexCamera_Acquisition_Local
{
public:
	cHySpexCamera_Background_Local(QObject* parent = nullptr);

	bool configure(const nlohmann::json& stateDoc) override;

	QString getStatusStr() override;

	eRESULT finished() override;

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
	void onCurrentState(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override;

	void onLensNames(const std::vector<std::string>& names) override;

	void onBackgroundReply(eBackgroundReply reply) override;

	void onShutterState(eShutterState state) override;

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
	void onCurrentState(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override;

	void onLensNames(const std::vector<std::string>& names) override;

	void onBackgroundReply(eBackgroundReply reply) override;

	void onShutterState(eShutterState state) override;

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


