
#pragma once

#include <cstdint>
#include <utility>

/**
	Storage of the data from the Ceres system is based on a block file format.  

	In this file format, each block contains a block type identifier.  The block type is used
	as a signal on how the data in the block should be decoded.

	The block type is four-byte unsigned integer.  The following regions 

    0         - 65,535         Range for Main program / Experiment Header data
    65,536    - 1,048,575      Range for Experiment Controller data
    1,048,576 - 4,294,967,295  Range for Sensor data
*/

struct BlockID_t
{
public:
    explicit BlockID_t(const uint32_t& id) : ID(id)
    {
    }

    explicit BlockID_t(uint32_t&& id) noexcept
        : ID(std::move(id))
    {
    }

    explicit operator uint32_t& () noexcept
    {
        return ID;
    }

    explicit operator const uint32_t& () const noexcept
    {
        return ID;
    }

    friend void swap(BlockID_t& a, BlockID_t& b) noexcept
    {
        using std::swap;
        swap(static_cast<uint32_t&>(a), static_cast<uint32_t&>(b));
    }

private:
    uint32_t ID;
};

static uint32_t SYSTEM_BASE_ID = 0;
static uint32_t SYSTEM_MAX_ID  = 0x0000FFFF;

static uint32_t EXP_CONTROL_BASE_ID = 0x00010000;
static uint32_t EXP_CONTROL_MAX_ID  = 0x000FFFFF;

static uint32_t SENSOR_BASE_ID = 0x00100000;
static uint32_t SENSOR_MAX_ID  = 0xFFFFFFFF;

/*
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
*/

