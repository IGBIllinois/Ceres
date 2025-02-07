/*
 * The GpsControllerNetDecoder is used to decode TCP packets from 
 * the GpsPropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>

namespace gps
{
    enum class ePacketType : uint16_t;
}


class cGpsControllerNetDecoder : public cNetworkDecoder
{
public:

    cGpsControllerNetDecoder() = default;
    virtual ~cGpsControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryReferenceData() = 0;
    virtual void onQueryReferenceParameters() = 0;
    virtual void onQueryReferenceState() = 0;

    virtual void onSetReferenceParameters(std::uint16_t min_integration_time_sec, 
        std::uint16_t max_integration_time_sec, std::uint16_t error_threshold_mm) = 0;

    virtual void onCalcReference() = 0;
    virtual void onStopReference() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override;
    virtual void processPacket(gps::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) {};
};


