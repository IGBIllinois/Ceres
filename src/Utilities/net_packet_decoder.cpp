

#include "net_packet_decoder.hpp"
#include "net_buffer.hpp"
#include "packet_utils.hpp"

void cNetworkDecoder::decode(const void* pBuffer, std::size_t buf_length)
{
    if (!pBuffer)
        return;

    net_buffer_view buffer(reinterpret_cast<const std::byte*>(pBuffer), buf_length);

    sPacketHeader_t hdr;

    while (buffer.size() > 0)
    {
        buffer >> hdr;
     
        if (buf_length < hdr.length)
        { 
            break;
        }

        processPacket(hdr, buffer);

        buffer.advance(hdr.length);
    }
}


