

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
    case ePacketType::CAMERA_MODE:
    {
        axis_CameraModeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto id = to_camera_mode_t(packet);
            onMode(id);
        }
        break;
    }
    case ePacketType::ACTIVE_CAMERA_ID:
    {
        axis_ActiveCameraIdMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto id = to_active_camera_id_t(packet);
            onCameraId(id);
        }
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        axis_ImageSizeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_image_size_t(packet);
            onImageSize(data.width, data.height);
        }
        break;
    }
    case ePacketType::FRAMES_PER_SECOND:
    {
        axis_FrameRateMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto fps = to_frame_rate_t(packet);
            onFrameRate(fps);
        }
        break;
    }
    case ePacketType::LAPSE_INTERVAL_MS:
    {
        axis_LapseIntervalMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto interval_ms = to_lapse_interval_t(packet);
            onLapseInterval(interval_ms);
        }
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        switch (hdr.revision)
        {
        case 1:
        {
            axis_StateMessage_1 packet;
            if (packet.ParseFromArray(buffer.data(), hdr.length))
            {
                auto state = to_current_state_t(packet);
                onCurrentState(state.valid, state.active_camera_id, state.width, state.height, state.frames_per_second);
            }
            break;
        }
        case 2:
        {
            axis_StateMessage_2 packet;
            if (packet.ParseFromArray(buffer.data(), hdr.length))
            {
                auto state = to_current_state_t(packet);
                onCurrentState(state.valid, state.active_camera_id, state.width, state.height, state.frames_per_second, state.min_camera_id, state.max_camera_id);
            }
            break;
        }
        case 3:
        {
            axis_StateMessage_3 packet;
            if (packet.ParseFromArray(buffer.data(), hdr.length))
            {
                auto state = to_current_state_t(packet);
                onCurrentState(state.valid, state.mode, state.active_camera_id, state.width, state.height, 
                    state.frames_per_second, state.lapse_interval_ms, state.min_camera_id, state.max_camera_id, state.min_frames_per_second, state.max_frames_per_second);
            }
            break;
        }
        }

        break;
    }
    case ePacketType::TAKE_PHOTO_REPLY:
    {
        axis_Reply_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto reply = to_reply_t(packet);
            onTakePhotoReply(reply != eReply::GOOD);
        }
        break;
    }
    }
}
