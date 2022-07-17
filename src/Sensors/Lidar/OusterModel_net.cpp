
#include "OusterModel_net.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cOusterModel_net::cOusterModel_net(QObject* parent)
:
    cOusterModel(parent),
    cOusterImuStream_Qt(this),
    cOusterLidarStream_Qt(this),
    mCmdStream(this)
{
    mConnected = false;
    mFrameCounter = 0;
    mImuPort = 0;
    mLidarPort = 0;
    mUseIpv6 = false;
}

uint16_t cOusterModel_net::data_class_id() const
{
    return mSerializer.classID();
}

bool cOusterModel_net::configure(const nlohmann::json& jsonCfg)
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
        emit errorMessage("LiDAR Error", "Multiple Ouster sensors were detected on the network!");
        return false;
    }

    auto sensor_ip = mActiveSensor.sensor_ip_address;
    auto local_ip = mActiveSensor.host_ip_address;
    auto use_ipv6 = mActiveSensor.using_ipv6;

    QString msg("Trying to establishing command connection to ");
    msg.append(mActiveSensor.name.c_str());
    msg.append(" at ");
    msg.append(sensor_ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

    if (!mCmdStream.connect_to_sensor(sensor_ip, use_ipv6, local_ip))
    {
        emit errorMessage("LiDAR Error", "Could not establish command connection to OUSTER lidar!");
        return false;
    }

    // We have a valid connection, save our parameters for later reconnection.
    mSensorIpAddress = sensor_ip;
    mDstIpAddress = local_ip;
    mUseIpv6 = use_ipv6;

/* BAF: Disabled for now.
    if (azimuth_min_deg.has_value() && azimuth_max_deg.has_value())
    {
        QString msg("Setting azimuth window to ");
        msg.append(QString::number(azimuth_min_deg.value()));
        msg.append(" degrees to  ");
        msg.append(QString::number(azimuth_max_deg.value()));
        msg.append(" degrees.");
        emit statusMessage(msg);

        mCmdStream.setAzimuthWindow(azimuth_min_deg.value(), azimuth_max_deg.value());
    }
*/

    if (mode.has_value())
    {
        mCmdStream.setLidarMode(mode.value());
    }

    mCmdStream.setUdpDestAuto();
    mCmdStream.reinitialize();

    emit statusMessage("Retrieving OUSTER lidar configuration parameters...");

    std::optional<ouster::config_param_2_t>		configParameters;
    do
    {
        try
        {
            configParameters = mCmdStream.retrieveConfigParam(true);
        }
        catch (const std::exception& e)
        {
        }

    } while (!configParameters.has_value());

    mConfigParameters = configParameters.value();

    mImuPort = mCmdStream.retrieveImuUdpPort(true);
    mLidarPort = mCmdStream.retrieveLidarUdpPort(true);

    return cLidarModel::configure(jsonCfg);
}

