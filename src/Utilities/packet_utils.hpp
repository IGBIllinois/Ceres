/*
*/

#pragma once

#include <cstdint>

class net_buffer;
class net_buffer_view;


struct sPacketHeader_t 
{
	uint16_t  id;
	uint8_t   revision;
	uint16_t  length;

	struct sTimestamp
	{
		uint64_t seconds;
		uint32_t nanos;
	}
	timestamp;
};

void set_timestamp(sPacketHeader_t::sTimestamp* timestamp);


net_buffer& operator>>(net_buffer& buffer, sPacketHeader_t& hdr);
net_buffer_view& operator>>(net_buffer_view& buffer, sPacketHeader_t& hdr);
net_buffer& operator<<(net_buffer& buffer, const sPacketHeader_t& hdr);

