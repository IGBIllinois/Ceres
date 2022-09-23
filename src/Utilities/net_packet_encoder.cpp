

#include "net_packet_encoder.hpp"


cNetworkEncoder::cNetworkEncoder(std::size_t capacity)
    : mBuffer(capacity)
{}

int cNetworkEncoder::sendData()
{
    auto n = sendOutgoingData(static_cast<const char*>(mBuffer.data()), mBuffer.size());
    clear();

    return n;
}

