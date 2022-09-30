
#pragma once

#include "net_packet_encoder.hpp"


class cAxisControllerNetEncoder : public cNetworkEncoder
{

public:
    cAxisControllerNetEncoder(std::size_t capacity);
    ~cAxisControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */
//    void sendCurrentState(bool valid, ouster::eLIDAR_MODE mode,
//        double min_deg, double max_deg);
};


