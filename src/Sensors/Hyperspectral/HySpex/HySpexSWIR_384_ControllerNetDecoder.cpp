

#include "HySpexSWIR_384_ControllerNetDecoder.hpp"

#include "hyspex_packet_utils.hpp"
#include "net_buffer.hpp"


void cHySpexSWIR_384_ControllerNetDecoder::processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer)
{
    using namespace hyspex;

    switch (id)
    {
    default:
    {
        break;
    }
    }
}


