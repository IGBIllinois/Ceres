/*
 * The HySpexSWIR_384_ControllerNetDecoder is used to decode TCP packets
 * from the HySpexSWIR_384_PropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>


class cHySpexSWIR_384_ControllerNetDecoder : public cNetworkDecoder
{
public:

    cHySpexSWIR_384_ControllerNetDecoder() = default;
    virtual ~cHySpexSWIR_384_ControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryState() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


