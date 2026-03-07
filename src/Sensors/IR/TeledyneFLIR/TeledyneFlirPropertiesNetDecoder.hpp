/*
 * The TeledyneFlirPropertiesNetDecoder is used to decode TCP packets 
 * from the TeledyneFlirControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>
#include <optional>


class cTeledyneFlirPropertiesNetDecoder : public cNetworkDecoder
{
public:

    cTeledyneFlirPropertiesNetDecoder() = default;
    virtual ~cTeledyneFlirPropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */
    virtual void onMode(uint8_t id) = 0;
    virtual void onImageSize(uint16_t width, uint16_t height) = 0;
    virtual void onFrameRate(double fps) = 0;
    virtual void onFrameInterval(uint32_t interval_ms) = 0;
    virtual void onCurrentState(bool valid, uint8_t mode,
        uint16_t width, uint16_t height, double fps, uint32_t interval_ms, 
        std::optional<double> min_fps, std::optional<double> max_fps) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


