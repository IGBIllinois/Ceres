
#pragma once

#include "net_packet_encoder.hpp"

#include <cstdint>

class cAxisControllerNetEncoder : public cNetworkEncoder
{

public:
    cAxisControllerNetEncoder(std::size_t capacity);
    ~cAxisControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
    void sendCurrentState(bool valid, uint8_t camera_id,
        uint16_t width, uint16_t height, uint8_t fps);

    void sendActiveCameraId(uint8_t camera_id);
    void sendImageSize(uint16_t width, uint16_t height);
    void sendFrameRate(uint8_t fps);
};


