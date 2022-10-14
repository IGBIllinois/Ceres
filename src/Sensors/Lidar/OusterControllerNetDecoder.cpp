

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
        ouster_QueryMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        switch (packet.query())
        {
        case eQUERY_STATE:
            return onQueryState();
        case eQUERY_LIDAR_MODE:
            return onQueryLidarMode();
        case eQUERY_AZIMUTH_WINDOW:
            return onQueryAzimuthWindow();
        }
        break;
    }
    case ePacketType::AZIMUTH_WINDOW:
    {
        ouster_AzimuthWindowMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_azimuth_window_t(packet);
        setAzimuthWindow(data.min_deg, data.max_deg);
        break;
    }
    case ePacketType::LIDAR_MODE:
    {
        ouster_LidarModeMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_lidar_mode_1(packet);
        setLidarMode(to_lidar_mode(data));
        break;
    }
    }
}




