
#pragma once

#include "ExperimentState.hpp"
#include "HySpexVNIR_3000N_PropertiesNetDecoder.hpp"
#include "HySpexVNIR_3000N_PropertiesNetEncoder.hpp"

// Forward Declarations
class cHySpexSWIR_384_PropertyPage_Remote;
class cHySpexVNIR_3000N_PropertyPage_Remote;


class cHySpexVNIR_3000N_Properties_Remote : public cExperimentStateRemoteInterface, public cExperimentState,
	private cHySpexVNIR_3000N_PropertiesNetDecoder, private cHySpexVNIR_3000N_PropertiesNetEncoder
{
	Q_OBJECT

public:
	cHySpexVNIR_3000N_Properties_Remote(cHySpexVNIR_3000N_PropertyPage_Remote* property_page);
	~cHySpexVNIR_3000N_Properties_Remote();

	QString getStatusStr() override;

	void configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	void initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

protected:
	void onCurrentState(bool valid, std::uint16_t average_frames,
		std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
		std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
		std::uint32_t num_backgrounds, const std::string& lens_name) override;

	void onLensNames(const std::vector<std::string>& names) override;

	void onBackgroundReply(eBackgroundReply reply) override;

	void onConnect() override;
	void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
	int sendOutgoingData(const char* data, std::size_t len) override;

private:
	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;
	uint16_t   mPort = 0;

	std::uint16_t mDesiredAverageFrames = 0;
	std::uint32_t mDesiredFramePeriod_us = 0;
	std::uint32_t mDesiredIntegrationTime_us = 0;
	std::uint32_t mDesiredNumBackgrounds = 0;

	bool mHasAcquisitionState = false;
	bool mWaitingForAcquisitionState = false;
	std::uint16_t mCurrentAverageFrames = 0;
	std::uint32_t mCurrentFramePeriod_us = 0;
	std::uint32_t mCurrentIntegrationTime_us = 0;
	std::uint32_t mCurrentNumBackgrounds = 0;

	bool mDoBackground = false;
};

