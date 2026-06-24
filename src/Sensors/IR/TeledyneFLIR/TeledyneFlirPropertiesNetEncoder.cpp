

#include "TeledyneFlirPropertiesNetEncoder.hpp"
#include "teledyne_flir_packet_utils.hpp"

using namespace flir;

cTeledyneFlirPropertiesNetEncoder::cTeledyneFlirPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cTeledyneFlirPropertiesNetEncoder::sendQueryState()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryMode()
{
    encode_query_camera_mode(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryImageSize()
{
    encode_query_image_size(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryFrameRate()
{
    encode_query_frame_rate(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryFrameInterval()
{
    encode_query_lapse_interval(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryThermalRange()
{
    encode_query_thermal_range(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetMode(uint8_t mode)
{
    encode_camera_mode(mode, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetFrameRate_fps(double fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetLapseInterval_ms(uint32_t interval_ms)
{
    encode_lapse_interval(interval_ms, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendGrabImage()
{
    encode_grab_image(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendTakePhoto(bool update_view)
{
    encode_take_photo(update_view, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendTakePhoto(bool update_view, bool auto_save)
{
    encode_take_photo(update_view, auto_save, mBuffer);
    sendData();
}

