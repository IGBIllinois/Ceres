/*
 * The HySpexVNIR_3000N_ControllerNetDecoder is used to decode TCP packets
 * from the HySpexVNIR_3000N_PropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>


class cHySpexVNIR_3000N_ControllerNetDecoder : public cNetworkDecoder
{
public:

    cHySpexVNIR_3000N_ControllerNetDecoder() = default;
    virtual ~cHySpexVNIR_3000N_ControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryState() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


