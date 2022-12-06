

#include "HySpexSWIR_384_PropertiesNetDecoder.hpp"
#include "hyspex_swir_384_packet_utils.hpp"
#include "net_buffer.hpp"

using namespace hyspex::swir384;


void cHySpexSWIR_384_PropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        CurrentState_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onCurrentState(packet.valid());
        break;
    }
    }
}
