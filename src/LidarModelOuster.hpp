
#pragma once

#include "LidarModel.hpp"

#include "LidarStreamOuster.hpp"
#include "LidarStreamOusterCmd.hpp"
#include "LidarStreamOusterImu.hpp"
#include "ouster/ouster_defs.h"
#include "ouster/OusterSensorDiscovery.h"

#include <QObject>

class cLidarModelOuster : public cLidarModel, private cOusterImuStream_Qt, private cOusterLidarStream_Qt
{
    Q_OBJECT

public:
    cLidarModelOuster(QObject* parent = nullptr);
    virtual ~cLidarModelOuster() = default;

    /*
     * Returns the preferred window title for the corresponding view.
     */
    QString getViewTitle() const override;

    void configure(nlohmann::json& jsonCfg) override;
    void writeDataHeader(cDataFile& file) override;

    double lidar_origin_to_beam_origin_mm() const;
    const std::vector<double>& beam_azimuth_angles_rad() const;
    const std::vector<double>& beam_altitude_angles_rad() const;

protected:
    void onNewData(const ouster::imu_data_t& new_data) override;
    void onNewData(uint16_t frameID, ouster::lidar_data_t& data) override;

protected:
    void run() override;

private:
    bool mConnected;

    cOusterCmdStream_Qt   mCmdStream;

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

    double mLidarOriginToBeamOrigin_mm;
    std::vector<double> mBeamAzimuthAngles_rad;
    std::vector<double> mBeamAltitudeAngles_rad;
};

