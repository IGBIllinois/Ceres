
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"
#include "LucidRgbPropertiesNetDecoder.hpp"
#include "LucidRgbPropertiesNetEncoder.hpp"

#include "Timers.hpp"


// Forward Declarations
class cLucidVisionLabsRgbPropertyPage_Remote;

#include <optional>


/***********************************************************************************/
/**    Base Class for  Lucid Vision Labs RGB Camera Remote Experiment States      **/
/***********************************************************************************/

class cLucidVisionLabsRgbCameraExperimentState_Remote : public cExperimentStateRemoteInterface, public cExperimentState,
	protected cLucidRgbPropertiesNetDecoder, protected cLucidRgbPropertiesNetEncoder
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbCameraExperimentState_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);
	virtual ~cLucidVisionLabsRgbCameraExperimentState_Remote();

	void cleanup() override;

	bool recording() override;
	bool initialize() override;

	void run() override {};
	void pause() override {};
	void stop() override {};


protected:
	virtual void onConnect() = 0;

private:
//	void onMode(uint8_t id) override {};
	void onImageSize(uint16_t width, uint16_t height) override {};
//	void onFrameRate(double fps) override {};
	//void onLapseInterval(uint32_t interval_ms) override {};
//	void onCurrentState(bool valid, uint8_t mode,
//		uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
//		std::optional<double> min_fps, std::optional<double> max_fps,
//		std::optional<float> min_K, std::optional<float> max_K) override {};
//	void onTakePhotoReply(bool error)  override {};

private:
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

protected:
	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;
	uint16_t   mPort = 0;
};

/***************************************************************************/
/**    Lucid Vision Labs RGB Experiment States to Configure Camera        **/
/***************************************************************************/

class cLucidVisionLabsRgbCamera_Configure_Remote : public cLucidVisionLabsRgbCameraExperimentState_Remote
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbCamera_Configure_Remote(const std::string& hostname, uint16_t port,
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

	bool mWaitingForMode = false;
	bool mWaitingForFrameRate = false;
	bool mWaitingForInterval = false;
};


/****************************************************************************/
/**        Lucid Vision Labs RGB Experiment States to Take Photo           **/
/****************************************************************************/

class cLucidVisionLabsRgbCamera_TakePhoto_Remote : public cLucidVisionLabsRgbCameraExperimentState_Remote
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbCamera_TakePhoto_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return true; };

	eRESULT finished() override;

private:
//	void onMode(uint8_t mode) override;
//	void onTakePhotoReply(bool error) override;

	void onConnect() override;

private:
	bool mUpdateView = false;

	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


