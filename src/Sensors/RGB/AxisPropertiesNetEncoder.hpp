/*
 * The AxisPropertiesNetEncoder is used to encode TCP packets
 * to send to the AxisControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cAxisPropertiesNetEncoder : public cNetworkEncoder
{

public:
    cAxisPropertiesNetEncoder(std::size_t capacity);
    ~cAxisPropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    void sendQueryState();
    void sendSetAzimuthWindow(double min_deg, double max_deg);
    void sendSetMode(const std::string& mode);
};


