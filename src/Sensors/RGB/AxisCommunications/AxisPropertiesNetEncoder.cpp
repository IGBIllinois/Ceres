

#include "AxisPropertiesNetEncoder.hpp"
#include "axis_communications_packet_utils.hpp"

using namespace axis;

cAxisPropertiesNetEncoder::cAxisPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cAxisPropertiesNetEncoder::sendQueryStateMessage()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryModeMessage()
{
    encode_query_camera_mode(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryImageSizeMessage()
{
    encode_query_image_size(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryFrameRateMessage()
{
    encode_query_frame_rate(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendQueryFrameIntervalMessage()
{
    encode_query_lapse_interval(mBuffer);
    sendData();
}


void cAxisPropertiesNetEncoder::sendSetModeMessage(uint8_t mode)
{
    encode_camera_mode(mode, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetCameraIdMessage(uint8_t id)
{
    encode_active_camera_id(id, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetImageSizeMessage(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetFrameRateMessage(uint8_t fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetLapseIntervalMessage(uint32_t interval_ms)
{
    encode_lapse_interval(interval_ms, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendGrabImageMessage()
{
    encode_grab_image(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendTakePhotoMessage(bool update_view)
{
    encode_take_photo(update_view, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendTakePhotoMessage(bool update_view, bool auto_save)
{
    encode_take_photo(update_view, auto_save, mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSaveStateMessage()
{
    encode_save_state(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendRestoreStateMessage()
{
    encode_restore_state(mBuffer);
    sendData();
}

