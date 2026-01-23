

#include "LucidRgbControllerNetEncoder.hpp"
#include "lucid_vision_labs_rgb_packet_utils.hpp"


cLucidRgbControllerNetEncoder::cLucidRgbControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cLucidRgbControllerNetEncoder::sendCurrentState(bool valid, uint8_t camera_id,
    uint16_t width, uint16_t height, uint8_t fps)
{
    encode_current_state(valid, camera_id, width, height, fps, mBuffer);
    sendData();
}

void cLucidRgbControllerNetEncoder::sendCurrentState(bool valid, uint8_t active_camera_id,
    uint16_t width, uint16_t height, uint8_t fps,
    uint8_t min_camera_id, uint8_t max_camera_id)
{
    encode_current_state(valid, active_camera_id, width, height, fps, min_camera_id, max_camera_id, mBuffer);
    sendData();
}

void cLucidRgbControllerNetEncoder::sendActiveCameraId(uint8_t camera_id)
{
    encode_active_camera_id(camera_id, mBuffer);
    sendData();
}

void cLucidRgbControllerNetEncoder::sendImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cLucidRgbControllerNetEncoder::sendFrameRate(uint8_t fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

