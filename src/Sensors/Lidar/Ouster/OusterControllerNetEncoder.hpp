
#pragma once

#include "net_packet_encoder.hpp"

#include <ouster_connect/ouster_defs.h>


class cOusterControllerNetEncoder : public cNetworkEncoder
{

public:
    cOusterControllerNetEncoder(std::size_t capacity);
    ~cOusterControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
    void sendLidarModeMessage(ouster::eLIDAR_MODE mode);
    void sendAzimuthWindowMessage(double min_deg, double max_deg);
    void sendCurrentStateMessage(bool valid, ouster::eLIDAR_MODE mode,
        double min_deg, double max_deg);
};


