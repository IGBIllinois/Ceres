/*
 * The OusterPropertiesNetEncoder is used to encode TCP packets
 * to send to the OusterControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cOusterPropertiesNetEncoder : public cNetworkEncoder
{

public:
    cOusterPropertiesNetEncoder(std::size_t capacity);
    ~cOusterPropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    void sendQueryState();
    void sendSetAzimuthWindow(double min_deg, double max_deg);
    void sendSetMode(const std::string& mode);
};


