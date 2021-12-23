
#pragma once

#include "LidarModel.hpp"

#include "OusterDataStream.hpp"
#include "OusterCmdStream.hpp"
#include "OusterImuStream.hpp"

#include <ouster/ouster_defs.h>
#include <ouster/OusterSensorDiscovery.h>

#include <QObject>

const static uint32_t MIN_RANGE_MM = 300;

class cOusterModel : public cLidarModel, private cOusterImuStream_Qt, private cOusterLidarStream_Qt
{
    Q_OBJECT

public:
    cOusterModel();
    virtual ~cOusterModel() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

    bool configure(const nlohmann::json& jsonCfg) override;
    void writeDataHeader(cBlockDataFile& file) override;

    uint16_t columnsPerFrame() const;
 //   std::vector<int> pixel_shift_by_row;
    uint16_t pixelsPerColumn() const;
    uint16_t columnWindowMin() const;
    uint16_t columnWindowMax() const;

    uint32_t minEncoderCount() const;
    uint32_t maxEncoderCount() const;

    double lidar_origin_to_beam_origin_mm() const;
    const std::vector<double>& beamAzimuthAngles_rad() const;
    const std::vector<double>& beamAltitudeAngles_rad() const;

    uint16_t              frameID() const;
    ouster::lidar_data_t  lidarData() const;
    ouster::imu_data_t    imuData() const;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

signals:
    void updateView();

protected:
    void onNewData(const ouster::imu_data_t& new_data) override;
    void onNewData(uint16_t frameID, ouster::lidar_data_t& data) override;

protected:
    void update() override;

private:
    bool mConnected;
    int mFrameCounter;

    uint16_t mImuPort;
    uint16_t mLidarPort;
    std::string mSensorIpAddress;
    std::string mDstIpAddress;
    bool mUseIpv6;

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

