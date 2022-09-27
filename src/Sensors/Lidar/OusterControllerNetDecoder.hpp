/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once

#include "Utilities.hpp"
#include "net_packet_decoder.hpp"

#include <string>


class cOusterControllerNetDecoder : public cNetworkDecoder
{
public:

    cOusterControllerNetDecoder() = default;
    virtual ~cOusterControllerNetDecoder() = default;

protected:
    virtual void onQueryState() = 0;
    virtual void onSetAzimuthWindow(double min_deg, double max_deg) = 0;
    virtual void onSetMode(const std::string& mode) = 0;

private:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


