

#include "HySpexSWIR_384_ControllerNetDecoder.hpp"

#include "hyspex_swir_384_packet_utils.hpp"
#include "net_buffer.hpp"

using namespace hyspex::swir384;


void cHySpexSWIR_384_ControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::QUERY_STATE:
    {
        onQueryState();
        break;
    }
    }
}




