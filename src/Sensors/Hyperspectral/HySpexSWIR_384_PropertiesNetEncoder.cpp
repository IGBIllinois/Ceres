

#include "HySpexSWIR_384_PropertiesNetEncoder.hpp"
#include "hyspex_swir_384_packet_utils.hpp"

using namespace hyspex::swir384;


cHySpexSWIR_384_PropertiesNetEncoder::cHySpexSWIR_384_PropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cHySpexSWIR_384_PropertiesNetEncoder::sendQueryState()
{
    encode_query_state(mBuffer);
    sendData();
}