bool cOusterModel_net::initialize()
{
    emit statusMessage("Retrieving OUSTER lidar sensor configuration...");

    //mCmdStream.enableLogging();

    std::optional<ouster::sensor_info_2_t> sensorInfo;
    do
    {
        try
        {
            sensorInfo = mCmdStream.retrieveSensorInfo();
        }
        catch (const std::exception& e)
        {
        }

    } while (!sensorInfo.has_value());

    mSensorInfo = sensorInfo.value();
    emit updateSensorInfo();

    updateName(mSensorInfo.product_line);

    mSerializer.setVersion(mSensorInfo.build_revision.major,
        mSensorInfo.build_revision.minor);


    std::optional<ouster::time_info_2_t> timeInfo;
    do
    {
        try
        {
            timeInfo = mCmdStream.retrieveTimeInfo();
        }
        catch (const std::exception& e)
        {
        }

    } while (!timeInfo.has_value());

    mTimeInfo = timeInfo.value();
    emit updateTimeInfo();

    std::optional<ouster::beam_intrinsics_2_t> beamIntrinsics;
    do
    {
        try
        {
            beamIntrinsics = mCmdStream.retrieveBeamIntrinsics();
        }
        catch (const std::exception& e)
        {
        }

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

    emit updateBeamIntrinsics();

    std::optional<ouster::imu_intrinsics_2_t>	imuIntrinsics;
    do
    {
        try
        {
            imuIntrinsics = mCmdStream.retrieveImuIntrinsics();
        }
        catch (const std::exception& e)
        {
        }

    } while (!imuIntrinsics.has_value());

    mImuIntrinsics = imuIntrinsics.value();
    emit updateImuIntrinsics();

    std::optional<ouster::lidar_intrinsics_2_t> lidarIntrinsics;
    do
    {
        try
        {
            lidarIntrinsics = mCmdStream.retrieveLidarIntrinsics();
        }
        catch (const std::exception& e)
        {
        }

    } while (!lidarIntrinsics.has_value());

    mLidarIntrinsics = lidarIntrinsics.value();
    emit updateLidarIntrinsics();

    std::optional<ouster::lidar_data_format_2_t> dataFormat;
    do
    {
        try
        {
            dataFormat = mCmdStream.retrieveLidarDataFormat();
        }
        catch (const std::exception& e)
        {
        }

    } while (!dataFormat.has_value());

    mDataFormat = dataFormat.value();
    cOusterLidarStream_Qt::setDataFormat(mDataFormat);
    emit updateDataFormat();

    mSerializer.setBufferCapacity(static_cast<std::size_t>(mDataFormat.pixels_per_column) *
        static_cast<std::size_t>(mDataFormat.columns_per_frame) *
        sizeof(ouster::lidar_data_block_t) + 32);

    std::optional<ouster::azimuth_range_t> azimuthWindow;
    do
    {
        try
        {
            azimuthWindow = mCmdStream.retrieveAzimuthWindow(true);
        }
        catch (const std::exception& e)
        {
        }

    } while (!azimuthWindow.has_value());

    mAzimuthWindow = azimuthWindow.value();
    emit updateAzimuthWindow();

    uint32_t min = minEncoderCount();
    uint32_t max = maxEncoderCount();

    emit updateEncoderCount(min, max);

    return cOusterModel::initialize();
}

bool cOusterModel_net::startCommunications()
{
    emit statusMessage("Trying to establishing IMU connection...");

    if (!cOusterImuStream_Qt::startCommunications(mDstIpAddress, mImuPort, mUseIpv6))
    {
        emit errorMessage("LiDAR Error", "Could not establish IMU data connection to OUSTER lidar!");
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    emit statusMessage("Trying to establishing LiDAR data connection...");

    if (!cOusterLidarStream_Qt::startCommunications(mDstIpAddress, mLidarPort, mUseIpv6))
    {
        emit errorMessage("LiDAR Error", "Could not establish data connection to OUSTER lidar!");
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    cOusterImuStream_Qt::clear();
    cOusterLidarStream_Qt::clear();

    mConnected = true;

    setStatus(sensor::eStatus::CONNECTING);

    return true;
}

void cOusterModel_net::stopCommunications()
{
    cOusterImuStream_Qt::stopCommunications();
    cOusterLidarStream_Qt::stopCommunications();

    setStatus(sensor::eStatus::STOPPED);
}

void cOusterModel_net::update()
{
    if (!mConnected) return;

    cOusterImuStream_Qt::processOneDatagram();
    cOusterLidarStream_Qt::processOneDatagram();

    if (status() != sensor::eStatus::RUNNING)
        setStatus(sensor::eStatus::RUNNING);
}

void cOusterModel_net::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cOusterModel_net::disableDataRecording()
{
    cLidarModel::disableDataRecording();
    mSerializer.detach();
}

void cOusterModel_net::writeDataHeader()
{
    mSerializer.write(mConfigParameters);
    mSerializer.write(mSensorInfo);
    mSerializer.write(mBeamIntrinsics);
    mSerializer.write(mImuIntrinsics);
    mSerializer.write(mLidarIntrinsics);
    mSerializer.write(mDataFormat);
}

void cOusterModel_net::onNewData(const ouster::imu_data_t& data)
{
    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(data);
    }

    mLastImuData = data;
    emit updateImuData();
}

void cOusterModel_net::onNewData(uint16_t frameID, const cOusterLidarData& data)
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

