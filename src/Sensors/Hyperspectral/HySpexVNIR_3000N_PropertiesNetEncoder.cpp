

#include "HySpexVNIR_3000N_PropertiesNetEncoder.hpp"
#include "hyspex_vnir_3000N_packet_utils.hpp"

using namespace hyspex::vnir3000N;

cHySpexVNIR_3000N_PropertiesNetEncoder::cHySpexVNIR_3000N_PropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState()
{
    encode_query_state(mBuffer);
    sendData();
}

