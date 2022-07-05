
#pragma once

#include "LidarModel.hpp"

#include "OusterDataStream.hpp"
#include "OusterCmdStream.hpp"
#include "OusterImuStream.hpp"
#include "../../BlockDataFile/OusterSerializer.hpp"

#include <ouster/ouster_defs.h>
#include <ouster/OusterSensorDiscovery.h>

#include <QObject>

const static uint32_t MIN_RANGE_MM = 300;

class cOusterModel : public cLidarModel
{
    Q_OBJECT

public:
    cOusterModel(QObject* parent = nullptr);
    virtual ~cOusterModel() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

    virtual bool configure(const nlohmann::json& jsonCfg) = 0;

    uint16_t columnsPerFrame() const;
    uint16_t pixelsPerColumn() const;
    uint16_t columnWindowMin() const;
    uint16_t columnWindowMax() const;

    uint32_t minEncoderCount() const;
    uint32_t maxEncoderCount() const;

    ouster::sensor_info_2_t getSensorInfo() const;
    ouster::time_info_2_t getTimeInfo() const;
    ouster::beam_intrinsics_2_t getBeamIntrinsics() const;
    ouster::imu_intrinsics_2_t getImuIntrinsics() const;
    ouster::lidar_intrinsics_2_t getLidarIntrinsics() const;
    ouster::lidar_data_format_2_t getLidarDataFormat() const;

    double lidar_origin_to_beam_origin_mm() const;
    const std::vector<double>& beamAzimuthAngles_rad() const;
    const std::vector<double>& beamAltitudeAngles_rad() const;

    uint16_t              frameID() const;
    cOusterLidarData      lidarData() const;
    ouster::imu_data_t    imuData() const;

signals:
    void updateSensorInfo();
    void updateTimeInfo();
    void updateBeamIntrinsics();
    void updateImuIntrinsics();
    void updateLidarIntrinsics();
    void updateDataFormat();

    void updateAzimuthWindow();
    void updateEncoderCount(int min, int max);
    void updateImuData();
    void updateLidarData();

protected:
    virtual void onNewData(const ouster::imu_data_t& new_data) = 0;
    virtual void onNewData(uint16_t frameID, const cOusterLidarData& data) = 0;

protected:
    int mFrameCounter;

    ouster::config_param_2_t		mConfigParameters;
    ouster::sensor_info_2_t		    mSensorInfo;
    ouster::time_info_2_t			mTimeInfo;
    ouster::beam_intrinsics_2_t	    mBeamIntrinsics;
    ouster::imu_intrinsics_2_t	    mImuIntrinsics;
    ouster::lidar_intrinsics_2_t	mLidarIntrinsics;
    ouster::lidar_data_format_2_t   mDataFormat;
	ouster::azimuth_range_t		    mAzimuthWindow;

//    ouster::alerts_t			mAlerts;

    ouster::imu_data_t  mLastImuData;

    uint16_t            mLastFrameID;
    cOusterLidarData    mLastLidarData;

    double mLidarOriginToBeamOrigin_mm;
    std::vector<double> mBeamAzimuthAngles_rad;
    std::vector<double> mBeamAltitudeAngles_rad;
};

