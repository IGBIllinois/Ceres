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

    void sendQueryState();
    void sendQueryMode();
    void sendQueryImageSize();
    void sendQueryFrameRate();
    void sendQueryFrameInterval();

    void sendSetMode(uint8_t mode);
    void sendSetCameraId(uint8_t id);
    void sendSetImageSize(uint16_t width, uint16_t height);
    void sendSetFrameRate_fps(uint8_t fps);
    void sendSetLapseInterval_ms(uint32_t interval_ms);
    void sendGrabImage();
    void sendTakePhoto(bool update_view);
    void sendTakePhoto(bool update_view, bool save_image);
};


