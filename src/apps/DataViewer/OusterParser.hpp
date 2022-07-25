/**
 * @file
 * @brief Parser for reading OUSTER LiDAR data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "OusterBlockId.hpp"
#include "OusterTypes.hpp"


// Forward Declarations
class cOusterLidarData;


class cOusterParser : public cBlockParser
{
public:
	cOusterParser();
	~cOusterParser() = default;

	cBlockID& blockID() override;

	virtual void onConfigParam_2(const ouster::config_param_2_t& data) = 0;
	virtual void onSensorInfo_2(const ouster::sensor_info_2_t& data) = 0;
	virtual void onTimestamp_2(const ouster::timestamp_2_t& data) = 0;
	virtual void onSyncPulseIn_2(const ouster::sync_pulse_in_2_t& data) = 0;
	virtual void onSyncPulseOut_2(const ouster::sync_pulse_out_2_t& data) = 0;
	virtual void onMultipurposeIo_2(const ouster::multipurpose_io_2_t& data) = 0;
	virtual void onNmea_2(const ouster::nmea_2_t& data) = 0;
	virtual void onTimeInfo_2(const ouster::time_info_2_t& data) = 0;
	virtual void onBeamIntrinsics_2(const ouster::beam_intrinsics_2_t& data) = 0;
	virtual void onImuIntrinsics_2(const ouster::imu_intrinsics_2_t& data) = 0;
	virtual void onLidarIntrinsics_2(const ouster::lidar_intrinsics_2_t& data) = 0;
	virtual void onLidarDataFormat_2(const ouster::lidar_data_format_2_t& data) = 0;
	virtual void onLidarDataFormat_2(const ouster::lidar_data_format_2_3_t& data) = 0;
	virtual void onImuData(const ouster::imu_data_t& data) = 0;
//	const cOusterLidarData&			getLidarData() const { return mLidarData; }


protected:
	void processData(BLOCK_MAJOR_VERSION_t major_version,
		             BLOCK_MINOR_VERSION_t minor_version, 
		             BLOCK_DATA_ID_t data_id, 
		             cDataBuffer& buffer) override;

	void processConfigParam_2(cDataBuffer& buffer);
	void processSensorInfo_2(cDataBuffer& buffer);
	void processTimestamp_2(cDataBuffer& buffer);
	void processSyncPulseIn_2(cDataBuffer& buffer);
	void processSyncPulseOut_2(cDataBuffer& buffer);
	void processMultipurposeIO_2(cDataBuffer& buffer);
	void processNmea_2(cDataBuffer& buffer);
	void processTimeInfo_2(cDataBuffer& buffer);
	void processBeamIntrinsics_2(cDataBuffer& buffer);
	void processImuIntrinsics_2(cDataBuffer& buffer);
	void processLidarIntrinsics_2(cDataBuffer& buffer);
	void processLidarDataFormat_2(cDataBuffer& buffer);
	void processLidarDataFormat_2_3(cDataBuffer& buffer);
	void processImuData(cDataBuffer& buffer);
	void processLidarData(cDataBuffer& buffer);
	void processLidarDataFrameTimestamp(cDataBuffer& buffer);

private:
	cOusterLidarID    mBlockID;
};

