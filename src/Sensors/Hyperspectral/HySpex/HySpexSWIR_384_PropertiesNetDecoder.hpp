/*
 * The HySpexSWIR_384_PropertiesNetDecoder is used to decode TCP packets 
 * from the HySpexSWIR_384_ControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "HySpexCamera_PropertiesNetDecoder.hpp"

#include <string>


class cHySpexSWIR_384_PropertiesNetDecoder : public cHySpexCamera_PropertiesNetDecoder
{
public:

    cHySpexSWIR_384_PropertiesNetDecoder() = default;
    virtual ~cHySpexSWIR_384_PropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */

protected:
    void processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override;
};


