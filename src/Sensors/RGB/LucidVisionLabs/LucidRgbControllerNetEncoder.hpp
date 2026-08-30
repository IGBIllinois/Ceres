
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
    void sendCurrentStateMessage(bool valid, uint8_t camera_id,
        uint16_t width, uint16_t height, uint8_t fps);

    void sendCurrentStateMessage(bool valid, uint8_t active_camera_id,
        uint16_t width, uint16_t height, uint8_t fps,
        uint8_t min_camera_id, uint8_t max_camera_id);

    void sendCameraModeMessage(uint8_t mode);
    void sendImageSizeMessage(uint16_t width, uint16_t height);

    void sendFrameRateMessage(uint8_t fps);
    void sendLapseIntervalMessage(uint32_t interval_ms);

    void sendTakePhotoReplyMessage();
};


