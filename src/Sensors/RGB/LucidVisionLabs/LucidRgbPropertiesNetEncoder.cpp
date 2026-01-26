

#include "LucidRgbPropertiesNetEncoder.hpp"
#include "lucid_vision_labs_rgb_packet_utils.hpp"


cLucidRgbPropertiesNetEncoder::cLucidRgbPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cLucidRgbPropertiesNetEncoder::sendQueryState()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendSetCameraId(uint8_t id)
{
    encode_active_camera_id(id, mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendSetImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendSetFrameRate_fps(uint8_t fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendGrabImage()
{
    encode_grab_image(mBuffer);
    sendData();
}

