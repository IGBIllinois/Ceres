/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams
 * connected to a remote computer.
 */

#pragma once

#include "AxisCommunicationsPropertyPage.hpp"
#include "AxisPropertiesNetDecoder.hpp"
#include "AxisPropertiesNetEncoder.hpp"

#include <optional>


class cAxisCommunicationsPropertyPage_Remote : public cAxisCommunicationsPropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cAxisPropertiesNetDecoder, private cAxisPropertiesNetEncoder
{

public:
    cAxisCommunicationsPropertyPage_Remote(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Remote() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

public:
    void onMode(uint8_t mode) override;
    void onCameraId(uint8_t id) override;
    void onImageSize(uint16_t width, uint16_t height) override;
    void onFrameRate(uint8_t fps) override;
    void onLapseInterval(uint32_t interval_ms) override;
    void onCurrentState(bool valid, uint8_t id,
        uint16_t width, uint16_t height, uint8_t fps) override;
    void onCurrentState(bool valid, uint8_t active_id,
        uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id) override;
    void onCurrentState(bool valid, uint8_t mode, uint8_t active_id,
        uint16_t width, uint16_t height, uint8_t fps, uint32_t interval_ms,
        uint8_t min_id, uint8_t max_id, std::optional<double> min_fps, std::optional<double> max_fps) override;

    void onTakePhotoReply(bool error)  override;

protected:
    void onConnect() override;
    void onGrabImagePressed() override;

protected:
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


