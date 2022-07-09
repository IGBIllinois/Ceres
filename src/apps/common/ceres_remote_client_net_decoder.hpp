/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"


class cCeresRemoteClientNetDecoder : public cNetworkDecoder
{
public:

    cCeresRemoteClientNetDecoder() = default;
    virtual ~cCeresRemoteClientNetDecoder() = default;

/*
 * The Ceres Remote Client application sends these packets and should never receive them!
 */
private:
};


