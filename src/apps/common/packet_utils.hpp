/*
   GNSS Receiver Communication SDK for C++ and Asio
   For connecting to and communicating with a Septentrio GNSS receiver, using C++/Asio
*/

#pragma once

#include <cstdint>

namespace ceres
{
	class net_buffer;
	class net_buffer_view;
}


enum class ePacketType : uint16_t
{
	UNKNOWN = 0,

	COMMAND_PCK_1	= 1,

	SPIDER_CAM_DATA_1 = 100,

	WEATHER_DATA_1 = 200,
};

struct sPacketHeader_t 
{
   ePacketType id;
   uint8_t revision;
   uint16_t length;

   struct sTimestamp
   {
	   uint64_t seconds;
	   uint32_t nanos;
   }
   timestamp;
};


ceres::net_buffer& operator>>(ceres::net_buffer& buffer, sPacketHeader_t& hdr);
ceres::net_buffer_view& operator>>(ceres::net_buffer_view& buffer, sPacketHeader_t& hdr);
ceres::net_buffer& operator<<(ceres::net_buffer& buffer, const sPacketHeader_t& hdr);

