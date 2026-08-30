

#include "LucidRgbControllerNetDecoder.hpp"

#include "lucid_vision_labs_rgb_packet_utils.hpp"
#include "net_buffer.hpp"

using namespace lucid;


void cLucidRgbControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
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
        lucid_QueryMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            switch (packet.query())
            {
            case eQUERY_STATE:
                return onQueryStateMessage();
            case eQUERY_IMAGE_SIZE:
                return onQueryImageSizeMessage();
            case eQUERY_FRAME_RATE:
                return onQueryFrameRateMessage();
            }
        }
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        lucid_ImageSizeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_image_size_t(packet);
            setImageSizeMessage(data.width, data.height);
        }
        break;
    }
    case ePacketType::FRAME_RATE_HZ:
    {
        lucid_FrameRateMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto fps = to_frame_rate_t(packet);
            setFrameRateMessage(fps);
        }
        break;
    }
    case ePacketType::GRAB_IMAGE:
        onGrabImageMessage();
        break;
    case ePacketType::SAVE_STATE:
        onSaveStateMessage();
        break;
    case ePacketType::RESTORE_STATE:
        onRestoreStateMessage();
        break;
    }
}




