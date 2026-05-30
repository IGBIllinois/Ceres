

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
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
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
            case eQUERY_LAPSE_INTERVAL:
                return onQueryLapseInterval();
            case eQUERY_THERMAL_RANGE:
                return onQueryThermalRange();
            }
        }
        break;
    }
    case ePacketType::CAMERA_MODE:
    {
        teledyne_CameraModeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto mode = to_camera_mode_t(packet);
            setMode(mode);
        }
        break;
    }
    case ePacketType::IMAGE_SIZE:
    {
        teledyne_ImageSizeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto data = to_image_size_t(packet);
            setImageSize(data.width, data.height);
        }
        break;
    }
    case ePacketType::FRAME_RATE_HZ:
    {
        teledyne_FrameRateMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto fps = to_frame_rate_t(packet);
            setFrameRate_Hz(fps);
        }
        break;
    }
    case ePacketType::LAPSE_INTERVAL_MS:
    {
        teledyne_LapseIntervalMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto interval_ms = to_lapse_interval_t(packet);
            setLapseInterval_ms(interval_ms);
        }
        break;
    }
    case ePacketType::THERMAL_RANGE_K:
    {
        teledyne_ThermalRangeMessage_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto range_K = to_thermal_range_t(packet);
            setThermalRange_K(range_K.min_thermal_value_K, range_K.max_thermal_value_K);
        }
        break;
    }
    case ePacketType::GRAB_IMAGE:
        onGrabImage();
        break;
    case ePacketType::TAKE_PHOTO:
    {
        teledyne_TakePhoto_1 packet;
        if (packet.ParseFromArray(buffer.data(), hdr.length))
        {
            auto update_view = to_take_photo_t(packet);
            onTakePhoto(update_view);
        }
        break;
    }
}
}




