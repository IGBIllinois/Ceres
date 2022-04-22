/**
 * @file
 * @brief Serializer for storing OUSTER LiDAR data into a block based data file
 */
#pragma once

#include "../../BlockDataFile/BlockSerializer.hpp"
#include "OusterBlockId.hpp"

// Forward Declarations
namespace ouster
{
	struct config_param_t;
	struct sensor_info_t;
	struct timestamp_t;
	struct sync_pulse_in_t;
	struct sync_pulse_out_t;
	struct multipurpose_io_t;
	struct nmea_t;
	struct time_info_t;
	struct beam_intrinsics_t;
	struct imu_intrinsics_t;
	struct lidar_intrinsics_t;
	struct lidar_data_format_t;
	struct imu_data_t;
}
class cOusterLidarData;


class cOusterSerializer : public cBlockSerializer
{
public:
	cOusterSerializer();
	explicit cOusterSerializer(std::size_t n, cBlockDataFile* pDataFile);
	~cOusterSerializer() = default;

	void write(const ouster::config_param_t& in);
	void write(const ouster::sensor_info_t& in);
	void write(const ouster::timestamp_t& in);
	void write(const ouster::sync_pulse_in_t& in);
	void write(const ouster::sync_pulse_out_t& in);
	void write(const ouster::multipurpose_io_t& in);
	void write(const ouster::nmea_t& in);
	void write(const ouster::time_info_t& in);
	void write(const ouster::beam_intrinsics_t& in);
	void write(const ouster::imu_intrinsics_t& in);
	void write(const ouster::lidar_intrinsics_t& in);
	void write(const ouster::lidar_data_format_t& in);
	void write(const ouster::imu_data_t& in);
	void write(uint16_t frameID, const cOusterLidarData& data);

protected:
	cBlockID& blockID() override;

private:
	cOusterLidarID    mBlockID;
};

