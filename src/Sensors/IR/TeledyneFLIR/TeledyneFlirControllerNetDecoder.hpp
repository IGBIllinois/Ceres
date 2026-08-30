/*
 * The TeledyneFlirControllerNetDecoder is used to decode TCP packets
 * from the TeledyneFlirPropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>
#include <cstdint>


class cTeledyneFlirControllerNetDecoder : public cNetworkDecoder
{
public:

    cTeledyneFlirControllerNetDecoder() = default;
    virtual ~cTeledyneFlirControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryStateMessage() = 0;
    virtual void onQueryModeMessage() = 0;
    virtual void onQueryImageSizeMessage() = 0;
    virtual void onQueryFrameRateMessage() = 0;
    virtual void onQueryLapseIntervalMessage() = 0;
    virtual void onQueryThermalRangeMessage() = 0;
    virtual void onGrabImageMessage() = 0;
    virtual void onTakePhotoMessage(bool updateView) = 0;
    virtual void onTakePhotoMessage(bool updateView, bool autoSave) = 0;

    virtual void onSetModeMessage(uint8_t mode) = 0;
    virtual void onSetImageSizeMessage(uint16_t width, uint16_t height) = 0;
    virtual void onSetFrameRateMessage(double fps) = 0;
    virtual void onSetLapseIntervalMessage(uint32_t interval_ms) = 0;
    virtual void onSetThermalRangeMessage(float min_value_K, float max_value_K) = 0;

    virtual void onSaveStateMessage() = 0;
    virtual void onRestoreStateMessage() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


