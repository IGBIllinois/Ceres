
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>

#if defined(_WIN32)

#include <WinSock2.h>
#include <chrono>

int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
    namespace sc = std::chrono;
    sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
    sc::seconds s = sc::duration_cast<sc::seconds>(d);
    tp->tv_sec = s.count();
    tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();

    return 0;
}

#else
    #include <sys/time.h>
#endif // _WIN32


void set_timestamp(sPacketHeader_t::sTimestamp* timestamp)
{
    struct timeval tv;

    gettimeofday(&tv, nullptr);

    timestamp->seconds = tv.tv_sec;
    timestamp->nanos = (tv.tv_usec * 1000);
}



net_buffer& operator>>(net_buffer& buffer, sPacketHeader_t& hdr)
{
    buffer >> hdr.id >> hdr.revision >> hdr.length >> hdr.timestamp.seconds >> hdr.timestamp.nanos;
    return buffer;
}

net_buffer& operator<<(net_buffer& buffer, const sPacketHeader_t& hdr)
{
    buffer << static_cast<uint16_t>(hdr.id) << hdr.revision << hdr.length << hdr.timestamp.seconds << hdr.timestamp.nanos;
    return buffer;
}

net_buffer_view& operator>>(net_buffer_view& buffer, sPacketHeader_t& hdr)
{
    buffer >> hdr.id >> hdr.revision >> hdr.length >> hdr.timestamp.seconds >> hdr.timestamp.nanos;
    return buffer;
}
