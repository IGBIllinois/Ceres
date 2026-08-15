

#include "LucidRgbPropertiesNetEncoder.hpp"
#include "lucid_vision_labs_rgb_packet_utils.hpp"

using namespace lucid;

cLucidRgbPropertiesNetEncoder::cLucidRgbPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cLucidRgbPropertiesNetEncoder::sendQueryState()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendQueryMode()
{
    encode_query_camera_mode(mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendQueryImageSize()
{
    encode_query_image_size(mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendQueryFrameRate()
{
    encode_query_frame_rate(mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendQueryLapseInterval()
{
    encode_query_lapse_interval(mBuffer);
    sendData();
}


void cLucidRgbPropertiesNetEncoder::sendSetMode(uint8_t mode)
{
    encode_camera_mode(mode, mBuffer);
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

void cLucidRgbPropertiesNetEncoder::sendSetLapseInterval_ms(uint32_t interval_ms)
{
    encode_lapse_interval(interval_ms, mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendGrabImage()
{
    encode_grab_image(mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendTakePhoto(bool update_view)
{
    encode_take_photo(update_view, mBuffer);
    sendData();
}

void cLucidRgbPropertiesNetEncoder::sendTakePhoto(bool update_view, bool auto_save)
{
    encode_take_photo(update_view, auto_save, mBuffer);
    sendData();
}


