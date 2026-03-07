

#include "TeledyneFlirControllerNetDecoder.hpp"

#include "teledyne_flir_packet_utils.hpp"
#include "net_buffer.hpp"

using namespace flir;

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
        case eQUERY_MODE:
            return onQueryMode();
        case eQUERY_IMAGE_SIZE:
            return onQueryImageSize();
        case eQUERY_FRAME_RATE:
            return onQueryFrameRate();
        case eQUERY_FRAME_INTERVAL:
            return onQueryFrameInterval();
        }
        break;
    }
    case ePacketType::CAMERA_MODE:
    {
        teledyne_CameraModeMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto mode = to_camera_mode_t(packet);
        setMode(mode);
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
        setFrameRate_Hz(fps);
        break;
    }
    case ePacketType::FRAMES_INTERVAL_MS:
    {
        teledyne_FrameIntervalMessage_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto interval_ms = to_frame_interval_t(packet);
        setFrameInterval_ms(interval_ms);
        break;
    }
    case ePacketType::GRAB_IMAGE:
        onGrabImage();
        break;
    }
}




