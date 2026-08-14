

#include "AxisPropertiesNetEncoder.hpp"
#include "axis_communications_packet_utils.hpp"

using namespace axis;

cAxisPropertiesNetEncoder::cAxisPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cAxisPropertiesNetEncoder::sendQueryState()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryMode()
{
    encode_query_camera_mode(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryImageSize()
{
    encode_query_image_size(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryFrameRate()
{
    encode_query_frame_rate(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryFrameInterval()
{
    encode_query_lapse_interval(mBuffer);
    sendData();
}


void cAxisPropertiesNetEncoder::sendSetMode(uint8_t mode)
{
    encode_camera_mode(mode, mBuffer);
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

void cAxisPropertiesNetEncoder::sendSetLapseInterval_ms(uint32_t interval_ms)
{
    encode_lapse_interval(interval_ms, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendGrabImage()
{
    encode_grab_image(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendTakePhoto(bool update_view)
{
    encode_take_photo(update_view, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendTakePhoto(bool update_view, bool auto_save)
{
    encode_take_photo(update_view, auto_save, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSaveState()
{
    encode_save_state(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendRestoreState()
{
    encode_restore_state(mBuffer);
    sendData();
}

