
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

int flir::encode_grab_image(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::GRAB_IMAGE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

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
    hdr.id = static_cast<uint16_t>(flir::ePacketType::FRAMES_PER_SECOND);
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
    hdr.id = static_cast<uint16_t>(flir::ePacketType::FRAMES_INTERVAL_MS);
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

    return state;
}

int flir::encode_current_state(bool valid, uint8_t mode, uint16_t width, uint16_t height, 
    double fps, uint32_t interval_ms, std::optional<double> min_fps, std::optional<double> max_fps, net_buffer& buffer)
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



