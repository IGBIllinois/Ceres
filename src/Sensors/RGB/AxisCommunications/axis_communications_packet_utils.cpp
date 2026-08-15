
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
    hdr.id = static_cast<uint16_t>(axis::ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int axis::encode_query_current_state(net_buffer& buffer)
{
    return encode_query(eQUERY_STATE, buffer);
}

int axis::encode_query_camera_mode(net_buffer& buffer)
{
    return encode_query(eQUERY_MODE, buffer);
}

int axis::encode_query_active_camera_id(net_buffer& buffer)
{
    return encode_query(eQUERY_ACTIVE_CAMERA_ID, buffer);
}

int axis::encode_query_image_size(net_buffer& buffer)
{
    return encode_query(eQUERY_IMAGE_SIZE, buffer);
}

int axis::encode_query_frame_rate(net_buffer& buffer)
{
    return encode_query(eQUERY_FRAME_RATE, buffer);
}

int axis::encode_query_lapse_interval(net_buffer& buffer)
{
    return encode_query(eQUERY_LAPSE_INTERVAL, buffer);
}

int axis::encode_grab_image(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::GRAB_IMAGE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t) + hdr.length;
}


uint8_t axis::to_camera_mode_t(const axis_CameraModeMessage_1& pckt)
{
    return static_cast<uint8_t>(pckt.mode());
}

