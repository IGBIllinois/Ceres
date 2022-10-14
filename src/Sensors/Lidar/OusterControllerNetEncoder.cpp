

#include "OusterControllerNetEncoder.hpp"
#include "ouster_packet_utils.hpp"
#include <ouster/ouster_utils.h>


cOusterControllerNetEncoder::cOusterControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cOusterControllerNetEncoder::sendLidarMode(ouster::eLIDAR_MODE mode)
{
    std::string lidar_mode(to_string(mode));
    encode_lidar_mode(lidar_mode, mBuffer);
    sendData();
}

void cOusterControllerNetEncoder::sendAzimuthWindow(double min_deg, double max_deg)
{
    encode_azimuth_window(min_deg, max_deg, mBuffer);
    sendData();
}

void cOusterControllerNetEncoder::sendCurrentState(bool valid,
    ouster::eLIDAR_MODE mode, double min_deg, double max_deg)
{
    std::string lidar_mode(to_string(mode));
    encode_current_state(valid, lidar_mode, min_deg, max_deg, mBuffer);
    sendData();
}

