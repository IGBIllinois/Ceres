/*
 * The HySpexSWIR_384_ControllerNetDecoder is used to decode TCP packets
 * from the HySpexSWIR_384_PropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "HySpexCamera_ControllerNetDecoder.hpp"

#include <string>


class cHySpexSWIR_384_ControllerNetDecoder : public cHySpexCamera_ControllerNetDecoder
{
public:

    cHySpexSWIR_384_ControllerNetDecoder() = default;
    virtual ~cHySpexSWIR_384_ControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */

protected:
    void processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override;
};


