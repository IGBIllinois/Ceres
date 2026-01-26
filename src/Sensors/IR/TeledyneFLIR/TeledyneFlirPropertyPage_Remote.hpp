/*
 * The TeledyneFlirPropertyPage to control the Teledyne FLIR camera
 * connected to a remote computer.
 */

#pragma once

#include "TeledyneFlirPropertyPage.hpp"
#include "TeledyneFlirPropertiesNetDecoder.hpp"
#include "TeledyneFlirPropertiesNetEncoder.hpp"

class cTeledyneFlirPropertyPage_Remote : public cTeledyneFlirPropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cTeledyneFlirPropertiesNetDecoder, private cTeledyneFlirPropertiesNetEncoder
{

public:
    cTeledyneFlirPropertyPage_Remote(QWidget* parent = nullptr);
    ~cTeledyneFlirPropertyPage_Remote() = default;

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


