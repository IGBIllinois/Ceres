/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams
 * connected to a remote computer.
 */

#pragma once

#include "AxisCommunicationsPropertyPage.hpp"
#include "AxisPropertiesNetDecoder.hpp"
#include "AxisPropertiesNetEncoder.hpp"

class cAxisCommunicationsPropertyPage_Remote : public cAxisCommunicationsPropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cAxisPropertiesNetDecoder, private cAxisPropertiesNetEncoder
{

public:
    cAxisCommunicationsPropertyPage_Remote(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Remote() = default;

public:
    void onCameraId(uint8_t id) override;
    void onImageSize(uint16_t width, uint16_t height) override;
    void onFrameRate(uint8_t fps) override;
    void onCurrentState(bool valid, uint8_t id,
        uint16_t width, uint16_t height, uint8_t fps) override;

protected:
    void onConnect() override;

protected:
    void showPage() override;
    void doOK() override;
    void doCancel() override;
    void doApply() override;

protected:
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

};


