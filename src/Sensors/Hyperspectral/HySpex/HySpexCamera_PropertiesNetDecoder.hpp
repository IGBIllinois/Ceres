/*
 * The HySpexCamera_PropertiesNetDecoder is used to decode TCP packets 
 * from the HySpexCamera_ControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>
#include <vector>


class cHySpexCamera_PropertiesNetDecoder : public cNetworkDecoder
{
public:

    cHySpexCamera_PropertiesNetDecoder() = default;
    virtual ~cHySpexCamera_PropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */
    virtual void onCurrentState(bool valid, std::uint16_t average_frames,
        std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
        std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
        std::uint32_t num_backgrounds, const std::string& lens_name) = 0;

    virtual void onLensNames(const std::vector<std::string>& names) = 0;

    enum class eBackgroundReply {GOOD, FAILED, ABORTED};
    virtual void onBackgroundReply(eBackgroundReply reply) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


