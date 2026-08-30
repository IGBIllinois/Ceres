

#include "OusterControllerNetDecoder.hpp"

#include "ouster_packet_utils.hpp"
#include "net_buffer.hpp"

#include <ouster_connect/ouster_utils.h>

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
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            switch (packet.query())
            {
            case eQUERY_STATE:
                return onQueryStateMessage();
            case eQUERY_LIDAR_MODE:
                return onQueryLidarModeMessage();
            case eQUERY_AZIMUTH_WINDOW:
                return onQueryAzimuthWindowMessage();
            }
        }
        break;
    }
    case ePacketType::AZIMUTH_WINDOW:
    {
        ouster_AzimuthWindowMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_azimuth_window_t(packet);
            onSetAzimuthWindowMessage(data.min_deg, data.max_deg);
        }
        break;
    }
    case ePacketType::LIDAR_MODE:
    {
        ouster_LidarModeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_lidar_mode_1(packet);
            onSetLidarModeMessage(to_lidar_mode(data));
        }
        break;
    }
    }
}




