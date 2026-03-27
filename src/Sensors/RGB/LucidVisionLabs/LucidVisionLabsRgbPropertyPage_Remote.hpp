/*
 * The LucidVisionLabsRgbPropertyPage to control the Lucid Vision Labs Triton RGB cameras
 * connected to a remote computer.
 */

#pragma once

#include "LucidVisionLabsRgbPropertyPage.hpp"
#include "LucidRgbPropertiesNetDecoder.hpp"
#include "LucidRgbPropertiesNetEncoder.hpp"

class cLucidVisionLabsRgbPropertyPage_Remote : public cLucidVisionLabsRgbPropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cLucidRgbPropertiesNetDecoder, private cLucidRgbPropertiesNetEncoder
{

public:
    cLucidVisionLabsRgbPropertyPage_Remote(QWidget* parent = nullptr);
    ~cLucidVisionLabsRgbPropertyPage_Remote() = default;

public:
    void onCameraId(uint8_t id) override;
    void onImageSize(uint16_t width, uint16_t height) override;
    void onFrameRate(uint8_t fps) override;
    void onCurrentState(bool valid, uint8_t id,
        uint16_t width, uint16_t height, uint8_t fps) override;
    void onCurrentState(bool valid, uint8_t active_id,
        uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id) override;

protected:
    void onConnect() override;
    void onGrabImagePressed() override;
    void exposureTimeSelectorChanged(const QString&) override;
    void exposureAutoModeChanged(const QString&) override;
    void pixelFormatChanged(const QString&) override;
    void gainAutoModeChanged(const QString&) override;
    void balanceWhiteAutoModeChanged(const QString&) override;

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
    bool mReconnectActive = false;
};


