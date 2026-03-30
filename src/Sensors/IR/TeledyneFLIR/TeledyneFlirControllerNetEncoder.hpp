
#pragma once

#include "net_packet_encoder.hpp"

#include <optional>
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
    void sendCurrentState(bool valid, uint8_t mode,
        uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
        std::optional<double> min_fps, std::optional<double> max_fps,
        std::optional<float> min_K, std::optional<float> max_K);

    void sendCameraMode(uint8_t mode);
    void sendImageSize(uint16_t width, uint16_t height);
    void sendFrameRate_Hz(double fps);
    void sendLapseInterval_ms(uint32_t interval_ms);
    void sendThermalRange_K(float min_value_K, float max_value_K);
    void sendTakePhotoReply();
};


