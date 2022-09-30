/*
 * The AxisControllerNetDecoder is used to decode TCP packets
 * from the AxisPropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>


class cAxisControllerNetDecoder : public cNetworkDecoder
{
public:

    cAxisControllerNetDecoder() = default;
    virtual ~cAxisControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryState() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


