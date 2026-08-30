
#pragma once

#include "net_packet_encoder.hpp"

#include <cstdint>
#include <optional>


class cAxisControllerNetEncoder : public cNetworkEncoder
{

public:
    cAxisControllerNetEncoder(std::size_t capacity);
    ~cAxisControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
    void sendCurrentStateMessage(bool valid, uint8_t camera_id,
        uint16_t width, uint16_t height, uint8_t fps);

    void sendCurrentStateMessage(bool valid, uint8_t active_camera_id,
        uint16_t width, uint16_t height, uint8_t fps,
        uint8_t min_camera_id, uint8_t max_camera_id);

    void sendCurrentStateMessage(bool valid, uint8_t mode, uint8_t active_camera_id,
        uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
        uint8_t min_camera_id, uint8_t max_camera_id,
        std::optional<double> min_fps, std::optional<double> max_fps);

    void sendCameraModeMessage(uint8_t mode);
    void sendActiveCameraIdMessage(uint8_t camera_id);
    void sendImageSizeMessage(uint16_t width, uint16_t height);
    void sendFrameRateMessage(uint8_t fps);
    void sendLapseIntervalMessage(uint32_t interval_ms);
    void sendTakePhotoReplyMessage();
};


