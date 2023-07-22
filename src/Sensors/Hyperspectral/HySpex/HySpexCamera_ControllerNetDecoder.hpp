/*
 * The HySpexCamera_ControllerNetDecoder is used to decode TCP packets
 * from the HySpexCamera_PropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>


class cHySpexCamera_ControllerNetDecoder : public cNetworkDecoder
{
public:

    cHySpexCamera_ControllerNetDecoder() = default;
    virtual ~cHySpexCamera_ControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryState() = 0;
    virtual void onQueryLensNames() = 0;
    virtual void onSetAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us) = 0;
    virtual void onSetLensName(const std::string& lens_name) = 0;
    virtual void onSetNumOfBackgrounds(int num_backgrounds) = 0;
    virtual void onCalcBackground() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


