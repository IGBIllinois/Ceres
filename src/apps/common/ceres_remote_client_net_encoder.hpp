
#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cCeresRemoteClientNetEncoder : public cNetworkEncoder
{

public:
    cCeresRemoteClientNetEncoder(std::size_t capacity);
    ~cCeresRemoteClientNetEncoder() = default;

    /*
     * Ceres Remote Client ----> Ceres
     */
    void sendDataFileState(bool is_open);
};


