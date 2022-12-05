/*
 * The OusterControllerNetDecoder is used to decode TCP packets
 * from the OusterPropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "Utilities.hpp"
#include "net_packet_decoder.hpp"

#include <ouster/ouster_defs.h>

#include <string>


class cOusterControllerNetDecoder : public cNetworkDecoder
{
public:

    cOusterControllerNetDecoder() = default;
    virtual ~cOusterControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryState() = 0;
    virtual void onQueryLidarMode() = 0;
    virtual void onQueryAzimuthWindow() = 0;
    virtual void setAzimuthWindow(double min_deg, double max_deg) = 0;
    virtual void setLidarMode(ouster::eLIDAR_MODE mode) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


