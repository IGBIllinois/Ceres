
#pragma once

#include "net_packet_encoder.hpp"

#include <cstdint>

class cLucidRgbControllerNetEncoder : public cNetworkEncoder
{

public:
    cLucidRgbControllerNetEncoder(std::size_t capacity);
    ~cLucidRgbControllerNetEncoder() = default;

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


