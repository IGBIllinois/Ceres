

#include "AxisControllerNetEncoder.hpp"
#include "axis_communications_packet_utils.hpp"


cAxisControllerNetEncoder::cAxisControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cAxisControllerNetEncoder::sendCurrentState(bool valid, uint8_t camera_id,
    uint16_t width, uint16_t height, uint8_t fps)
{
    encode_current_state(valid, camera_id, width, height, fps, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendCurrentState(bool valid, uint8_t active_camera_id,
    uint16_t width, uint16_t height, uint8_t fps,
    uint8_t min_camera_id, uint8_t max_camera_id)
{
    encode_current_state(valid, active_camera_id, width, height, fps, min_camera_id, max_camera_id, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendCurrentState(bool valid, uint8_t mode, uint8_t active_camera_id,
    uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
    uint8_t min_camera_id, uint8_t max_camera_id,
    std::optional<double> min_fps, std::optional<double> max_fps)
{
    encode_current_state(valid, mode, active_camera_id, width, height, fps, interval_ms, 
        min_camera_id, max_camera_id, min_fps, max_fps, mBuffer);

    sendData();
}

void cAxisControllerNetEncoder::sendCameraMode(uint8_t mode)
{
    encode_camera_mode(mode, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendActiveCameraId(uint8_t camera_id)
{
    encode_active_camera_id(camera_id, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendFrameRate(uint8_t fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendLapseInterval_ms(uint32_t interval_ms)
{
    encode_lapse_interval(interval_ms, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendTakePhotoReply()
{
    encode_take_photo_reply(eReply::GOOD, mBuffer);
    sendData();
}
