/*
 * The HySpexVNIR_3000N_PropertyPage to control the HySpex VNIR 3000N
 * connected to a remote computer.
 */

#pragma once

#include "HySpexVNIR_3000N_PropertyPage.hpp"
#include "HySpexVNIR_3000N_PropertiesNetDecoder.hpp"
#include "HySpexVNIR_3000N_PropertiesNetEncoder.hpp"

class cHySpexVNIR_3000N_PropertyPage_Remote : public cHySpexVNIR_3000N_PropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cHySpexVNIR_3000N_PropertiesNetDecoder, private cHySpexVNIR_3000N_PropertiesNetEncoder
{
public:
    cHySpexVNIR_3000N_PropertyPage_Remote(QWidget* parent = nullptr);
    ~cHySpexVNIR_3000N_PropertyPage_Remote() = default;

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

    void changeShutterState()  override;
    void doCalcBackground() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

private:
    void sendChangedData();
    void queryState();
    void queryShutterState();
    void queryBackgroundState();
    void queryLensNames();
    void setAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    void setLensName(const std::string& lens_name);
    void setNumOfBackgrounds(int num_backgrounds);
    void calcBackground();

protected:
    /*** Network Message handers from the decoder */
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


