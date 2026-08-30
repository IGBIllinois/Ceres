/*
 * The AxisPropertiesNetDecoder is used to decode TCP packets 
 * from the AxisControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>
#include <optional>


class cAxisPropertiesNetDecoder : public cNetworkDecoder
{
public:

    cAxisPropertiesNetDecoder() = default;
    virtual ~cAxisPropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */
    virtual void onModeMessage(uint8_t id) = 0;
    virtual void onCameraIdMessage(uint8_t id) = 0;
    virtual void onImageSizeMessage(uint16_t width, uint16_t height) = 0;
    virtual void onFrameRateMessage(uint8_t fps) = 0;
    virtual void onLapseIntervalMessage(uint32_t interval_ms) = 0;

    virtual void onCurrentStateMessage(bool valid, uint8_t id,
        uint16_t width, uint16_t height, uint8_t fps) = 0;

    virtual void onCurrentStateMessage(bool valid, uint8_t active_id,
        uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id) = 0;

    virtual void onCurrentStateMessage(bool valid, uint8_t mode, uint8_t active_id,
        uint16_t width, uint16_t height, uint8_t fps, uint32_t interval_ms,
        uint8_t min_id, uint8_t max_id, std::optional<double> min_fps, std::optional<double> max_fps) = 0;

    virtual void onTakePhotoReplyMessage(bool error) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


