

#include "TeledyneFlirPropertiesNetEncoder.hpp"
#include "teledyne_flir_packet_utils.hpp"


cTeledyneFlirPropertiesNetEncoder::cTeledyneFlirPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cTeledyneFlirPropertiesNetEncoder::sendQueryState()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetCameraId(uint8_t id)
{
    encode_active_camera_id(id, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetFrameRate_fps(uint8_t fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendGrabImage()
{
    encode_grab_image(mBuffer);
    sendData();
}

