/*
 * The HySpexVNIR_3000N_PropertiesNetDecoder is used to decode TCP packets 
 * from the HySpexVNIR_3000N_ControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "HySpexCamera_PropertiesNetDecoder.hpp"

#include <string>


class cHySpexVNIR_3000N_PropertiesNetDecoder : public cHySpexCamera_PropertiesNetDecoder
{
public:

    cHySpexVNIR_3000N_PropertiesNetDecoder() = default;
    virtual ~cHySpexVNIR_3000N_PropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */

protected:
    void processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override;
};


