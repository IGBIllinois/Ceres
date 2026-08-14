

#include "AxisControllerNetDecoder.hpp"

#include "axis_communications_packet_utils.hpp"
#include "net_buffer.hpp"


void cAxisControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    using namespace axis;

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
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            switch (packet.query())
            {
            case eQUERY_STATE:
                return onQueryState();
            case eQUERY_MODE:
                return onQueryMode();
            case eQUERY_ACTIVE_CAMERA_ID:
                return onQueryCameraId();
            case eQUERY_IMAGE_SIZE:
                return onQueryImageSize();
            case eQUERY_FRAME_RATE:
                return onQueryFrameRate();
            case eQUERY_LAPSE_INTERVAL:
                return onQueryLapseInterval();
            }
        }
        break;
    }
    case ePacketType::CAMERA_MODE:
    {
        axis_CameraModeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto mode = to_camera_mode_t(packet);
            setMode(mode);
        }
        break;
    }
    case ePacketType::ACTIVE_CAMERA_ID:
    {
        axis_ActiveCameraIdMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto id = to_active_camera_id_t(packet);
            setCameraId(id);
        }
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        axis_ImageSizeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_image_size_t(packet);
            setImageSize(data.width, data.height);
        }
        break;
    }
    case ePacketType::FRAMES_PER_SECOND:
    {
        axis_FrameRateMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto fps = to_frame_rate_t(packet);
            setFrameRate(fps);
        }
        break;
    }
    case ePacketType::LAPSE_INTERVAL_MS:
    {
        axis_LapseIntervalMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto interval_ms = to_lapse_interval_t(packet);
            setLapseInterval_ms(interval_ms);
        }
        break;
    }
    case ePacketType::GRAB_IMAGE:
        onGrabImage();
        break;
    case ePacketType::TAKE_PHOTO:
    {
        if (hdr.revision == 1)
        {
            axis_TakePhoto_1 packet;
            if (packet.ParseFromArray(buffer.data(), hdr.length))
            {
                auto update_view = to_take_photo_t(packet);
                onTakePhoto(update_view);
            }
        }
        else if (hdr.revision == 2)
        {
            axis_TakePhoto_2 packet;
            if (packet.ParseFromArray(buffer.data(), hdr.length))
            {
                auto result = to_take_photo_t(packet);
                onTakePhoto(result.update_view, result.auto_save);
            }
        }
        break;
    }
    case ePacketType::SAVE_STATE:
        onSaveState();
        break;
    case ePacketType::RESTORE_STATE:
        onRestoreState();
        break;
    }
}




