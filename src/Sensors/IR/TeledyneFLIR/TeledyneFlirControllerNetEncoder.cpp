

#include "TeledyneFlirControllerNetEncoder.hpp"
#include "teledyne_flir_packet_utils.hpp"

using namespace flir;

cTeledyneFlirControllerNetEncoder::cTeledyneFlirControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cTeledyneFlirControllerNetEncoder::sendCurrentState(bool valid, uint8_t active_camera_id,
    uint16_t width, uint16_t height, uint8_t fps, uint16_t interval_s,
    uint8_t min_fps, uint8_t max_fps)
{
    encode_current_state(valid, active_camera_id, width, height, fps, interval_s, min_fps, max_fps, mBuffer);
    sendData();
}

void cTeledyneFlirControllerNetEncoder::sendCameraMode(uint8_t mode)
{
    encode_camera_mode(mode, mBuffer);
    sendData();
}

void cTeledyneFlirControllerNetEncoder::sendImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cTeledyneFlirControllerNetEncoder::sendFrameRate_Hz(double fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

void cTeledyneFlirControllerNetEncoder::sendFrameInterval_ms(uint32_t interval_ms)
{
    encode_frame_interval(interval_ms, mBuffer);
    sendData();
}

