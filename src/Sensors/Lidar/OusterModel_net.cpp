
#include "OusterModel_net.hpp"
#include "OusterAsyncCommands.hpp"
#include "../../Utilities/Constants.hpp"

#include <ouster/ouster_utils.h>
#include <optional>
#include <string>


///////////////////////////////////////////////////////////////////////////////
// The OUSTER model class
///////////////////////////////////////////////////////////////////////////////

cOusterModel_net::cOusterModel_net(QObject* parent)
:
    cOusterModel(parent),
    cOusterImuStream_Qt(this),
    cOusterLidarStream_Qt(this),
    mCmdStream(this),
    mQueueTimer(this)
{
    mConnected = false;
    mPauseCommunications = false;
    mFrameCounter = 0;
    mImuPort = 0;
    mLidarPort = 0;
    mUseIpv6 = false;

 //   mCmdStream.enableSocketLogging();

    mQueueTimer.setInterval(10);

    QObject::connect(&mQueueTimer, &QTimer::timeout,
        this, &cOusterModel_net::checkCmdQueue);
}

cOusterModel_net::~cOusterModel_net()
{
    mQueueTimer.stop();
    while (!mCmdQueue.empty())
    {
        auto cmd = mCmdQueue.front();
        mCmdQueue.pop();
        delete cmd;
    }
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
                emit logMessage(logWARNING, q_name(), str);
                azimuth_min_deg.reset();
                azimuth_max_deg.reset();
            }
            else if (azimuth_max_deg <= azimuth_min_deg)
            {
                QString str = "Error in the \"ouster\" configuration:\n";
                str.append("    The minimum angle for the \"azimuth window\" must be less than the maximum angle.\n");
                str.append("\nThe \"azimuth window\" parameters will be ignored.");
                emit logMessage(logWARNING, q_name(), str);
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
                emit logMessage(logWARNING, q_name(), str);
            }
        }
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"ouster\" configuration: ";
        str.append(e.what());
        emit logMessage(logERROR, q_name(), str);
        return false;
    }

    emit logMessage(logSTATUS, q_name(), "Searching for OUSTER LiDARs...");

    auto sensors = ouster::find_sensors(false, true, false);

    if (sensors.empty())
    {
        emit logMessage(logERROR, q_name(), "No Ouster sensors were detected on the network!");
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
        emit logMessage(logERROR, q_name(), "Multiple Ouster sensors were detected on the network!");
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
        emit logMessage(logERROR, q_name(), "Could not establish command connection to OUSTER lidar!");
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

    retrieveConfigParam();

    mImuPort = mCmdStream.retrieveImuUdpPort(true);
    mLidarPort = mCmdStream.retrieveLidarUdpPort(true);

    return cLidarModel::configure(jsonCfg);
}

