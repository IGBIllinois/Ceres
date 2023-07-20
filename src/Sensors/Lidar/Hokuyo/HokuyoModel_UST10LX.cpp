
#include "HokuyoModel_UST10LX.hpp"
#include "Constants.hpp"

#include <optional>
#include <string>


///////////////////////////////////////////////////////////////////////////////
// The Hokuyo UST-10LX model class
///////////////////////////////////////////////////////////////////////////////

cHokuyoModel_UST10LX::cHokuyoModel_UST10LX(QObject* parent)
:
    cHokuyoModel(parent)
{
    mModel = "UST-10LX";

    mConnected = false;
    mPauseCommunications = false;
    mFrameCounter = 0;
    mLidarPort = 0;
}

cHokuyoModel_UST10LX::~cHokuyoModel_UST10LX()
{
}


uint16_t cHokuyoModel_UST10LX::data_class_id() const
{
    return mSerializer.classID();
}

void cHokuyoModel_UST10LX::updateViews()
{

}

bool cHokuyoModel_UST10LX::configure(const nlohmann::json& jsonCfg)
{
/*
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

    qInfo() << "Searching for OUSTER LiDARs...";
    emit logMessage(logSTATUS, q_name(), "Searching for OUSTER LiDARs...");

    auto sensors = ouster::find_sensors(false, true, false);

    if (sensors.empty())
    {
        qCritical() << "No Ouster sensors were detected on the network!";
        emit logMessage(logERROR, q_name(), "No Ouster sensors were detected on the network!");
        return false;
    }
    
    mActiveSensor = sensors[0];
    if (sensors.size() > 1)
    {
        emit logMessage(logERROR, q_name(), "Multiple Hokuyo sensors were detected on the network!");
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

    qInfo() << msg;

    emit statusMessage(msg);

    if (!mCmdStream.connect_to_sensor(sensor_ip, use_ipv6, local_ip))
    {
        emit logMessage(logERROR, q_name(), "Could not establish command connection to OUSTER lidar!");
        return false;
    }

    // We have a valid connection, save our parameters for later reconnection.
    mSensorIpAddress = sensor_ip;
    mDstIpAddress = local_ip;

    if (mode.has_value())
    {
        mCmdStream.setLidarMode(mode.value());
    }

    mCmdStream.setUdpDestAuto();
    mCmdStream.reinitialize();

    emit statusMessage("Retrieving Hokuyo lidar configuration parameters...");

    retrieveConfigParam();

    mImuPort = mCmdStream.retrieveImuUdpPort(true);
    mLidarPort = mCmdStream.retrieveLidarUdpPort(true);
*/

    return cLidarModel::configure(jsonCfg);
}

bool cHokuyoModel_UST10LX::initialize()
{
    emit statusMessage("Retrieving Hokuyo lidar sensor configuration...");

/*
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
*/

    return cHokuyoModel::initialize();
}

bool cHokuyoModel_UST10LX::startCommunications()
{
    setStatus(sensor::eStatus::CONNECTING);

    emit statusMessage("Trying to establishing LiDAR data connection...");

/*
    if (!cOusterLidarStream_Qt::startCommunications(mDstIpAddress, mLidarPort, mUseIpv6))
    {
        emit logMessage(logERROR, q_name(), "Could not establish data connection to OUSTER lidar!");
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    cOusterLidarStream_Qt::clear();
*/

    mConnected = true;

    setStatus(sensor::eStatus::CONNECTED);
    emit statusMessage("LiDAR data connected");

    return true;
}

void cHokuyoModel_UST10LX::stopCommunications()
{
//    cOusterLidarStream_Qt::stopCommunications();

    setStatus(sensor::eStatus::STOPPED);

    mConnected = false;
}

void cHokuyoModel_UST10LX::pauseCommunications()
{
    mPauseCommunications = true;
}

void cHokuyoModel_UST10LX::restoreCommunications()
{
    mPauseCommunications = false;
}

bool cHokuyoModel_UST10LX::isCommunicationsPaused() const
{
    return mPauseCommunications;
}

void cHokuyoModel_UST10LX::update()
{
    if (!mConnected) return;

    if (mPauseCommunications)
    {
//        cOusterLidarStream_Qt::clear();
        return;
    }

//    cOusterLidarStream_Qt::processOneDatagram();

    if (getStatus() != sensor::eStatus::RUNNING)
        setStatus(sensor::eStatus::RUNNING);
}

void cHokuyoModel_UST10LX::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cHokuyoModel_UST10LX::disableDataRecording()
{
    cLidarModel::disableDataRecording();
    mSerializer.detach();
}

void cHokuyoModel_UST10LX::writeDataHeader()
{
//    mSerializer.write(mConfigParameters);
}

/*
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
*/

void cHokuyoModel_UST10LX::emitStatusMessage(QString& msg)
{
    emit statusMessage(msg);
}

void cHokuyoModel_UST10LX::emitLogMessage(quint8 type, QString msg)
{
    emit logMessage(type, q_name(), msg);
}

