
#include "hyspex_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>


int hyspex::encode_query_state(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

hyspex::sCurrentState_t hyspex::to_current_state_1(const hyspex_CurrentState_1& pckt)
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

int hyspex::encode_current_state(const sCurrentState_t& state, net_buffer& buffer)
{
    hyspex_CurrentState_1 pckt;

    pckt.set_valid(state.valid);
    pckt.set_valid(state.valid);

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



