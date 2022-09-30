

#include "AxisControllerNetEncoder.hpp"
#include "axis_communications_packet_utils.hpp"


cAxisControllerNetEncoder::cAxisControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

/*
void cOusterControllerNetEncoder::sendCurrentState(bool valid,
    ouster::eLIDAR_MODE mode, double min_deg, double max_deg)
{
    std::string lidar_mode(to_string(mode));
    encode_current_state(valid, lidar_mode, min_deg, max_deg, mBuffer);
    sendData();
}
*/
