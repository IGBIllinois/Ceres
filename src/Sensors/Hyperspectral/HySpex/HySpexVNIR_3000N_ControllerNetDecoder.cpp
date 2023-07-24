

#include "HySpexVNIR_3000N_ControllerNetDecoder.hpp"

#include "hyspex_packet_utils.hpp"
#include "net_buffer.hpp"


void cHySpexVNIR_3000N_ControllerNetDecoder::processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer)
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


