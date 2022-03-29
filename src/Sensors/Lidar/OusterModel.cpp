
#include "OusterModel.hpp"
#include "OusterFactory.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cOusterModel::cOusterModel()
:
    mCmdStream(),
    mLastFrameID(0),
    mLidarOriginToBeamOrigin_mm(0)
{
    mConnected = false;
    mFrameCounter = 0;
    mImuPort = 0;
    mLidarPort = 0;
    mUseIpv6 = false;
}

char* cOusterModel::descriptor() const
{
    return ouster_id;
}

ouster::sensor_info_t cOusterModel::getSensorInfo() const
{
    return mSensorInfo;
}

ouster::time_info_t cOusterModel::getTimeInfo() const
{
    return mTimeInfo;
}

ouster::beam_intrinsics_t cOusterModel::getBeamIntrinsics() const
{
    return mBeamIntrinsics;
}

ouster::imu_intrinsics_t cOusterModel::getImuIntrinsics() const
{
    return mImuIntrinsics;
}

ouster::lidar_intrinsics_t cOusterModel::getLidarIntrinsics() const
{
    return mLidarIntrinsics;
}

ouster::lidar_data_format_t cOusterModel::getLidarDataFormat() const
{
    return mDataFormat;
}

bool cOusterModel::configure(const nlohmann::json& jsonCfg)
{
    std::optional<double> azimuth_min_deg;
    std::optional<double> azimuth_max_deg;
    std::optional<ouster::eLIDAR_MODE> mode;

    try
    {
        cLidarModel::configure(jsonCfg);

        if (jsonCfg.contains("azimuth window"))
        {
            azimuth_min_deg = jsonCfg["azimuth window"][0];
            azimuth_max_deg = jsonCfg["azimuth window"][1];

            if ((azimuth_min_deg < 0.0) || (azimuth_min_deg > 360.0) ||
                (azimuth_max_deg < 0.0) || (azimuth_max_deg > 360.0))
            {
                QString str = "Error in the \"ouster\" configuration:\n";
                str.append("    The \"azimuth window\" min/max values must be in the range 0.0 to 360.0 degrees.\n");
                str.append("\nThe \"azimuth window\" parameters will be ignored.");
                emit warningMessage("Configuration Warning", str);
                azimuth_min_deg.reset();
                azimuth_max_deg.reset();
            }
            else if (azimuth_max_deg <= azimuth_min_deg)
            {
                QString str = "Error in the \"ouster\" configuration:\n";
                str.append("    The minimum angle for the \"azimuth window\" must be less than the maximum angle.\n");
                str.append("\nThe \"azimuth window\" parameters will be ignored.");
                emit warningMessage("Configuration Warning", str);
                azimuth_min_deg.reset();
                azimuth_max_deg.reset();
            }
        }

        if (jsonCfg.contains("mode"))
        {
            std::string s = jsonCfg["mode"];

            if ((s == "512x10") || (s == "512X10"))
            {
                mode = ouster::eLIDAR_MODE::MODE_512x10;
            }
            else if ((s == "1024x10") || (s == "1024X10"))
            {
                mode = ouster::eLIDAR_MODE::MODE_1024x10;
            }
            else if ((s == "2048x10") || (s == "2048X10"))
            {
                mode = ouster::eLIDAR_MODE::MODE_2048x10;
            }
            else if ((s == "512x20") || (s == "512X20"))
            {
                mode = ouster::eLIDAR_MODE::MODE_512x20;
            }
            else if ((s == "1024x20") || (s == "1024X20"))
            {
                mode = ouster::eLIDAR_MODE::MODE_1024x20;
            }
            else
            {
                QString str = "Error in the \"ouster\" configuration:\n";
                str.append("    Unknown mode: ");
                str.append(s.c_str());
                str.append("\n\n");
                str.append("    Valid mode are: \n");
                str.append("        512x10, 1024x10, 2048x10\n");
                str.append("        512x20, 1024x20\n");
                str.append("\nThe \"mode\" parameter will be ignored.");
                emit warningMessage("Configuration Warning", str);
            }
        }
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"ouster\" configuration: ";
        str.append(e.what());
        emit errorMessage("Configuration Error", str);
        return false;
    }

    emit statusMessage("Searching for OUSTER LiDARs...");

    auto sensors = ouster::find_sensors(false, true, false);

    if (sensors.empty())
    {
        emit errorMessage("LiDAR Error", "No Ouster sensors were detected on the network!");
        return false;
    }
    
    mActiveSensor = sensors[0];
    if (sensors.size() > 1)
    {
/*
        std::cout << "The following Ouster sensors were detected:" << std::endl;
        for (auto& sensor : sensors)
        {
            std::cout << "    " << sensor.name << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Please use the \"lidar_hostname\" command line option to select sensor." << std::endl;
*/
        return false;
    }

    auto sensor_ip = mActiveSensor.sensor_ip_address;
    auto dst_ip = mActiveSensor.host_ip_address;
    auto use_ipv6 = mActiveSensor.using_ipv6;

    QString msg("Trying to establishing command connection to ");
    msg.append(mActiveSensor.name.c_str());
    msg.append(" at ");
    msg.append(sensor_ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

    if (!mCmdStream.connect_to_sensor(sensor_ip, use_ipv6))
    {
        emit errorMessage("LiDAR Error", "Could not establish command connection to OUSTER lidar!");
        return false;
    }

    // We have a valid connection, save our parameters for later reconnection.
    mSensorIpAddress = sensor_ip;
    mDstIpAddress = dst_ip;
    mUseIpv6 = use_ipv6;


    if (azimuth_min_deg.has_value() && azimuth_max_deg.has_value())
    {
        mCmdStream.setAzimuthWindow(azimuth_min_deg.value(), azimuth_max_deg.value());
    }

    mCmdStream.retrieveSensorInfo();;

    if (mode.has_value())
    {
        mCmdStream.setLidarMode(mode.value());
    }

    mCmdStream.setUdpDestAuto();
    mCmdStream.reinitialize();

    std::optional<ouster::config_param_t>		configParameters;
    do
    {
        configParameters = mCmdStream.retrieveConfigParam(true);
    } while (!configParameters.has_value());
    mConfigParameters = configParameters.value();

    mImuPort = mCmdStream.retrieveImuUdpPort(true);
    mLidarPort = mCmdStream.retrieveLidarUdpPort(true);

    return true;
}

bool cOusterModel::startCommunications()
{

    emit statusMessage("Trying to establishing IMU connection...");

    if (!cOusterImuStream_Qt::startCommunications(mDstIpAddress, mImuPort, mUseIpv6))
    {
        emit errorMessage("LiDAR Error", "Could not establish IMU data connection to OUSTER lidar!");
        return false;
    }

    emit statusMessage("Trying to establishing LiDAR data connection...");

    if (!cOusterLidarStream_Qt::startCommunications(mDstIpAddress, mLidarPort, mUseIpv6))
    {
        emit errorMessage("LiDAR Error", "Could not establish data connection to OUSTER lidar!");
        return false;
    }

    emit statusMessage("Retrieving OUSTER lidar sensor configuration...");

    std::optional<ouster::sensor_info_t> sensorInfo;
    do
    {
        sensorInfo = mCmdStream.retrieveSensorInfo();
    } while (!sensorInfo.has_value());
    mSensorInfo = sensorInfo.value();
//    emit updateSensorInfo(mSensorInfo);
    emit updateSensorInfo();

    mSerializer.setVersion(mSensorInfo.build_revision.major,
        mSensorInfo.build_revision.minor);


    std::optional<ouster::time_info_t> timeInfo;
    do
    {
        timeInfo = mCmdStream.retrieveTimeInfo();
    } while (!timeInfo.has_value());
    mTimeInfo = timeInfo.value();
//    emit updateTimeInfo(mTimeInfo);
    emit updateTimeInfo();

    std::optional<ouster::beam_intrinsics_t> beamIntrinsics;
    do
    {
        beamIntrinsics = mCmdStream.retrieveBeamIntrinsics();
    } while (!beamIntrinsics.has_value());
    mBeamIntrinsics = beamIntrinsics.value();

    mLidarOriginToBeamOrigin_mm = mBeamIntrinsics.lidar_to_beam_origins_mm;
    for (auto azimuth_deg : mBeamIntrinsics.azimuth_angles_deg)
    {
        mBeamAzimuthAngles_rad.push_back(-1.0 * azimuth_deg * nConstants::DEG_TO_RAD);
    }
    for (auto altitude_deg : mBeamIntrinsics.altitude_angles_deg)
    {
        mBeamAltitudeAngles_rad.push_back(altitude_deg * nConstants::DEG_TO_RAD);
    }
//    emit updateBeamIntrinsics(mBeamIntrinsics);
    emit updateBeamIntrinsics();

    std::optional<ouster::imu_intrinsics_t>	imuIntrinsics;
    do
    {
        imuIntrinsics = mCmdStream.retrieveImuIntrinsics();
    } while (!imuIntrinsics.has_value());
    mImuIntrinsics = imuIntrinsics.value();
//    emit updateImuIntrinsics(mImuIntrinsics);
    emit updateImuIntrinsics();

    std::optional<ouster::lidar_intrinsics_t> lidarIntrinsics;
    do
    {
        lidarIntrinsics = mCmdStream.retrieveLidarIntrinsics();
    } while (!lidarIntrinsics.has_value());
    mLidarIntrinsics = lidarIntrinsics.value();
//    emit updateLidarIntrinsics(mLidarIntrinsics);
    emit updateLidarIntrinsics();

    std::optional<ouster::lidar_data_format_t> dataFormat;
    do
    {
        dataFormat = mCmdStream.retrieveLidarDataFormat();
    } while (!dataFormat.has_value());
    mDataFormat = dataFormat.value();
    cOusterLidarStream_Qt::setDataFormat(mDataFormat);
//    emit updateDataFormat(mDataFormat);
    emit updateDataFormat();

    mSerializer.setBufferCapacity(static_cast<std::size_t>(mDataFormat.pixels_per_column) *
        static_cast<std::size_t>(mDataFormat.columns_per_frame) *
        sizeof(ouster::lidar_data_block_t));

    std::optional<ouster::azimuth_range_t> azimuthWindow;
    do
    {
        azimuthWindow = mCmdStream.retrieveAzimuthWindow(true);
    } while (!azimuthWindow.has_value());
    mAzimuthWindow = azimuthWindow.value();
    emit updateAzimuthWindow(mAzimuthWindow);

    uint32_t min = minEncoderCount();
    uint32_t max = maxEncoderCount();

    emit updateEncoderCount(min, max);


    cOusterImuStream_Qt::clear();
    cOusterLidarStream_Qt::clear();

    mConnected = true;

    return true;
}

void cOusterModel::stopCommunications()
{
    cOusterImuStream_Qt::stopCommunications();
    cOusterLidarStream_Qt::stopCommunications();
}

void cOusterModel::update()
{
    if (!mConnected) return;

    cOusterImuStream_Qt::processOneDatagram();
    cOusterLidarStream_Qt::processOneDatagram();
}

void cOusterModel::writeDataHeader(cBlockDataFile& file)
{
    mSerializer.attach(&file);
    mSerializer.write(mConfigParameters);
    mSerializer.write(mSensorInfo);
    mSerializer.write(mBeamIntrinsics);
    mSerializer.write(mImuIntrinsics);
    mSerializer.write(mLidarIntrinsics);
    mSerializer.write(mDataFormat);
}

void cOusterModel::endDataRecording()
{
    cLidarModel::endDataRecording();
    mSerializer.detach();
}

void cOusterModel::onNewData(const ouster::imu_data_t& data)
{
    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(data);
    }

    mLastImuData = data;
//    emit updateImuData(data);
    emit updateImuData();
}

void cOusterModel::onNewData(uint16_t frameID, const cOusterLidarData& data)
{
    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(frameID, data);
    }

    mLastFrameID = frameID;
    mLastLidarData = data;

    if (--mFrameCounter < 1)
    {
        mFrameCounter = 3;
        emit updateLidarData();
    }
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
