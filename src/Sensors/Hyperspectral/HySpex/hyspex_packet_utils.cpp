
#include "hyspex_vnir_3000N_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>

using namespace hyspex::vnir3000N;


int hyspex::vnir3000N::encode_query_state(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

sCurrentState_t hyspex::vnir3000N::to_current_state_1(const CurrentState_1& pckt)
{
    sCurrentState_t data;

    data.valid = pckt.valid();

    if (data.valid)
    {
    }
    else
    {
    }

    return data;
}

int hyspex::vnir3000N::encode_current_state(bool valid, net_buffer& buffer)
{
    CurrentState_1 pckt;

    pckt.set_valid(valid);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CURRENT_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}



