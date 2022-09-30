

#include "AxisPropertiesNetDecoder.hpp"
#include "axis_communications_packet_utils.hpp"
#include "net_buffer.hpp"


void cAxisPropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
/*
        CurrentState_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onCurrentState(packet.valid(), packet.mode(),
            packet.azimuth_min_deg(), packet.azimuth_max_deg());
*/
        break;
    }
    }
}
