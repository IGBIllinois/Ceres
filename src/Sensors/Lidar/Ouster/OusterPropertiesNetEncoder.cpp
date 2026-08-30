

#include "OusterPropertiesNetEncoder.hpp"
#include "ouster_packet_utils.hpp"


cOusterPropertiesNetEncoder::cOusterPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cOusterPropertiesNetEncoder::sendQueryStateMessage()
{
    encode_query_state(mBuffer);
    sendData();
}

void cOusterPropertiesNetEncoder::sendQueryLidarModeMessage()
{
    encode_query_lidar_mode(mBuffer);
    sendData();
}

void cOusterPropertiesNetEncoder::sendQueryAzimuthWindowMessage()
{
    encode_query_azimuth_window(mBuffer);
    sendData();
}

void cOusterPropertiesNetEncoder::sendSetAzimuthWindowMessage(double min_deg, double max_deg)
{
    encode_azimuth_window(min_deg, max_deg, mBuffer);
    sendData();
}

void cOusterPropertiesNetEncoder::sendSetLidarModeMessage(const std::string& mode)
{
    encode_lidar_mode(mode, mBuffer);
    sendData();
}

