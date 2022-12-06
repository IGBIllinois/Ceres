/*
 * The HySpexVNIR_3000N_PropertiesNetDecoder is used to decode TCP packets 
 * from the HySpexVNIR_3000N_ControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>


class cHySpexVNIR_3000N_PropertiesNetDecoder : public cNetworkDecoder
{
public:

    cHySpexVNIR_3000N_PropertiesNetDecoder() = default;
    virtual ~cHySpexVNIR_3000N_PropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */
    virtual void onCurrentState(bool valid) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


