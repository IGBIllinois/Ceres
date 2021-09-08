
#pragma once

#include "LidarModel.hpp"

#include "LidarStreamOuster.hpp"
#include "LidarStreamOusterCmd.hpp"
#include "LidarStreamOusterImu.hpp"
#include "ouster/ouster_defs.h"
#include "ouster/OusterSensorDiscovery.h"

#include <QObject>

class cLidarModelOuster : public cLidarModel
{
    Q_OBJECT

public:
    cLidarModelOuster(QObject* parent = nullptr);
    virtual ~cLidarModelOuster() = default;

    void configure(nlohmann::json& jsonCfg) override;
    void writeDataHeader(cDataFile& file) override;

private slots:
    void processImuData(ouster::imu_data_t data);
    void processLidarData(uint16_t frameID, ouster::lidar_data_t data);

protected:

    cOusterCmdStream_Qt   mCmdStream;
    cOusterLidarStream_Qt mDataStream;
    cOusterImuStream_Qt   mImuStream;

    ouster::sensor_network_info_t mActiveSensor;

    ouster::config_param_t		mConfigParameters;
    ouster::sensor_info_t		mSensorInfo;
    ouster::time_info_t			mTimeInfo;
    ouster::beam_intrinsics_t	mBeamIntrinsics;
    ouster::imu_intrinsics_t	mImuIntrinsics;
    ouster::lidar_intrinsics_t	mLidarIntrinsics;
    ouster::lidar_data_format_t mDataFormat;
	ouster::azimuth_range_t		mAzimuthWindow;

//    ouster::alerts_t			mAlerts;

    ouster::imu_data_t    mLastImuData;

    uint16_t              mLastFrameID;
    ouster::lidar_data_t  mLastLidarData;
};

