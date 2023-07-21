/*
 * The HySpexVNIR_3000N_ControllerNetDecoder is used to decode TCP packets
 * from the HySpexVNIR_3000N_PropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "HySpexCamera_ControllerNetDecoder.hpp"

#include <string>


class cHySpexVNIR_3000N_ControllerNetDecoder : public cHySpexCamera_ControllerNetDecoder
{
public:

    cHySpexVNIR_3000N_ControllerNetDecoder() = default;
    virtual ~cHySpexVNIR_3000N_ControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */

protected:
//    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


