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
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected:
    void onConnect() override;
    void onDisconnect() override;

protected:
    void connectToModel() override {}
    void createWidgets() override;
    void enableControls(bool enable) override;
    void buttonClicked(QAbstractButton* button) override;

    void showPage() override;

    void doCalcBackground() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

protected:
    void sendChangedData();
    void queryState();
    void queryLensNames();
    void setAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    void setLensName(const std::string& lens_name);
    void setNumOfBackgrounds(int num_backgrounds);
    void calcBackground();

public:
    /*** The Message handlers from the network decoder */
    void onCurrentStateMessage(bool valid, std::uint16_t average_frames,
        std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
        std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
        std::uint32_t num_backgrounds, const std::string& lens_name) override;

    void onLensNamesMessage(const std::vector<std::string>& names) override;

    void onLensInfoMessage(const std::string& name, double working_distance_cm, double fov_deg) override;

    void onCommandReplyMessage(eCommandReply reply) override;

    void onBackgroundReplyMessage(eBackgroundReply reply) override;

    void onShutterStateMessage(eShutterState state) override;

private:
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

private:
    bool mReconnectActive = false;
    bool mAcquisitionParametersValid = false;
    bool mBackgroundValid = false;
};


