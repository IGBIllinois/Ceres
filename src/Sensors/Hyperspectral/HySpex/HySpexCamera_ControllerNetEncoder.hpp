/*
 * The HySpexCamera_ControllerNetEncoder is used to encode TCP packets
 * to the HySpexCamera_PropertiesNetDecoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_encoder.hpp"

#include <vector>
#include <string>

class cHySpexCamera_ControllerNetEncoder : public cNetworkEncoder
{

public:
    cHySpexCamera_ControllerNetEncoder(std::size_t capacity);
    ~cHySpexCamera_ControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
    void sendCurrentState(bool valid, std::uint16_t average_frames,
        std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
        std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
        std::uint32_t num_backgrounds, const std::string& lens_name);

    void sendLensNames(const std::vector<std::string>& names);
};


