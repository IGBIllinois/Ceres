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
    virtual void onQueryState() = 0;
    virtual void onQueryMode() = 0;
    virtual void onQueryCameraId() = 0;
    virtual void onQueryImageSize() = 0;
    virtual void onQueryFrameRate() = 0;
    virtual void onQueryLapseInterval() = 0;
    virtual void onGrabImage() = 0;
    virtual void onTakePhoto(bool updateView) = 0;
    virtual void onTakePhoto(bool updateView, bool autoSave) = 0;

    virtual void setMode(uint8_t mode) = 0;
    virtual void setCameraId(uint8_t id) = 0;
    virtual void setImageSize(uint16_t width, uint16_t height) = 0;
    virtual void setFrameRate(uint8_t fps) = 0;
    virtual void setLapseInterval_ms(uint32_t interval_ms) = 0;

    virtual void onSaveState() = 0;
    virtual void onRestoreState() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


