/*
 * The HySpexSWIR_384_ControllerNetEncoder is used to encode TCP packets
 * to the HySpexSWIR_384_PropertiesNetDecoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_encoder.hpp"


class cHySpexSWIR_384_ControllerNetEncoder : public cNetworkEncoder
{

public:
    cHySpexSWIR_384_ControllerNetEncoder(std::size_t capacity);
    ~cHySpexSWIR_384_ControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
//    void sendCurrentState(bool valid, ouster::eLIDAR_MODE mode,
//        double min_deg, double max_deg);
};


