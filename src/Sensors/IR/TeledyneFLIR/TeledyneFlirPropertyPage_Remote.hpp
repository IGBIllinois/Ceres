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
    void onMode(uint8_t mode) override;
    void onImageSize(uint16_t width, uint16_t height) override;
    void onFrameRate(double fps) override;
    void onFrameInterval(uint32_t interval_ms) override;
    void onCurrentState(bool valid, uint8_t mode,
        uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
        std::optional<double> min_fps, std::optional<double> max_fps) override;

protected:
    void onConnect() override;
    void requestImage() override;

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


