/*
 * The AxisControllerNetDecoder is used to decode TCP packets
 * from the AxisPropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>
#include <cstdint>


class cAxisControllerNetDecoder : public cNetworkDecoder
{
public:

    cAxisControllerNetDecoder() = default;
    virtual ~cAxisControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryStateMessage() = 0;
    virtual void onQueryModeMessage() = 0;
    virtual void onQueryCameraIdMessage() = 0;
    virtual void onQueryImageSizeMessage() = 0;
    virtual void onQueryFrameRateMessage() = 0;
    virtual void onQueryLapseIntervalMessage() = 0;
    virtual void onGrabImageMessage() = 0;
    virtual void onTakePhotoMessage(bool updateView) = 0;
    virtual void onTakePhotoMessage(bool updateView, bool autoSave) = 0;

    virtual void setModeMessage(uint8_t mode) = 0;
    virtual void setCameraIdMessage(uint8_t id) = 0;
    virtual void setImageSizeMessage(uint16_t width, uint16_t height) = 0;
    virtual void setFrameRateMessage(uint8_t fps) = 0;
    virtual void setLapseIntervalMessage(uint32_t interval_ms) = 0;

    virtual void onSaveStateMessage() = 0;
    virtual void onRestoreStateMessage() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


