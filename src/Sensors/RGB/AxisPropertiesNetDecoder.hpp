/*
 * The AxisPropertiesNetDecoder is used to decode TCP packets 
 * from the AxisControllerNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>


class cAxisPropertiesNetDecoder : public cNetworkDecoder
{
public:

    cAxisPropertiesNetDecoder() = default;
    virtual ~cAxisPropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */
    virtual void onCurrentState(bool valid, const std::string& mode,
        double min_deg, double max_deg) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


