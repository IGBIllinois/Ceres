
#pragma once

#include "LidarModel.hpp"

#include "OusterDataStream.hpp"
#include "OusterCmdStream.hpp"
#include "OusterImuStream.hpp"
#include "OusterSerializer.hpp"

#include <ouster/ouster_defs.h>
#include <ouster/OusterSensorDiscovery.h>

#include <QObject>

const static uint32_t MIN_RANGE_MM = 300;

class cOusterModel : public cLidarModel, private cOusterImuStream_Qt, private cOusterLidarStream_Qt
{
    Q_OBJECT

public:
    cOusterModel(QObject* parent = nullptr);
    virtual ~cOusterModel() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void writeDataHeader(cBlockDataFile& file) override;
    void endDataRecording() override;

    uint16_t columnsPerFrame() const;
    uint16_t pixelsPerColumn() const;
    uint16_t columnWindowMin() const;
    uint16_t columnWindowMax() const;

    uint32_t minEncoderCount() const;
    uint32_t maxEncoderCount() const;

    ouster::sensor_info_t getSensorInfo() const;
    ouster::time_info_t getTimeInfo() const;
    ouster::beam_intrinsics_t getBeamIntrinsics() const;
    ouster::imu_intrinsics_t getImuIntrinsics() const;
    ouster::lidar_intrinsics_t getLidarIntrinsics() const;
    ouster::lidar_data_format_t getLidarDataFormat() const;

    double lidar_origin_to_beam_origin_mm() const;
    const std::vector<double>& beamAzimuthAngles_rad() const;
    const std::vector<double>& beamAltitudeAngles_rad() const;

    uint16_t              frameID() const;
    cOusterLidarData      lidarData() const;
    ouster::imu_data_t    imuData() const;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

signals:
    void updateSensorInfo();
    void updateTimeInfo();
    void updateBeamIntrinsics();
    void updateImuIntrinsics();
    void updateLidarIntrinsics();
    void updateDataFormat();

/*
    void updateSensorInfo(ouster::sensor_info_t info);
    void updateTimeInfo(ouster::time_info_t info);
    void updateBeamIntrinsics(ouster::beam_intrinsics_t beam_intrinsics);
    void updateImuIntrinsics(ouster::imu_intrinsics_t imu_intrinsics);
    void updateLidarIntrinsics(ouster::lidar_intrinsics_t lidar_intrinsics);
    void updateDataFormat(ouster::lidar_data_format_t lidar_data_format);
    void updateImuData(ouster::imu_data_t data);
    void updateAzimuthWindow(ouster::azimuth_range_t azimuth_range);
*/

    void updateAzimuthWindow();
    void updateEncoderCount(int min, int max);
    void updateImuData();
    void updateLidarData();

protected:
    void onNewData(const ouster::imu_data_t& new_data) override;
    void onNewData(uint16_t frameID, const cOusterLidarData& data) override;

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

    ouster::imu_data_t  mLastImuData;

    uint16_t            mLastFrameID;
    cOusterLidarData    mLastLidarData;

    double mLidarOriginToBeamOrigin_mm;
    std::vector<double> mBeamAzimuthAngles_rad;
    std::vector<double> mBeamAltitudeAngles_rad;

    cOusterSerializer mSerializer;
};

