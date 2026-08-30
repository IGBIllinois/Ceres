
/*
  * The GpsPropertiesNetDecoder is used to decode TCP packets from 
  * the GpsControllerNetEncoder.  You will need to override various
  * virtual methods!
  */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>
#include <vector>

namespace gps
{
    enum class ePacketType : uint16_t;
}


class cGpsPropertiesNetDecoder : public cNetworkDecoder
{
public:
    cGpsPropertiesNetDecoder() = default;
    virtual ~cGpsPropertiesNetDecoder() = default;

protected:
    /*
     * Controller ----> Property Page
     */
    virtual void onReferenceParametersMessage(bool valid, uint16_t min_integration_time_sec,
                                         uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm) = 0;

    virtual void onReferenceDataMessage(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
                                    double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid) = 0;

    virtual void onReferencePositionMessage(int x_mm, int y_mm, int z_mm, double error_mm, int count) = 0;

    enum class eReferenceReply { GOOD, FAILED, ABORTED, PENDING, IDLE };
    virtual void onReferenceCommandReplyMessage(eReferenceReply reply) = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
    virtual void processPacket(gps::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) = 0;
};

