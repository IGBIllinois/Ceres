

#include "ceres_remote_client_net_encoder.hpp"
#include "packet_utils.hpp"

using namespace ceres;


cCeresRemoteClientNetEncoder::cCeresRemoteClientNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

/*
void cCeresRemoteClientNetEncoder::sendGeodetic(const gps::PVT_Geodetic_2_t& pvt)
{
    encodeGeodetic(pvt);
    sendOutgoingData(static_cast<const char*>(mBuffer.data()), mBuffer.size());
    clear();
}
*/

