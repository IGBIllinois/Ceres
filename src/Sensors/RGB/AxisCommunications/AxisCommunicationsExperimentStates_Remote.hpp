
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"
#include "AxisPropertiesNetDecoder.hpp"
#include "AxisPropertiesNetEncoder.hpp"

#include "Timers.hpp"


// Forward Declarations
class cLucidVisionLabsRgbPropertyPage_Remote;

#include <optional>


/***********************************************************************************/
/**         Base Class for Axis Communications Remote Experiment States           **/
/***********************************************************************************/

class cAxisCommunicationsExperimentState_Remote : public cExperimentStateRemoteInterface, public cExperimentState,
	protected cAxisPropertiesNetDecoder, protected cAxisPropertiesNetEncoder
{
	Q_OBJECT

public:
	cAxisCommunicationsExperimentState_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);
	virtual ~cAxisCommunicationsExperimentState_Remote();

	void cleanup() override;

	bool recording() override;
	bool initialize() override;

	void run() override {};
	void pause() override {};
	void stop() override {};


protected:
	virtual void onConnect() = 0;

private:
	void onMode(uint8_t id) override {};
	void onCameraId(uint8_t id) override {};
	void onImageSize(uint16_t width, uint16_t height) override {};
	void onFrameRate(uint8_t fps) override {};
	void onLapseInterval(uint32_t interval_ms) override {};
	void onCurrentState(bool valid, uint8_t id, uint16_t width, uint16_t height, uint8_t fps) override {};
	void onCurrentState(bool valid, uint8_t active_id, uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id) override {};
	void onCurrentState(bool valid, uint8_t mode, uint8_t active_id, uint16_t width, uint16_t height, uint8_t fps, uint32_t interval_ms,
		uint8_t min_id, uint8_t max_id, std::optional<double> min_fps, std::optional<double> max_fps) override {};

	void onTakePhotoReply(bool error)  override {};

private:
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

protected:
	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;
	uint16_t   mPort = 0;
};


/***********************************************************************/
/** Axis Communications Experiment State to Save Current Camera State **/
/***********************************************************************/

class cAxisCommunications_SaveState_Remote : public cAxisCommunicationsExperimentState_Remote
{
	Q_OBJECT

public:
	cAxisCommunications_SaveState_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override { return true; };
	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

private:
	void onConnect() override;

private:
	eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


/***********************************************************************/
/**   Axis Communications Experiment State to Restore Camera State    **/
/***********************************************************************/

class cAxisCommunications_RestoreState_Remote : public cAxisCommunicationsExperimentState_Remote
{
	Q_OBJECT

public:
	cAxisCommunications_RestoreState_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override { return true; };
	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

private:
	void onConnect() override;

private:
	eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


/***************************************************************************/
/**     Axis Communications Experiment States to Configure Camera         **/
/***************************************************************************/

class cAxisCommunications_Configure_Remote : public cAxisCommunicationsExperimentState_Remote
{
	Q_OBJECT

public:
	cAxisCommunications_Configure_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return false; };

	eRESULT finished() override;

private:
//	void onMode(uint8_t id) override;
//	void onFrameRate(double fps) override;
//	void onLapseInterval(uint32_t interval_ms) override;
//	void onCurrentState(bool valid, uint8_t mode,
//		uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
//		std::optional<double> min_fps, std::optional<double> max_fps,
//		std::optional<float> min_K, std::optional<float> max_K) override;

	void onConnect() override;

private:
	int mMode = -1;
	int mLapseInterval_ms = -1;
	double mFrameRate_fps = -1;

	bool mWaitingForConfiguration = true;

	bool mWaitingForCameraId = false;
	bool mWaitingForMode = false;
	bool mWaitingForFrameRate = false;
	bool mWaitingForInterval = false;
};


/****************************************************************************/
/**         Axis Communications Experiment States to Take Photo            **/
/****************************************************************************/

class cAxisCommunications_TakePhoto_Remote : public cAxisCommunicationsExperimentState_Remote
{
	Q_OBJECT

public:
	cAxisCommunications_TakePhoto_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return true; };

	eRESULT finished() override;

private:
	void onConnect() override;

private:
	bool mUpdateView = false;

	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


