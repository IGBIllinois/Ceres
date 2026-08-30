

#include "TeledyneFlirPropertiesNetEncoder.hpp"
#include "teledyne_flir_packet_utils.hpp"

using namespace flir;

cTeledyneFlirPropertiesNetEncoder::cTeledyneFlirPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cTeledyneFlirPropertiesNetEncoder::sendQueryStateMessage()
{
    encode_query_current_state(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryModeMessage()
{
    encode_query_camera_mode(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryImageSizeMessage()
{
    encode_query_image_size(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryFrameRateMessage()
{
    encode_query_frame_rate(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryFrameIntervalMessage()
{
    encode_query_lapse_interval(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendQueryThermalRangeMessage()
{
    encode_query_thermal_range(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetModeMessage(uint8_t mode)
{
    encode_camera_mode(mode, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetImageSizeMessage(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetFrameRateMessage(double fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSetLapseIntervalMessage(uint32_t interval_ms)
{
    encode_lapse_interval(interval_ms, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendGrabImageMessage()
{
    encode_grab_image(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendTakePhotoMessage(bool update_view)
{
    encode_take_photo(update_view, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendTakePhotoMessage(bool update_view, bool auto_save)
{
    encode_take_photo(update_view, auto_save, mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendSaveStateMessage()
{
    encode_save_state(mBuffer);
    sendData();
}

void cTeledyneFlirPropertiesNetEncoder::sendRestoreStateMessage()
{
    encode_restore_state(mBuffer);
    sendData();
}

