

#include "TeledyneFlirControllerNetEncoder.hpp"
#include "teledyne_flir_packet_utils.hpp"

using namespace flir;

cTeledyneFlirControllerNetEncoder::cTeledyneFlirControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cTeledyneFlirControllerNetEncoder::sendCurrentState(bool valid, uint8_t mode,
    uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
    std::optional<double> min_fps, std::optional<double> max_fps,
    std::optional<float> min_K, std::optional<float> max_K)
{
    encode_current_state(valid, mode, width, height, fps, interval_ms,
        min_fps, max_fps, min_K, max_K, mBuffer);

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

void cTeledyneFlirControllerNetEncoder::sendLapseInterval_ms(uint32_t interval_ms)
{
    encode_lapse_interval(interval_ms, mBuffer);
    sendData();
}

void cTeledyneFlirControllerNetEncoder::sendThermalRange_K(float min_value_K, float max_value_K)
{
    encode_thermal_range(min_value_K, max_value_K, mBuffer);
    sendData();
}

void cTeledyneFlirControllerNetEncoder::sendTakePhotoReply()
{
    encode_take_photo_reply(flir::eReply::GOOD, mBuffer);
    sendData();
}


