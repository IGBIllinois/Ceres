
#include "OusterModel.hpp"
#include "OusterFactory.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cOusterModel::cOusterModel(QObject* parent)
:
    cLidarModel(parent),
    mLastFrameID(0),
    mLidarOriginToBeamOrigin_mm(0)
{
    mFrameCounter = 0;
}

char* cOusterModel::descriptor() const
{
    return ouster_id;
}

ouster::sensor_info_2_t cOusterModel::getSensorInfo() const
{
    return mSensorInfo;
}

ouster::time_info_2_t cOusterModel::getTimeInfo() const
{
    return mTimeInfo;
}

ouster::beam_intrinsics_2_t cOusterModel::getBeamIntrinsics() const
{
    return mBeamIntrinsics;
}

ouster::imu_intrinsics_2_t cOusterModel::getImuIntrinsics() const
{
    return mImuIntrinsics;
}

ouster::lidar_intrinsics_2_t cOusterModel::getLidarIntrinsics() const
{
    return mLidarIntrinsics;
}

ouster::lidar_data_format_2_t cOusterModel::getLidarDataFormat() const
{
    return mDataFormat;
}

uint16_t cOusterModel::columnsPerFrame() const
{
    return mDataFormat.columns_per_frame;
}

uint16_t cOusterModel::pixelsPerColumn() const
{
    return mDataFormat.pixels_per_column;
}

uint16_t cOusterModel::columnWindowMin() const
{
    return mDataFormat.column_window_min;
}

uint16_t cOusterModel::columnWindowMax() const
{
    return mDataFormat.column_window_max;
}

uint32_t cOusterModel::minEncoderCount() const
{
    return mAzimuthWindow.min_deg * ouster::DEG_TO_ENCODER_TICS;
    return 22528;
}

uint32_t cOusterModel::maxEncoderCount() const
{
    return mAzimuthWindow.max_deg * ouster::DEG_TO_ENCODER_TICS;
    return 67584;
}


double cOusterModel::lidar_origin_to_beam_origin_mm() const
{
    return mLidarOriginToBeamOrigin_mm;
}

const std::vector<double>& cOusterModel::beamAzimuthAngles_rad() const
{
    return mBeamAzimuthAngles_rad;
}

const std::vector<double>& cOusterModel::beamAltitudeAngles_rad() const
{
    return mBeamAltitudeAngles_rad;
}

uint16_t cOusterModel::frameID() const
{
    return mLastFrameID;
}

cOusterLidarData cOusterModel::lidarData() const
{
    return mLastLidarData;
}

ouster::imu_data_t cOusterModel::imuData() const
{
    return mLastImuData;
}
