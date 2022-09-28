

#include "OusterControllerNetDecoder.hpp"

#include "ouster_packet_utils.hpp"
#include "net_buffer.hpp"

#include <ouster/ouster_utils.h>

void cOusterControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::QUERY_STATE:
    {
        onQueryState();
        break;
    }
    case ePacketType::SET_AZIMUTH_WINDOW:
    {
        SetAzimuthWindow_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_azimuth_window_t(packet);
        onSetAzimuthWindow(data.min_deg, data.max_deg);
        break;
    }
    case ePacketType::SET_LIDAR_MODE:
    {
        SetLidarMode_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_lidar_mode_1(packet);
        onSetMode(to_lidar_mode(data));
        break;
    }
    }
}




