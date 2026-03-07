

#include "TeledyneFlirPropertiesNetDecoder.hpp"
#include "teledyne_flir_packet_utils.hpp"
#include "net_buffer.hpp"

using namespace flir;

void cTeledyneFlirPropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
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
        teledyne_CameraModeMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto id = to_camera_mode_t(packet);
        onMode(id);
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        teledyne_ImageSizeMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_image_size_t(packet);
        onImageSize(data.width, data.height);
        break;
    }
    case ePacketType::FRAMES_PER_SECOND:
    {
        teledyne_FrameRateMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto fps = to_frame_rate_t(packet);
        onFrameRate(fps);
        break;
    }
    case ePacketType::FRAMES_INTERVAL_MS:
    {
        teledyne_FrameIntervalMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto interval_ms = to_frame_interval_t(packet);
        onFrameInterval(interval_ms);
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        teledyne_StateMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto state = to_current_state_t(packet);
        onCurrentState(state.valid, state.mode, state.width,
            state.height, state.frames_per_second, state.frames_interval_ms,
            state.min_frames_per_second, state.max_frames_per_second);
        break;
    }
    }
}
