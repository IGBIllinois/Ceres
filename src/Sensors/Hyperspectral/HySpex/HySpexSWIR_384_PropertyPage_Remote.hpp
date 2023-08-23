/*
 * The HySpexSWIR_384_PropertyPage to control the HySpex SWIR 384
 * connected to a remote computer.
 */

#pragma once

#include "HySpexSWIR_384_PropertyPage.hpp"
#include "HySpexSWIR_384_PropertiesNetDecoder.hpp"
#include "HySpexSWIR_384_PropertiesNetEncoder.hpp"


class cHySpexSWIR_384_PropertyPage_Remote : public cHySpexSWIR_384_PropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cHySpexSWIR_384_PropertiesNetDecoder,
    private cHySpexSWIR_384_PropertiesNetEncoder
{

public:
    cHySpexSWIR_384_PropertyPage_Remote(QWidget* parent = nullptr);
    ~cHySpexSWIR_384_PropertyPage_Remote() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry) override;

public:
    void onCurrentState(bool valid, std::uint16_t average_frames,
        std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
        std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
        std::uint32_t num_backgrounds, const std::string& lens_name) override;

    void onLensNames(const std::vector<std::string>& names) override;

    void onCommandReply(eCommandReply reply) override;

    void onBackgroundReply(eBackgroundReply reply) override;

    void onShutterState(eShutterState state) override;

protected:
    void onConnect() override;

protected:
    void showPage() override;

    void doCalcBackground() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

protected:
    void sendChangedData(bool* pNeedsUpdate = nullptr);
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

private:
    bool mWaitingForBackgroundReply = false;
};


