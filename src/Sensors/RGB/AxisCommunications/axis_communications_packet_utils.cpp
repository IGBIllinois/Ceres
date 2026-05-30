
#include "axis_communications_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>

int encode_query(axis_eQuery query, net_buffer& buffer)
{
    axis_QueryMessage_1 pckt;

    pckt.set_query(query);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int encode_query_current_state(net_buffer& buffer)
{
    return encode_query(eQUERY_STATE, buffer);
}

int encode_query_active_camera_id(net_buffer& buffer)
{
    return encode_query(eQUERY_ACTIVE_CAMERA_ID, buffer);
}

int encode_query_image_size(net_buffer& buffer)
{
    return encode_query(eQUERY_IMAGE_SIZE, buffer);
}

int encode_query_frame_rate(net_buffer& buffer)
{
    return encode_query(eQUERY_FRAME_RATE, buffer);
}

int encode_grab_image(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::GRAB_IMAGE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t) + hdr.length;
}

uint8_t to_active_camera_id_t(const axis_ActiveCameraIdMessage_1& pckt)
{
    return static_cast<uint8_t>(pckt.camera_id());
}

int encode_active_camera_id(uint8_t id, net_buffer& buffer)
{
    axis_ActiveCameraIdMessage_1 pckt;

    pckt.set_camera_id(id);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::ACTIVE_CAMERA_ID);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


sImageSize to_image_size_t(const axis_ImageSizeMessage_1& pckt)
{
    sImageSize image_size;
    image_size.width = pckt.width();
    image_size.height = pckt.height();
    return image_size;
}

int encode_image_size(uint16_t width, uint16_t height, net_buffer& buffer)
{
    axis_ImageSizeMessage_1 pckt;

    pckt.set_width(width);
    pckt.set_height(height);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::IMAGE_SIZE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

uint8_t to_frame_rate_t(const axis_FrameRateMessage_1& pckt)
{
    return static_cast<uint8_t>(pckt.frames_per_second());
}

int encode_frame_rate(uint8_t fps, net_buffer& buffer)
{
    axis_FrameRateMessage_1 pckt;

    pckt.set_frames_per_second(fps);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::FRAMES_PER_SECOND);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


sCurrentState to_current_state_t(const axis_StateMessage_1& pckt)
{
    sCurrentState state;

    state.valid = pckt.valid();
    state.active_camera_id = pckt.camera_id();
    state.width = pckt.width();
    state.height = pckt.height();
    state.frames_per_second = pckt.frames_per_second();
    state.min_camera_id = 0;
    state.max_camera_id = 0;

    return state;
}

int encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
    uint16_t height, uint8_t fps, net_buffer& buffer)
{
    axis_StateMessage_1 pckt;

    pckt.set_valid(valid);
    pckt.set_camera_id(camera_id);
    pckt.set_width(width);
    pckt.set_height(height);
    pckt.set_frames_per_second(fps);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CURRENT_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


sCurrentState to_current_state_t(const axis_StateMessage_2& pckt)
{
    sCurrentState state;

    state.valid = pckt.valid();
    state.active_camera_id = pckt.active_camera_id();
    state.width = pckt.width();
    state.height = pckt.height();
    state.frames_per_second = pckt.frames_per_second();
    state.min_camera_id = pckt.min_camera_id();
    state.max_camera_id = pckt.max_camera_id();

    return state;
}

int encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
    uint16_t height, uint8_t fps, uint8_t min_camera_id, uint8_t max_camera_id, net_buffer& buffer)
{
    axis_StateMessage_2 pckt;

    pckt.set_valid(valid);
    pckt.set_active_camera_id(camera_id);
    pckt.set_width(width);
    pckt.set_height(height);
    pckt.set_frames_per_second(fps);
    pckt.set_min_camera_id(min_camera_id);
    pckt.set_max_camera_id(max_camera_id);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CURRENT_STATE);
    hdr.revision = 2;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


