
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"
#include "TeledyneFlirPropertiesNetDecoder.hpp"
#include "TeledyneFlirPropertiesNetEncoder.hpp"
#include "TeledyneFlirExperimentStatesHelpers.hpp"
#include "TeledyneFlirCameraModel.hpp"

#include "Timers.hpp"


// Forward Declarations
class cTeledyneFlirPropertyPage_Remote;

#include <optional>


/*******************************************************************/
/**    Base Class for Teledyne FLIR Remote Experiment States      **/
/*******************************************************************/

class cTeledyneFlirCameraExperimentState_Remote : public cExperimentStateRemoteInterface, public cExperimentState,
	protected cTeledyneFlirPropertiesNetDecoder, protected cTeledyneFlirPropertiesNetEncoder
{
	Q_OBJECT

public:
	cTeledyneFlirCameraExperimentState_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);
	virtual ~cTeledyneFlirCameraExperimentState_Remote();

	void cleanup() override;

	bool recording() override;
	bool initialize() override;

	void run() override {};
	void pause() override {};
	void stop() override {};


protected:
	virtual void onConnect() = 0;

private:
	void onModeMessage(uint8_t id) override {};
	void onImageSizeMessage(uint16_t width, uint16_t height) override {};
	void onFrameRateMessage(double fps) override {};
	void onLapseIntervalMessage(uint32_t interval_ms) override {};
	void onThermalRangeMessage(float min_value_K, float max_value_K) override {};
	void onCurrentStateMessage(bool valid, uint8_t mode,
		uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
		std::optional<double> min_fps, std::optional<double> max_fps,
		std::optional<float> min_K, std::optional<float> max_K) override {};
	void onTakePhotoReplyMessage(bool error)  override {};

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
/**    Teledyne FLIR Experiment State to Save Current Camera State    **/
/***********************************************************************/

class cTeledyneFlirCamera_SaveState_Remote : public cTeledyneFlirCameraExperimentState_Remote
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_SaveState_Remote(const std::string& hostname, uint16_t port,
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
/**      Teledyne FLIR Experiment State to Restore Camera State       **/
/***********************************************************************/

class cTeledyneFlirCamera_RestoreState_Remote : public cTeledyneFlirCameraExperimentState_Remote
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_RestoreState_Remote(const std::string& hostname, uint16_t port,
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


/*******************************************************************/
/**     Teledyne FLIR Experiment State to Configure Camera        **/
/*******************************************************************/

class cTeledyneFlirCamera_Configure_Remote : public cTeledyneFlirCameraExperimentState_Remote, protected cTeledyneFlirExperimentHelper_Configure
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_Configure_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return false; };

	eRESULT finished() override;

private:
	void onModeMessage(uint8_t id) override;
	void onFrameRateMessage(double fps) override;
	void onLapseIntervalMessage(uint32_t interval_ms) override;
	void onCurrentStateMessage(bool valid, uint8_t mode,
		uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
		std::optional<double> min_fps, std::optional<double> max_fps,
		std::optional<float> min_K, std::optional<float> max_K) override;

	void onConnect() override;
};


/*******************************************************************/
/**        Teledyne FLIR Experiment State to Take Photo           **/
/*******************************************************************/

class cTeledyneFlirCamera_TakePhoto_Remote : public cTeledyneFlirCameraExperimentState_Remote, protected cTeledyneFlirExperimentHelper_TakePhoto
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_TakePhoto_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return true; };

	eRESULT finished() override;

private:
	void onModeMessage(uint8_t mode) override;
	void onTakePhotoReplyMessage(bool error) override;

	void onConnect() override;

private:
	cTeledyneFlirCameraModel::eMode mMode = cTeledyneFlirCameraModel::eMode::CONTINUOUS;

	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


