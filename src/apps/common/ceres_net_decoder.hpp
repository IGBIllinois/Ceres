/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

class cCeresNetDecoder : public cNetworkDecoder
{
public:
    cCeresNetDecoder() = default;
    virtual ~cCeresNetDecoder() = default;

/*
 * The Ceres application sends these packets and should never receive them!
 */
private:
    void experimentInfo(const std::string&, const std::string&, const std::string&, const std::string&) override {};
    void spidercamPosition(const spidercam::sPosition& pos) override {};
    void weatherData(bool valid, double wind_speed_mph, double wind_direction_deg) override {};
};


