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
    virtual void onQueryCameraId() = 0;
    virtual void onQueryImageSize() = 0;
    virtual void onQueryFrameRate() = 0;

    virtual void setCameraId(uint8_t id) = 0;
    virtual void setImageSize(uint16_t width, uint16_t height) = 0;
    virtual void setFrameRate(uint8_t fps) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


