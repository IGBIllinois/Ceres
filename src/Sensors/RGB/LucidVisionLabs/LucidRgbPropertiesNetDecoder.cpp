

#include "LucidRgbPropertiesNetDecoder.hpp"
#include "lucid_vision_labs_rgb_packet_utils.hpp"
#include "net_buffer.hpp"


void cLucidRgbPropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    using namespace lucid;

    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        lucid_ImageSizeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_image_size_t(packet);
            onImageSizeMessage(data.width, data.height);
        }
        break;
    }
    case ePacketType::FRAME_RATE_HZ:
    {
        lucid_FrameRateMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto fps = to_frame_rate_t(packet);
            onFrameRateMessage(fps);
        }
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        switch (hdr.revision)
        {
        case 1:
        {
            lucid_StateMessage_1 packet;
            if (packet.ParseFromArray(buffer.data(), hdr.length))
            {
                auto state = to_current_state_t(packet);
                onCurrentStateMessage(state.valid, state.active_camera_id, state.width, state.height, state.frames_per_second);
            }
            break;
        }
        case 2:
        {
            lucid_StateMessage_2 packet;
            if (packet.ParseFromArray(buffer.data(), hdr.length))
            {
                auto state = to_current_state_t(packet);
                onCurrentStateMessage(state.valid, state.active_camera_id, state.width, state.height, state.frames_per_second, state.min_camera_id, state.max_camera_id);
            }
            break;
        }
        }

        break;
    }
    }
}
