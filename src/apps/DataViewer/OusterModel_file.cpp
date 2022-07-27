
#include "OusterModel_file.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cOusterModel_file::cOusterModel_file(QObject* parent)
:
    cOusterModel(parent)
{
}

void cOusterModel_file::onConfigParam_2(const ouster::config_param_2_t& data)
{
    mConfigParameters = data;
}

void cOusterModel_file::onSensorInfo_2(const ouster::sensor_info_2_t& data)
{
    mSensorInfo = data;
    emit updateSensorInfo();

}

void cOusterModel_file::onTimestamp_2(const ouster::timestamp_2_t& data)
{}

void cOusterModel_file::onSyncPulseIn_2(const ouster::sync_pulse_in_2_t& data)
{}

void cOusterModel_file::onSyncPulseOut_2(const ouster::sync_pulse_out_2_t& data)
{}

void cOusterModel_file::onMultipurposeIo_2(const ouster::multipurpose_io_2_t& data)
{}

void cOusterModel_file::onNmea_2(const ouster::nmea_2_t& data)
{}

void cOusterModel_file::onTimeInfo_2(const ouster::time_info_2_t& data)
{
    mTimeInfo = data;
    emit updateTimeInfo();

}

void cOusterModel_file::onBeamIntrinsics_2(const ouster::beam_intrinsics_2_t& data)
{
    mBeamIntrinsics = data;
    emit updateBeamIntrinsics();

}

void cOusterModel_file::onImuIntrinsics_2(const ouster::imu_intrinsics_2_t& data)
{
    mImuIntrinsics = data;
    emit updateImuIntrinsics();

}

void cOusterModel_file::onLidarIntrinsics_2(const ouster::lidar_intrinsics_2_t& data)
{
    mLidarIntrinsics = data;
    emit updateLidarIntrinsics();
}

void cOusterModel_file::onLidarDataFormat_2(const ouster::lidar_data_format_2_t& data)
{
    mDataFormat = data;
//    ouster::azimuth_range_t		    mAzimuthWindow;
    emit updateDataFormat();
}

void cOusterModel_file::onImuData(const ouster::imu_data_t& data)
{
    onNewData(data);
}

void cOusterModel_file::onLidarData(const cOusterLidarData& data)
{
    onNewData(data.frame_id(), data);
}

void cOusterModel_file::onNewData(const ouster::imu_data_t& data)
{
    mLastImuData = data;
    emit updateImuData();
}

void cOusterModel_file::onNewData(uint16_t frameID, const cOusterLidarData& data)
{
    mLastFrameID = frameID;
    mLastLidarData = data;

    emit updateLidarData();
}
