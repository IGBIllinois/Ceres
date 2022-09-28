
#pragma once

#include "net_packet_encoder.hpp"

#include <ouster/ouster_defs.h>


class cOusterControllerNetEncoder : public cNetworkEncoder
{

public:
    cOusterControllerNetEncoder(std::size_t capacity);
    ~cOusterControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
    void sendCurrentState(bool valid, ouster::eLIDAR_MODE mode,
        double min_deg, double max_deg);
};


