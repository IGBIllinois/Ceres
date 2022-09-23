/*
 * The NetworkDecoder is used to decode network based packets 
 * You will need to override various virtual methods!
 */

#pragma once

#include "packet_utils.hpp"

#include <cstddef>


class cNetworkDecoder
{
public:

    cNetworkDecoder() = default;
    virtual ~cNetworkDecoder() = default;

    /**
     * @brief The decode method convert the byte stream from a TCP receiver port.
     * The decode the various packets and call the various pure virtual 
     * methods.
     */
    void decode(const void* pBuffer, std::size_t buf_length);

protected:
    virtual void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) = 0;
};


