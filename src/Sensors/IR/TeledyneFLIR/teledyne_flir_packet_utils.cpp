
#include "teledyne_flir_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>


int encode_query(teledyne_eQuery query, net_buffer& buffer)
{
    teledyne_QueryMessage_1 pckt;

    pckt.set_query(query);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int flir::encode_query_current_state(net_buffer& buffer)
{
    return encode_query(eQUERY_STATE, buffer);
}

int flir::encode_query_camera_mode(net_buffer& buffer)
{
    return encode_query(eQUERY_MODE, buffer);
}

int flir::encode_query_image_size(net_buffer& buffer)
{
    return encode_query(eQUERY_IMAGE_SIZE, buffer);
}

int flir::encode_query_frame_rate(net_buffer& buffer)
{
    return encode_query(eQUERY_FRAME_RATE, buffer);
}

int flir::encode_query_frame_interval(net_buffer& buffer)
{
    return encode_query(eQUERY_FRAME_INTERVAL, buffer);
}

int flir::encode_query_thermal_range(net_buffer& buffer)
{
    return encode_query(eQUERY_THERMAL_RANGE, buffer);
}

int flir::encode_grab_image(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::GRAB_IMAGE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    return sizeof(sPacketHeader_t) + hdr.length;
}

uint8_t flir::to_camera_mode_t(const teledyne_CameraModeMessage_1& pckt)
{
    return static_cast<uint8_t>(pckt.mode());
}

int flir::encode_camera_mode(uint8_t mode, net_buffer& buffer)
{
    teledyne_CameraModeMessage_1 pckt;

    pckt.set_mode(mode);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::CAMERA_MODE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


flir::sImageSize flir::to_image_size_t(const teledyne_ImageSizeMessage_1& pckt)
{
    sImageSize image_size;
    image_size.width = pckt.width();
    image_size.height = pckt.height();
    return image_size;
}

int flir::encode_image_size(uint16_t width, uint16_t height, net_buffer& buffer)
{
    teledyne_ImageSizeMessage_1 pckt;

    pckt.set_width(width);
    pckt.set_height(height);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::IMAGE_SIZE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

double flir::to_frame_rate_t(const teledyne_FrameRateMessage_1& pckt)
{
    return pckt.frames_per_second();
}

int flir::encode_frame_rate(double fps, net_buffer& buffer)
{
    teledyne_FrameRateMessage_1 pckt;

    pckt.set_frames_per_second(fps);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::FRAME_RATE_HZ);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

uint32_t flir::to_frame_interval_t(const teledyne_FrameIntervalMessage_1& pckt)
{
    return pckt.frames_interval_ms();
}

int flir::encode_frame_interval(uint32_t interval_ms, net_buffer& buffer)
{
    teledyne_FrameIntervalMessage_1 pckt;

    pckt.set_frames_interval_ms(interval_ms);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::FRAME_INTERVAL_MS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

flir::sThermalRange flir::to_thermal_range_t(const teledyne_ThermalRangeMessage_1& pckt)
{
    sThermalRange range;

    range.min_thermal_value_K = pckt.min_thermal_value_k();
    range.max_thermal_value_K = pckt.max_thermal_value_k();

    return range;
}
int flir::encode_thermal_range(float min_value_K, float max_value_K, net_buffer& buffer)
{
    teledyne_ThermalRangeMessage_1 pckt;

    pckt.set_min_thermal_value_k(min_value_K);
    pckt.set_max_thermal_value_k(max_value_K);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::THERMAL_RANGE_K);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


flir::sCurrentState flir::to_current_state_t(const teledyne_StateMessage_1& pckt)
{
    sCurrentState state;

    state.valid = pckt.valid();
    state.mode = pckt.mode();
    state.width = pckt.width();
    state.height = pckt.height();
    state.frames_per_second = pckt.frames_per_second();
    state.frames_interval_ms = pckt.frames_interval_ms();

    if (pckt.has_min_frames_per_second())
        state.min_frames_per_second = pckt.min_frames_per_second();

    if (pckt.has_max_frames_per_second())
        state.max_frames_per_second = pckt.max_frames_per_second();

    if (pckt.has_min_thermal_value_k())
        state.min_thermal_value_K = pckt.min_thermal_value_k();

    if (pckt.has_max_thermal_value_k())
        state.max_thermal_value_K = pckt.max_thermal_value_k();

    return state;
}

int flir::encode_current_state(bool valid, uint8_t mode, uint16_t width, uint16_t height, 
    double fps, uint32_t interval_ms, std::optional<double> min_fps, std::optional<double> max_fps, 
    std::optional<float> min_K, std::optional<float> max_K, net_buffer& buffer)
{
    teledyne_StateMessage_1 pckt;

    pckt.set_valid(valid);
    pckt.set_mode(mode);
    pckt.set_width(width);
    pckt.set_height(height);
    pckt.set_frames_per_second(fps);
    pckt.set_frames_interval_ms(interval_ms);

    if (min_fps.has_value())
        pckt.set_min_frames_per_second(min_fps.value());

    if (max_fps.has_value())
        pckt.set_max_frames_per_second(max_fps.value());

    if (min_K.has_value())
        pckt.set_min_thermal_value_k(min_K.value());

    if (max_K.has_value())
        pckt.set_max_thermal_value_k(max_K.value());

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::CURRENT_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

bool flir::to_take_photo_t(const teledyne_TakePhoto_1& pckt)
{
    return pckt.update_view();
}

int flir::encode_take_photo(bool update_view, net_buffer& buffer)
{
    teledyne_TakePhoto_1 pckt;

    pckt.set_update_view(update_view);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(flir::ePacketType::TAKE_PHOTO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

flir::eReply flir::to_reply_t(const teledyne_Reply_1& pckt)
{
    switch (pckt.reply())
    {
    case eReply_FAILED: return eReply::FAILED;
    case eReply_ABORTED: return eReply::ABORTED;
    case eReply_PENDING: return eReply::PENDING;
    }

    return eReply::GOOD;
}

int encode_reply(flir::eReply reply, flir::ePacketType packet_type, net_buffer& buffer)
{
    teledyne_Reply_1 pckt;

    switch (reply)
    {
    default:
    case flir::eReply::GOOD:
        pckt.set_reply(eReply_GOOD);
        break;
    case flir::eReply::FAILED:
        pckt.set_reply(eReply_FAILED);
        break;
    case flir::eReply::ABORTED:
        pckt.set_reply(eReply_ABORTED);
        break;
    case flir::eReply::PENDING:
        pckt.set_reply(eReply_PENDING);
        break;
    }

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(packet_type);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

/*
int flir::encode_camera_mode_reply(eReply reply, net_buffer& buffer)
{
    return encode_reply(reply, flir::ePacketType::CAMERA_MODE_REPLY, buffer);
}

int flir::encode_image_size_reply(eReply reply, net_buffer& buffer)
{
    return encode_reply(reply, flir::ePacketType::IMAGE_SIZE_REPLY, buffer);
}

int flir::encode_frame_rate_reply(eReply reply, net_buffer& buffer)
{
    return encode_reply(reply, flir::ePacketType::FRAME_RATE_REPLY, buffer);
}

int flir::encode_frame_interval_reply(eReply reply, net_buffer& buffer)
{
    return encode_reply(reply, flir::ePacketType::FRAME_INTERVAL_REPLY, buffer);
}

int flir::encode_thermal_range_reply(eReply reply, net_buffer& buffer)
{
    return encode_reply(reply, flir::ePacketType::THERMAL_RANGE_REPLY, buffer);
}
*/

int flir::encode_take_photo_reply(eReply reply, net_buffer& buffer)
{
    return encode_reply(reply, flir::ePacketType::TAKE_PHOTO_REPLY, buffer);
}

