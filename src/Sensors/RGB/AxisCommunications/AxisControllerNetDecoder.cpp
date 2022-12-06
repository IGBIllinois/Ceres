

#include "AxisControllerNetDecoder.hpp"

#include "axis_communications_packet_utils.hpp"
#include "net_buffer.hpp"


void cAxisControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
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
        axis_QueryMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        switch (packet.query())
        {
        case eQUERY_STATE:
            return onQueryState();
        case eQUERY_ACTIVE_CAMERA_ID:
            return onQueryCameraId();
        case eQUERY_IMAGE_SIZE:
            return onQueryImageSize();
        case eQUERY_FRAME_RATE:
            return onQueryFrameRate();
        }
        break;
    }
    case ePacketType::ACTIVE_CAMERA_ID:
    {
        axis_ActiveCameraIdMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto id = to_active_camera_id_t(packet);
        setCameraId(id);
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        axis_ImageSizeMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_image_size_t(packet);
        setImageSize(data.width, data.height);
        break;
    }
    case ePacketType::FRAMES_PER_SECOND:
    {
        axis_FrameRateMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto fps = to_frame_rate_t(packet);
        setFrameRate(fps);
        break;
    }
    }
}




