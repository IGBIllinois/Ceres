

#include "TeledyneFlirControllerNetDecoder.hpp"

#include "teledyne_flir_packet_utils.hpp"
#include "net_buffer.hpp"


void cTeledyneFlirControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
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
        teledyne_QueryMessage_1 packet;
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
        teledyne_ActiveCameraIdMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto id = to_active_camera_id_t(packet);
        setCameraId(id);
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        teledyne_ImageSizeMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_image_size_t(packet);
        setImageSize(data.width, data.height);
        break;
    }
    case ePacketType::FRAMES_PER_SECOND:
    {
        teledyne_FrameRateMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto fps = to_frame_rate_t(packet);
        setFrameRate(fps);
        break;
    }
    case ePacketType::GRAB_IMAGE:
        onGrabImage();
        break;
    }
}




