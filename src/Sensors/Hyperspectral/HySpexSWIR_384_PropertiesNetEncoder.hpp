/*
 * The HySpexSWIR_384_PropertiesNetEncoder is used to encode TCP packets
 * to send to the HySpexSWIR_384_ControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cHySpexSWIR_384_PropertiesNetEncoder : public cNetworkEncoder
{

public:
    cHySpexSWIR_384_PropertiesNetEncoder(std::size_t capacity);
    ~cHySpexSWIR_384_PropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    void sendQueryState();
};


