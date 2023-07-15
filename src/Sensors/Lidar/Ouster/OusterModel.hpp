
#pragma once

#include "../LidarModel.hpp"

#include <ouster/ouster_defs.h>
#include <ouster/OusterLidarData.h>

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
    const char* descriptor() const override;

    virtual bool configure(const nlohmann::json& jsonCfg) = 0;

    std::vector<std::string> getLidarModes() const;

    ouster::eLIDAR_MODE getLidarMode() const;

    ouster::azimuth_range_t getAzimuthWindow() const;

    uint16_t columnsPerFrame() const;
    uint16_t pixelsPerColumn() const;
    uint16_t columnWindowMin() const;
    uint16_t columnWindowMax() const;

    uint32_t minEncoderCount() const;
    uint32_t maxEncoderCount() const;

    ouster::sensor_info_2_t       getSensorInfo() const;
    ouster::time_info_2_t         getTimeInfo() const;
    ouster::beam_intrinsics_2_t   getBeamIntrinsics() const;
    ouster::imu_intrinsics_2_t    getImuIntrinsics() const;
    ouster::lidar_intrinsics_2_t  getLidarIntrinsics() const;
    ouster::lidar_data_format_2_t getLidarDataFormat() const;

    double lidar_origin_to_beam_origin_mm() const;
    const std::vector<double>& beamAzimuthAngles_rad() const;
    const std::vector<double>& beamAltitudeAngles_rad() const;

    uint16_t              frameID() const;
    cOusterLidarData      lidarData() const;
    ouster::imu_data_t    imuData() const;

public:
    void setAzimuthWindow(ouster::azimuth_range_t range);

signals:
    void updateSensorInfo();
    void updateTimeInfo();
    void updateLidarMode();
    void updateBeamIntrinsics();
    void updateImuIntrinsics();
    void updateLidarIntrinsics();
    void updateDataFormat();

    void updateAzimuthWindow();
    void updateImuData();
    void updateLidarData();


public slots:
    virtual void changeAzimuthWindow(double min_deg, double max_deg) = 0;
    virtual void changeLidarMode(QString mode_str) = 0;


protected:
    virtual void onNewData(const ouster::imu_data_t& new_data) = 0;
    virtual void onNewData(uint16_t frameID, const cOusterLidarData& data) = 0;

protected:
    int mImuDataCounter;
    int mLidarDataCounter;

    ouster::config_param_2_t		mConfigParameters;
    ouster::sensor_info_2_t		    mSensorInfo;
    ouster::time_info_2_t			mTimeInfo;
    ouster::beam_intrinsics_2_t	    mBeamIntrinsics;
    ouster::imu_intrinsics_2_t	    mImuIntrinsics;
    ouster::lidar_intrinsics_2_t	mLidarIntrinsics;
    ouster::lidar_data_format_2_t   mDataFormat;

//    ouster::alerts_t			mAlerts;

    ouster::imu_data_t  mLastImuData;

    uint16_t            mLastFrameID;
    cOusterLidarData    mLastLidarData;

    double mLidarOriginToBeamOrigin_mm;
    std::vector<double> mBeamAzimuthAngles_rad;
    std::vector<double> mBeamAltitudeAngles_rad;
};

