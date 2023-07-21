/*
 * The HySpexCamera_PropertiesNetEncoder is used to encode TCP packets
 * to send to the HySpexCamera_ControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cHySpexCamera_PropertiesNetEncoder : public cNetworkEncoder
{

public:
    cHySpexCamera_PropertiesNetEncoder(std::size_t capacity);
    ~cHySpexCamera_PropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    void sendQueryState();
    void sendQueryLensNames();
    void sendAverageFrames(std::uint32_t average_frame);
    void sendFramePeriod_us(std::uint32_t frame_period_us);
    void sendIntegrationTime_us(std::uint32_t integration_time_us);
    void sendLensName(const std::string& lens_name);
    void sendNumOfBackgrounds(int num_backgrounds);
    void sendCalcBackground();
};


