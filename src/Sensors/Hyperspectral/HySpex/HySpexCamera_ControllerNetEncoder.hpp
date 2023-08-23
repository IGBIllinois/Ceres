/*
 * The HySpexCamera_ControllerNetEncoder is used to encode TCP packets
 * to the HySpexCamera_PropertiesNetDecoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_encoder.hpp"
#include "hyspex_properties.pb.h"

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

     /*
      * The encode methods will encode the packet into the data buffer but will not send the
      * data unless the packet will not fit into the buffer.
      *
      * You need to call sendData to make sure the data is sent out!
      */
    void encodeCurrentState(bool valid, std::uint16_t average_frames,
        std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
        std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
        std::uint32_t num_backgrounds, const std::string& lens_name);

    void encodeLensNames(const std::vector<std::string>& names);

    void encodeCommandReply(hyspex_eCommand reply);

    void encodeBackgroundReply(hyspex_eBackgroundReply reply);

    void encodeShutterStateReply(hyspex_eShutterState state);


    /*
     * The send methods will encode the packet into the data buffer and will automatically
     * send the data.
     */
    void sendCurrentState(bool valid, std::uint16_t average_frames,
        std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
        std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
        std::uint32_t num_backgrounds, const std::string& lens_name);

    void sendLensNames(const std::vector<std::string>& names);

    void sendCommandReply(hyspex_eCommand reply);

    void sendBackgroundReply(hyspex_eBackgroundReply reply);

    void sendShutterStateReply(hyspex_eShutterState state);
};


