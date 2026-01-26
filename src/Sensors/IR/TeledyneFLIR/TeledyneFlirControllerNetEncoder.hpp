
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
    void sendCurrentState(bool valid, uint8_t camera_id,
        uint16_t width, uint16_t height, uint8_t fps);

    void sendCurrentState(bool valid, uint8_t active_camera_id,
        uint16_t width, uint16_t height, uint8_t fps,
        uint8_t min_camera_id, uint8_t max_camera_id);

    void sendActiveCameraId(uint8_t camera_id);
    void sendImageSize(uint16_t width, uint16_t height);
    void sendFrameRate(uint8_t fps);
};


