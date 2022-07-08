
#include "packet_utils.hpp"
#include "net_buffer.hpp"

using namespace ceres;

net_buffer& operator>>(net_buffer& buffer, sPacketHeader_t& hdr)
{
    uint16_t id;
    buffer >> id >> hdr.revision >> hdr.length >> hdr.timestamp.seconds >> hdr.timestamp.nanos;
    hdr.id = static_cast<ePacketType>(id);
    return buffer;
}

net_buffer& operator<<(net_buffer& buffer, const sPacketHeader_t& hdr)
{
    buffer << static_cast<uint16_t>(hdr.id) << hdr.revision << hdr.length << hdr.timestamp.seconds << hdr.timestamp.nanos;
    return buffer;
}

net_buffer_view& operator>>(net_buffer_view& buffer, sPacketHeader_t& hdr)
{
    uint16_t id;
    buffer >> id >> hdr.revision >> hdr.length >> hdr.timestamp.seconds >> hdr.timestamp.nanos;
    hdr.id = static_cast<ePacketType>(id);
    return buffer;
}
