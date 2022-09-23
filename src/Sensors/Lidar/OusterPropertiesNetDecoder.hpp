/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once

#include "Utilities.hpp"
#include "net_packet_decoder.hpp"


class cOusterPropertiesNetDecoder : public cNetworkDecoder
{
public:

    cOusterPropertiesNetDecoder() = default;
    virtual ~cOusterPropertiesNetDecoder() = default;

/*
 * The Ceres Remote Client application sends these packets and should never receive them!
 */
private:
};


