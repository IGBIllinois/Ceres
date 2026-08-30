

#include "OusterPropertiesNetDecoder.hpp"
#include "ouster_packet_utils.hpp"
#include "net_buffer.hpp"


void cOusterPropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::AZIMUTH_WINDOW:
    {
        ouster_AzimuthWindowMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_azimuth_window_t(packet);
            onAzimuthWindowMessage(data.min_deg, data.max_deg);
        }
        break;
    }
    case ePacketType::LIDAR_MODE:
    {
        ouster_LidarModeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto mode = to_lidar_mode_1(packet);
            onLidarModeMessage(mode);
        }
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        ouster_StateMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
            onCurrentStateMessage(packet.valid(), packet.mode(), packet.azimuth_min_deg(), packet.azimuth_max_deg());
        break;
    }
    }
}
