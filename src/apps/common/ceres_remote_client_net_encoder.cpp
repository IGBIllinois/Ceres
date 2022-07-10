

#include "ceres_remote_client_net_encoder.hpp"
#include "packet_utils.hpp"

using namespace ceres;


cCeresRemoteClientNetEncoder::cCeresRemoteClientNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cCeresRemoteClientNetEncoder::sendDataFileState(bool is_open)
{
    encode_file_open_state(is_open, mBuffer);
    sendData();
}

