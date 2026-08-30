/*
 * The AxisPropertiesNetEncoder is used to encode TCP packets
 * to send to the AxisControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cAxisPropertiesNetEncoder : public cNetworkEncoder
{

public:
    cAxisPropertiesNetEncoder(std::size_t capacity);
    ~cAxisPropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    void sendQueryStateMessage();
    void sendQueryModeMessage();
    void sendQueryImageSizeMessage();
    void sendQueryFrameRateMessage();
    void sendQueryFrameIntervalMessage();

    void sendSetModeMessage(uint8_t mode);
    void sendSetCameraIdMessage(uint8_t id);
    void sendSetImageSizeMessage(uint16_t width, uint16_t height);
    void sendSetFrameRateMessage(uint8_t fps);
    void sendSetLapseIntervalMessage(uint32_t interval_ms);
    void sendGrabImageMessage();
    void sendTakePhotoMessage(bool update_view);
    void sendTakePhotoMessage(bool update_view, bool save_image);

    void sendSaveStateMessage();
    void sendRestoreStateMessage();
};


