
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"
#include "TeledyneFlirPropertiesNetDecoder.hpp"
#include "TeledyneFlirPropertiesNetEncoder.hpp"

#include "Timers.hpp"


// Forward Declarations
class cTeledyneFlirCameraModel;
class cTeledyneFlirPropertyPage_Remote;

#include <optional>

/*******************************************************************/
/**    Base Class for Teledyne FLIR Local Experiment States      **/
/*******************************************************************/

class cTeledyneFlirCameraExperimentState_Local : public QObject, public cExperimentState
{
	Q_OBJECT

public:
	cTeledyneFlirCameraExperimentState_Local(cTeledyneFlirCameraModel* pModel, QObject* parent = nullptr);
	virtual ~cTeledyneFlirCameraExperimentState_Local();

	void cleanup() override;

	bool initialize() override;

protected:
	const cTeledyneFlirCameraModel* mpModel;
};

class cTeledyneFlirCamera_Configure_Local : public cTeledyneFlirCameraExperimentState_Local
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_Configure_Local(cTeledyneFlirCameraModel* pModel, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

signals:
	void requestMode(int mode);
	void requestFrameRate_Hz(double frame_rate_hz);
	void requestFrameInterval_ms(uint32_t frame_interval_ms);

public slots:
	void modeChanged(int mode);
	void frameIntervalChanged(int interval_ms);
	void frameRateChanged(double rate_fps);

private:
	int mMode = 0;
	int mFrameInterval_ms = 0;
	double mFrameRate_fps = 0;

	bool mUpdateConfiguration = true;

	bool mWaitingForMode = false;
	bool mWaitingForFrameRate = false;
	bool mWaitingForInterval = false;
};


class cTeledyneFlirCamera_TakePhoto_Local : public cTeledyneFlirCameraExperimentState_Local
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_TakePhoto_Local(cTeledyneFlirCameraModel* pModel, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return true; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

signals:
	void updateView();
	void takePhoto(bool update_view);

public slots:
	void onPhotoTaken();

private:
	bool mUpdateView = false;

	bool mTriggerPhoto = true;
	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};



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

private:
	void onMode(uint8_t id) override {};
	void onImageSize(uint16_t width, uint16_t height) override {};
	void onFrameRate(double fps) override {};
	void onFrameInterval(uint32_t interval_ms) override {};
	void onThermalRange(float min_value_K, float max_value_K) override {};
	void onCurrentState(bool valid, uint8_t mode,
		uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
		std::optional<double> min_fps, std::optional<double> max_fps,
		std::optional<float> min_K, std::optional<float> max_K) override {};
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

class cTeledyneFlirCamera_Configure_Remote : public cTeledyneFlirCameraExperimentState_Remote
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_Configure_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

private:
	void onMode(uint8_t id) override;
	void onFrameRate(double fps) override;
	void onFrameInterval(uint32_t interval_ms) override;
	void onCurrentState(bool valid, uint8_t mode,
		uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
		std::optional<double> min_fps, std::optional<double> max_fps,
		std::optional<float> min_K, std::optional<float> max_K) override;

	void onConnect() override;

private:
	int mMode = -1;
	int mFrameInterval_ms = -1;
	double mFrameRate_fps = -1;

	bool mUpdateConfiguration = true;

	bool mWaitingForMode = false;
	bool mWaitingForFrameRate = false;
	bool mWaitingForInterval = false;
};


class cTeledyneFlirCamera_TakePhoto_Remote : public cTeledyneFlirCameraExperimentState_Remote
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_TakePhoto_Remote(const std::string& hostname, uint16_t port,
		const std::string& localIpAddress, bool use_IpV6, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return true; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

private:
	void onMode(uint8_t mode) override;
	void onTakePhotoReply(bool error) override;

	void onConnect() override;

private:
	bool mUpdateView = false;

	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


