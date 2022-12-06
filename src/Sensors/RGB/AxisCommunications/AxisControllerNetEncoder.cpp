

#include "AxisControllerNetEncoder.hpp"
#include "axis_communications_packet_utils.hpp"


cAxisControllerNetEncoder::cAxisControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cAxisControllerNetEncoder::sendCurrentState(bool valid, uint8_t camera_id,
    uint16_t width, uint16_t height, uint8_t fps)
{
    encode_current_state(valid, camera_id, width, height, fps, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendActiveCameraId(uint8_t camera_id)
{
    encode_active_camera_id(camera_id, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendImageSize(uint16_t width, uint16_t height)
{
    encode_image_size(width, height, mBuffer);
    sendData();
}

void cAxisControllerNetEncoder::sendFrameRate(uint8_t fps)
{
    encode_frame_rate(fps, mBuffer);
    sendData();
}

/*
void cOusterControllerNetEncoder::sendCurrentState(bool valid,
    ouster::eLIDAR_MODE mode, double min_deg, double max_deg)
{
    std::string lidar_mode(to_string(mode));
    encode_current_state(valid, lidar_mode, min_deg, max_deg, mBuffer);
    sendData();
}
*/