int axis::encode_camera_mode(uint8_t mode, net_buffer& buffer)
{
    axis_CameraModeMessage_1 pckt;

    pckt.set_mode(mode);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CAMERA_MODE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


uint8_t axis::to_active_camera_id_t(const axis_ActiveCameraIdMessage_1& pckt)
{
    return static_cast<uint8_t>(pckt.camera_id());
}

int axis::encode_active_camera_id(uint8_t id, net_buffer& buffer)
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


axis::sImageSize axis::to_image_size_t(const axis_ImageSizeMessage_1& pckt)
{
    sImageSize image_size;
    image_size.width = pckt.width();
    image_size.height = pckt.height();
    return image_size;
}

int axis::encode_image_size(uint16_t width, uint16_t height, net_buffer& buffer)
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


uint8_t axis::to_frame_rate_t(const axis_FrameRateMessage_1& pckt)
{
    return static_cast<uint8_t>(pckt.frames_per_second());
}

int axis::encode_frame_rate(uint8_t fps, net_buffer& buffer)
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


uint32_t axis::to_lapse_interval_t(const axis_LapseIntervalMessage_1& pckt)
{
    return pckt.lapse_interval_ms();
}

int axis::encode_lapse_interval(uint32_t interval_ms, net_buffer& buffer)
{
    axis_LapseIntervalMessage_1 pckt;

    pckt.set_lapse_interval_ms(interval_ms);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::LAPSE_INTERVAL_MS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


axis::sCurrentState axis::to_current_state_t(const axis_StateMessage_1& pckt)
{
    sCurrentState state;

    state.valid = pckt.valid();
    state.mode = 0;
    state.active_camera_id = pckt.camera_id();
    state.width = pckt.width();
    state.height = pckt.height();
    state.frames_per_second = pckt.frames_per_second();
    state.lapse_interval_ms = 0;
    state.min_camera_id = 0;
    state.max_camera_id = 0;

    return state;
}

int axis::encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
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


axis::sCurrentState axis::to_current_state_t(const axis_StateMessage_2& pckt)
{
    sCurrentState state;

    state.valid = pckt.valid();
    state.mode = 0;
    state.active_camera_id = pckt.active_camera_id();
    state.width = pckt.width();
    state.height = pckt.height();
    state.frames_per_second = pckt.frames_per_second();
    state.lapse_interval_ms = 0;
    state.min_camera_id = pckt.min_camera_id();
    state.max_camera_id = pckt.max_camera_id();

    return state;
}

int axis::encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
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

axis::sCurrentState axis::to_current_state_t(const axis_StateMessage_3& pckt)
{
    sCurrentState state;

    state.valid = pckt.valid();
    state.mode = pckt.mode();
    state.active_camera_id = pckt.active_camera_id();
    state.width = pckt.width();
    state.height = pckt.height();
    state.frames_per_second = pckt.frames_per_second();
    state.lapse_interval_ms = pckt.lapse_interval_ms();
    state.min_camera_id = pckt.min_camera_id();
    state.max_camera_id = pckt.max_camera_id();

    if (pckt.has_min_frames_per_second())
        state.min_frames_per_second = pckt.min_frames_per_second();

    if (pckt.has_max_frames_per_second())
        state.max_frames_per_second = pckt.max_frames_per_second();

    return state;
}

int axis::encode_current_state(bool valid, uint8_t mode, uint8_t camera_id, uint16_t width,
    uint16_t height, uint8_t fps, uint32_t interval_ms, uint8_t min_camera_id, uint8_t max_camera_id,
    std::optional<double> min_fps, std::optional<double> max_fps, net_buffer& buffer)
{
    axis_StateMessage_3 pckt;

    pckt.set_valid(valid);
    pckt.set_mode(mode);
    pckt.set_active_camera_id(camera_id);
    pckt.set_width(width);
    pckt.set_height(height);
    pckt.set_frames_per_second(fps);
    pckt.set_lapse_interval_ms(interval_ms);
    pckt.set_min_camera_id(min_camera_id);
    pckt.set_max_camera_id(max_camera_id);

    if (min_fps.has_value())
        pckt.set_min_frames_per_second(min_fps.value());

    if (max_fps.has_value())
        pckt.set_max_frames_per_second(max_fps.value());

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CURRENT_STATE);
    hdr.revision = 3;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


bool axis::to_take_photo_t(const axis_TakePhoto_1& pckt)
{
    return pckt.update_view();
}

int axis::encode_take_photo(bool update_view, net_buffer& buffer)
{
    axis_TakePhoto_1 pckt;

    pckt.set_update_view(update_view);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::TAKE_PHOTO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

axis::sTakePhoto axis::to_take_photo_t(const axis_TakePhoto_2& pckt)
{
    sTakePhoto state;

    state.update_view = pckt.update_view();
    state.auto_save = pckt.auto_save();

    return state;
}

int axis::encode_take_photo(bool update_view, bool auto_save, net_buffer& buffer)
{
    axis_TakePhoto_2 pckt;

    pckt.set_update_view(update_view);
    pckt.set_auto_save(auto_save);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::TAKE_PHOTO);
    hdr.revision = 2;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

axis::eReply axis::to_reply_t(const axis_Reply_1& pckt)
{
    switch (pckt.reply())
    {
    case eReply_FAILED: return eReply::FAILED;
    case eReply_ABORTED: return eReply::ABORTED;
    case eReply_PENDING: return eReply::PENDING;
    }

    return eReply::GOOD;
}

int encode_reply(axis::eReply reply, axis::ePacketType packet_type, net_buffer& buffer)
{
    axis_Reply_1 pckt;

    switch (reply)
    {
    default:
    case axis::eReply::GOOD:
        pckt.set_reply(eReply_GOOD);
        break;
    case axis::eReply::FAILED:
        pckt.set_reply(eReply_FAILED);
        break;
    case axis::eReply::ABORTED:
        pckt.set_reply(eReply_ABORTED);
        break;
    case axis::eReply::PENDING:
        pckt.set_reply(eReply_PENDING);
        break;
    }

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

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
*/

int axis::encode_take_photo_reply(eReply reply, net_buffer& buffer)
{
    return encode_reply(reply, ePacketType::TAKE_PHOTO_REPLY, buffer);
}


/**********************************************************
 * Experiment State -> Controller packets utilities
 **********************************************************/

int axis::encode_save_state(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(axis::ePacketType::SAVE_STATE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t) + hdr.length;
}

int axis::encode_restore_state(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(axis::ePacketType::RESTORE_STATE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t) + hdr.length;
}
