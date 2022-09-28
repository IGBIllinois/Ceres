

#include "OusterPropertiesNetEncoder.hpp"
#include "ouster_packet_utils.hpp"


cOusterPropertiesNetEncoder::cOusterPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cOusterPropertiesNetEncoder::sendQueryState()
{
    encode_query_state(mBuffer);
    sendData();
}

void cOusterPropertiesNetEncoder::sendSetAzimuthWindow(double min_deg, double max_deg)
{
    encode_azimuth_window(min_deg, max_deg, mBuffer);
    sendData();
}

void cOusterPropertiesNetEncoder::sendSetMode(const std::string& mode)
{
    encode_lidar_mode(mode, mBuffer);
    sendData();
}

