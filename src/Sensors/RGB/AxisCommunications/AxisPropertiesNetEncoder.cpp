

#include "AxisPropertiesNetEncoder.hpp"
#include "axis_communications_packet_utils.hpp"


cAxisPropertiesNetEncoder::cAxisPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cAxisPropertiesNetEncoder::sendQueryState()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetCameraId(uint8_t id)
{
    encode_active_camera_id(id, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetFrameRate_fps(uint8_t fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendGrabImage()
{
    encode_grab_image(mBuffer);
    sendData();
}

