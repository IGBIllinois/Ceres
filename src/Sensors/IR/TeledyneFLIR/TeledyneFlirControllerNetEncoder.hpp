
#pragma once

#include "net_packet_encoder.hpp"

#include <cstdint>

class cTeledyneFlirControllerNetEncoder : public cNetworkEncoder
{

public:
    cTeledyneFlirControllerNetEncoder(std::size_t capacity);
    ~cTeledyneFlirControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
    void sendCurrentState(bool valid, uint8_t active_camera_id,
        uint16_t width, uint16_t height, uint8_t fps, uint16_t interval_s,
        uint8_t min_fps, uint8_t max_fps);

    void sendCameraMode(uint8_t mode);
    void sendImageSize(uint16_t width, uint16_t height);
    void sendFrameRate_Hz(double fps);
    void sendFrameInterval_ms(uint32_t interval_ms);
};


