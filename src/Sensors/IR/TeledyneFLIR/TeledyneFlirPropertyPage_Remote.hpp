/*
 * The TeledyneFlirPropertyPage to control the Teledyne FLIR camera
 * connected to a remote computer.
 */

#pragma once

#include "TeledyneFlirPropertyPage.hpp"
#include "TeledyneFlirPropertiesNetDecoder.hpp"
#include "TeledyneFlirPropertiesNetEncoder.hpp"

#include <optional>


class cTeledyneFlirPropertyPage_Remote : public cTeledyneFlirPropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cTeledyneFlirPropertiesNetDecoder, private cTeledyneFlirPropertiesNetEncoder
{
public:
    cTeledyneFlirPropertyPage_Remote(QWidget* parent = nullptr);
    ~cTeledyneFlirPropertyPage_Remote() = default;
public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

public:
    void onModeMessage(uint8_t mode) override;
    void onImageSizeMessage(uint16_t width, uint16_t height) override;
    void onFrameRateMessage(double fps) override;
    void onLapseIntervalMessage(uint32_t interval_ms) override;
    void onThermalRangeMessage(float min_value_K, float max_value_K) override;
    void onCurrentStateMessage(bool valid, uint8_t mode,
        uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
        std::optional<double> min_fps, std::optional<double> max_fps,
        std::optional<float> min_K, std::optional<float> max_K) override;

    void onTakePhotoReplyMessage(bool error)  override;

protected:
    void onConnect() override;
    void onGrabImagePressed() override;

protected:
    void connectToModel() override {};
    void createWidgets() override;
    void enableControls(bool enable) override;
    void buttonClicked(QAbstractButton* button) override;

    void showPage() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

protected:
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

private:
    uint8_t  mDefaultMode = 255;
    double   mDefaultFrameRate_fps = 0;
    uint32_t mDefaultLapseInterval_ms = 0;

    bool mReconnectActive = false;
};


