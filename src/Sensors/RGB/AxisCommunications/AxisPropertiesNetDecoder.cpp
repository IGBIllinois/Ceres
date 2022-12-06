

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
    case ePacketType::ACTIVE_CAMERA_ID:
    {
        axis_ActiveCameraIdMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto id = to_active_camera_id_t(packet);
        onCameraId(id);
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        axis_ImageSizeMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_image_size_t(packet);
        onImageSize(data.width, data.height);
        break;
    }
    case ePacketType::FRAMES_PER_SECOND:
    {
        axis_FrameRateMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto fps = to_frame_rate_t(packet);
        onFrameRate(fps);
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        axis_StateMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto state = to_current_state_t(packet);
        onCurrentState(state.valid, state.camera_id, state.width,
            state.height, state.frames_per_second);
        break;
    }
    }
}
