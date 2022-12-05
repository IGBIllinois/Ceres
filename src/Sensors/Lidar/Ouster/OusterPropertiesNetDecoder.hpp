/*
 * The OusterPropertiesNetDecoder is used to decode TCP packets 
 * from the OusterControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>


class cOusterPropertiesNetDecoder : public cNetworkDecoder
{
public:

    cOusterPropertiesNetDecoder() = default;
    virtual ~cOusterPropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */
    virtual void onAzimuthWindow(double min_deg, double max_deg) = 0;
    virtual void onLidarMode(const std::string& mode) = 0;
    virtual void onCurrentState(bool valid, const std::string& mode,
        double min_deg, double max_deg) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


