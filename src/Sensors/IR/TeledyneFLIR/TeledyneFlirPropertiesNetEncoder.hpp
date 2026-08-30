/*
 * The TeledyneFlirPropertiesNetEncoder is used to encode TCP packets
 * to send to the TeledyneFlirControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cTeledyneFlirPropertiesNetEncoder : public cNetworkEncoder
{

public:
    cTeledyneFlirPropertiesNetEncoder(std::size_t capacity);
    ~cTeledyneFlirPropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    void sendQueryStateMessage();
    void sendQueryModeMessage();
    void sendQueryImageSizeMessage();
    void sendQueryFrameRateMessage();
    void sendQueryFrameIntervalMessage();
    void sendQueryThermalRangeMessage();

    void sendSetModeMessage(uint8_t mode);
    void sendSetImageSizeMessage(uint16_t width, uint16_t height);
    void sendSetFrameRateMessage(double fps);
    void sendSetLapseIntervalMessage(uint32_t interval_ms);
    void sendGrabImageMessage();
    void sendTakePhotoMessage(bool update_view);
    void sendTakePhotoMessage(bool update_view, bool save_image);

    void sendSaveStateMessage();
    void sendRestoreStateMessage();
};


