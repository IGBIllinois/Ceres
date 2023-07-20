

#include "HySpexVNIR_3000N_PropertiesNetEncoder.hpp"
#include "hyspex_packet_utils.hpp"


cHySpexVNIR_3000N_PropertiesNetEncoder::cHySpexVNIR_3000N_PropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState()
{
    using namespace hyspex;

    encode_query_state(mBuffer);
    sendData();
}

