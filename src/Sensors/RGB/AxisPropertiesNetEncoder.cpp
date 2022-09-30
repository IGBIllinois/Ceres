

#include "AxisPropertiesNetEncoder.hpp"
#include "axis_communications_packet_utils.hpp"


cAxisPropertiesNetEncoder::cAxisPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cAxisPropertiesNetEncoder::sendQueryState()
{
    encode_query_state(mBuffer);
    sendData();
}

void cAxisPropertiesNetEncoder::sendSetAzimuthWindow(double min_deg, double max_deg)
{
//    encode_azimuth_window(min_deg, max_deg, mBuffer);
//    sendData();
}

void cAxisPropertiesNetEncoder::sendSetMode(const std::string& mode)
{
//    encode_lidar_mode(mode, mBuffer);
//    sendData();
}

