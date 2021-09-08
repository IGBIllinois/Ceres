
#pragma once

#include <cstdint>

/**
	Storage of the data from the Spidercam system is based on a block file format.  

	In this file format, each block contains a block type identifier.  The block type is used
	as a signal on how the data in the block should be decoded.

	The block type is four-byte unsigned integer.  See the enumerations below.
*/

enum class BlockIDs: uint32_t
{
	HEADER_1	= 0,

	PVT_1		= 100,

	GPS_PVT_1	= 200,

	GPS_UTC_1	= 300,

	OUSTER_INFO_1 = 1000,

	OUSTER_CONFIG_1 = 1100,

	OUSTER_BEAM_INTRINSICS_1 = 1200,

	OUSTER_IMU_INTRINSICS_1 = 1300,

	OUSTER_LIDAR_INTRINSICS_1 = 1400,

	OUSTER_LIDAR_DATA_FORMAT_1 = 1500,

	OUSTER_IMU_DATA_1 = 1600,

	OUSTER_LIDAR_DATA_1 = 1700,

};