bool cOusterModel_net::initialize()
{
    emit statusMessage("Retrieving OUSTER lidar sensor configuration...");

//    mCmdStream.enableLogging();

    retrieveSensorInfo();

    std::optional<ouster::time_info_2_t> timeInfo;
    do
    {
        try
        {
            timeInfo = mCmdStream.retrieveTimeInfo();
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Time Info:" << e.what();
            timeInfo.reset();
        }

    } while (!timeInfo.has_value());

    mTimeInfo = timeInfo.value();
    emit updateTimeInfo();

    if (!retrieveBeamIntrinsics())
    {
        qCritical() << "Bad mBeamIntrinsics data!";
        return false;
    }

    if (!retrieveImuIntrinsics())
    {
        qCritical() << "Bad mImuIntrinsics data!";
        return false;
    }

    if (!retrieveLidarIntrinsics())
    {
        qCritical() << "Bad mLidarIntrinsics data!";
        return false;
    }

    if (!retrieveLidarDataFormat())
    {
        qCritical() << "Bad mDataFormat data!";
        return false;
    }

    retrieveAzimuthWindow();

    if (!cOusterImuStream_Qt::determineLocalEndpoint(mDstIpAddress, mImuPort, mUseIpv6))
    {
        emit logMessage(logERROR, q_name(), "Could not establish IMU data connection to OUSTER lidar!");
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    if (!cOusterLidarStream_Qt::determineLocalEndpoint(mDstIpAddress, mImuPort, mUseIpv6))
    {
        emit logMessage(logERROR, q_name(), "Could not establish data connection to OUSTER lidar!");
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    return cOusterModel::initialize();
}

void cOusterModel_net::setLidarDataFormat(const ouster::lidar_data_format_2_t& format)
{
    mDataFormat = format;

    cOusterLidarStream_Qt::setDataFormat(mDataFormat);

    emit updateDataFormat();

    mSerializer.setBufferCapacity(static_cast<std::size_t>(mDataFormat.pixels_per_column) *
        static_cast<std::size_t>(mDataFormat.columns_per_frame) *
        sizeof(ouster::lidar_data_block_t) + 32);

    if (mSerializer)
    {
        mSerializer.write(mDataFormat);
    }
}

void cOusterModel_net::setLidarMode(ouster::eLIDAR_MODE mode)
{
    mConfigParameters.lidar_mode = mode;

    switch (mode)
    {
    case ouster::eLIDAR_MODE::MODE_512x10:
    case ouster::eLIDAR_MODE::MODE_512x20:
        mDataFormat.columns_per_frame = 512;
        break;
    case ouster::eLIDAR_MODE::MODE_1024x10:
    case ouster::eLIDAR_MODE::MODE_1024x20:
        mDataFormat.columns_per_frame = 1024;
        break;
    case ouster::eLIDAR_MODE::MODE_2048x10:
        mDataFormat.columns_per_frame = 2048;
        break;
    }

    cOusterLidarStream_Qt::setDataFormat(mDataFormat);

    emit updateDataFormat();

    mSerializer.setBufferCapacity(static_cast<std::size_t>(mDataFormat.pixels_per_column) *
        static_cast<std::size_t>(mDataFormat.columns_per_frame) *
        sizeof(ouster::lidar_data_block_t) + 32);
}


void cOusterModel_net::changeLidarMode(QString mode_str)
{
    ouster::eLIDAR_MODE mode = to_lidar_mode(mode_str.toStdString());

    if (mConfigParameters.lidar_mode == mode)
        return;

    mCmdQueue.push(new cOusterAsyncCmd_SetLidarMode(this, mode));
    mCmdQueue.push(new cOusterAsyncCmd_Reinitialize(this));
    mCmdQueue.push(new cOusterAsyncCmd_StopDataCollection(this));
    mCmdQueue.push(new cOusterAsyncCmd_WaitForRunning(this));
    mCmdQueue.push(new cOusterAsyncCmd_GetLidarDataFormat(this));
    mCmdQueue.push(new cOusterAsyncCmd_GetLidarMode(this, mode));
    mCmdQueue.push(new cOusterAsyncCmd_StartDataCollection(this));

    startCmdQueue();

    return;
}

void cOusterModel_net::changeAzimuthWindow(double min_deg, double max_deg)
{
    if ((mConfigParameters.azimuth_window.min_deg == min_deg)
        && (mConfigParameters.azimuth_window.max_deg == max_deg))
        return;

    mCmdQueue.push(new cOusterAsyncCmd_SetAzimuthWindow(this, min_deg, max_deg));
    mCmdQueue.push(new cOusterAsyncCmd_Reinitialize(this));
    mCmdQueue.push(new cOusterAsyncCmd_WaitForRunning(this));
    mCmdQueue.push(new cOusterAsyncCmd_GetAzimuthWindow(this, min_deg, max_deg));
    mCmdQueue.push(new cOusterAsyncCmd_GetLidarDataFormat(this));

    startCmdQueue();

    return;
}

bool cOusterModel_net::startCommunications()
{
    setStatus(sensor::eStatus::CONNECTING);

    emit statusMessage("Trying to establishing IMU connection...");

    if (!cOusterImuStream_Qt::startCommunications(mDstIpAddress, mImuPort, mUseIpv6))
    {
        emit logMessage(logERROR, q_name(), "Could not establish IMU data connection to OUSTER lidar!");
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    emit statusMessage("Trying to establishing LiDAR data connection...");

    if (!cOusterLidarStream_Qt::startCommunications(mDstIpAddress, mLidarPort, mUseIpv6))
    {
        emit logMessage(logERROR, q_name(), "Could not establish data connection to OUSTER lidar!");
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    cOusterImuStream_Qt::clear();
    cOusterLidarStream_Qt::clear();

    mConnected = true;

    setStatus(sensor::eStatus::CONNECTED);
    emit statusMessage("LiDAR data connected");

    return true;
}

void cOusterModel_net::stopCommunications()
{
    cOusterImuStream_Qt::stopCommunications();
    cOusterLidarStream_Qt::stopCommunications();

    setStatus(sensor::eStatus::STOPPED);

    mConnected = false;
}

void cOusterModel_net::pauseCommunications()
{
    mPauseCommunications = true;
}

void cOusterModel_net::restoreCommunications()
{
    mPauseCommunications = false;
}

bool cOusterModel_net::isCommunicationsPaused() const
{
    return mPauseCommunications;
}

void cOusterModel_net::update()
{
    if (!mConnected) return;

    if (mPauseCommunications)
    {
        cOusterImuStream_Qt::clear();
        cOusterLidarStream_Qt::clear();
        return;
    }

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

void cOusterModel_net::retrieveConfigParam()
{
    std::optional<ouster::config_param_2_t>	configParameters;
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
}

void cOusterModel_net::retrieveSensorInfo()
{
    std::optional<ouster::sensor_info_2_t> sensorInfo;
    do
    {
        try
        {
            sensorInfo = mCmdStream.retrieveSensorInfo();
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Sensor Info:" << e.what();
            sensorInfo.reset();
        }

    } while (!sensorInfo.has_value());

    mSensorInfo = sensorInfo.value();
    emit updateSensorInfo();

    updateName(mSensorInfo.product_line);

    mSerializer.setVersion(mSensorInfo.build_revision.major,
        mSensorInfo.build_revision.minor);
}

bool cOusterModel_net::retrieveBeamIntrinsics()
{
    std::optional<ouster::beam_intrinsics_2_t> beamIntrinsics;
    do
    {
        try
        {
            beamIntrinsics = mCmdStream.retrieveBeamIntrinsics();
            if (beamIntrinsics.value().azimuth_angles_deg.empty())
                beamIntrinsics.reset();
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Beam Intrinsics:" << e.what();
            beamIntrinsics.reset();
        }

    } while (!beamIntrinsics.has_value());

    mBeamIntrinsics = beamIntrinsics.value();

    if (mBeamIntrinsics.azimuth_angles_deg.empty())
    {
        return false;
    }

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

    return true;
}

bool cOusterModel_net::retrieveImuIntrinsics()
{
    std::optional<ouster::imu_intrinsics_2_t>	imuIntrinsics;
    do
    {
        try
        {
            imuIntrinsics = mCmdStream.retrieveImuIntrinsics();
            if (imuIntrinsics.value().imu_to_sensor_transform.empty())
                imuIntrinsics.reset();
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Imu Intrinsics:" << e.what();
            imuIntrinsics.reset();
        }

    } while (!imuIntrinsics.has_value());

    mImuIntrinsics = imuIntrinsics.value();

    if (mImuIntrinsics.imu_to_sensor_transform.empty())
    {
        return false;
    }

    emit updateImuIntrinsics();

    return true;
}

bool cOusterModel_net::retrieveLidarIntrinsics()
{
    std::optional<ouster::lidar_intrinsics_2_t> lidarIntrinsics;
    do
    {
        try
        {
            lidarIntrinsics = mCmdStream.retrieveLidarIntrinsics();
            if (lidarIntrinsics.value().lidar_to_sensor_transform.empty())
                lidarIntrinsics.reset();
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Lidar Intrinsics:" << e.what();
            lidarIntrinsics.reset();
        }

    } while (!lidarIntrinsics.has_value());

    mLidarIntrinsics = lidarIntrinsics.value();
    if (mLidarIntrinsics.lidar_to_sensor_transform.empty())
    {
        return false;
    }

    emit updateLidarIntrinsics();

    return true;
}

bool cOusterModel_net::retrieveLidarDataFormat()
{
    std::optional<ouster::lidar_data_format_2_t> dataFormat;
    do
    {
        try
        {
            dataFormat = mCmdStream.retrieveLidarDataFormat();
            if ((dataFormat.value().pixels_per_column < 32))
                dataFormat.reset();
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Data Format:" << e.what();
            dataFormat.reset();
        }

    } while (!dataFormat.has_value());

    mDataFormat = dataFormat.value();

    if (!((mDataFormat.columns_per_frame == 512) 
        || (mDataFormat.columns_per_frame == 1024)
        || (mDataFormat.columns_per_frame == 2048)))
    {
        return false;
    }

    if (!((mDataFormat.pixels_per_column == 32) || (mDataFormat.pixels_per_column == 64)
        || (mDataFormat.pixels_per_column == 128)))
    {
        return false;
    }

    cOusterLidarStream_Qt::setDataFormat(mDataFormat);

    emit updateDataFormat();

    mSerializer.setBufferCapacity(static_cast<std::size_t>(mDataFormat.pixels_per_column) *
        static_cast<std::size_t>(mDataFormat.columns_per_frame) *
        sizeof(ouster::lidar_data_block_t) + 32);

    return true;
}

void cOusterModel_net::retrieveLidarMode()
{
    std::optional<ouster::eLIDAR_MODE> lidarMode;
    do
    {
        try
        {
            lidarMode = mCmdStream.retrieveLidarMode(true);
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Lidar Mode:" << e.what();
            lidarMode.reset();
        }

    } while (!lidarMode.has_value());

    mConfigParameters.lidar_mode = lidarMode.value();
}

void cOusterModel_net::retrieveAzimuthWindow()
{
    std::optional<ouster::azimuth_range_t> azimuthWindow;
    do
    {
        try
        {
            azimuthWindow = mCmdStream.retrieveAzimuthWindow(true);
        }
        catch (const std::exception& e)
        {
            qCritical() << "Exception Azimuth Window:" << e.what();
            azimuthWindow.reset();
        }

    } while (!azimuthWindow.has_value());

    mConfigParameters.azimuth_window = azimuthWindow.value();

    emit updateAzimuthWindow();
}

void cOusterModel_net::emitStatusMessage(QString& msg)
{
    emit statusMessage(msg);
}

void cOusterModel_net::emitLogMessage(quint8 type, QString msg)
{
    emit logMessage(type, q_name(), msg);
}

void cOusterModel_net::startCmdQueue()
{
/*
    QString msg = "Starting command queue...";
    emit statusMessage(msg);
    emit logMessage(logINFO, q_name(), msg);
*/

    if (mQueueTimer.isActive()) return;

    while (!mCmdQueue.empty())
    {
        auto cmd = mCmdQueue.front();
        if (cmd->postCmd())
            break;

        // Command failed!
        mCmdQueue.pop();
        delete cmd;
    }

    mQueueTimer.start();
}

void cOusterModel_net::checkCmdQueue()
{
    if (mCmdQueue.empty())
    {
/*
        QString msg = "Command queue complete";
        emit statusMessage(msg);
        emit logMessage(logINFO, q_name(), msg);
*/

        mQueueTimer.stop();
        return;
    }

    auto cmd = mCmdQueue.front();

    if (!cmd->complete())
        return;

    mCmdQueue.pop();
    delete cmd;

    while (!mCmdQueue.empty())
    {
        cmd = mCmdQueue.front();
        if (cmd->postCmd())
            break;

        // Command failed!
        mCmdQueue.pop();
        delete cmd;
    }

    if (mCmdQueue.empty())
    {
        mQueueTimer.stop();
        return;
    }
}

