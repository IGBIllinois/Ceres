
#include "OusterModel.hpp"
#include "../Utilities/Constants.hpp"

#include <QMessageBox>
#include <optional>

cOusterModel::cOusterModel(QObject* parent)
:
    cLidarModel(parent),
    mCmdStream(),
    mLastFrameID(0)
{
    mConnected = false;
    mFrameCounter = 0;
}

void cOusterModel::configure(const nlohmann::json& jsonCfg)
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
                QMessageBox msg(QMessageBox::Warning, "Configuration Warning", str);
                msg.exec();
                azimuth_min_deg.reset();
                azimuth_max_deg.reset();
            }
            else if (azimuth_max_deg <= azimuth_min_deg)
            {
                QString str = "Error in the \"ouster\" configuration:\n";
                str.append("    The minimum angle for the \"azimuth window\" must be less than the maximum angle.\n");
                str.append("\nThe \"azimuth window\" parameters will be ignored.");
                QMessageBox msg(QMessageBox::Warning, "Configuration Warning", str);
                msg.exec();
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
                QMessageBox msg(QMessageBox::Warning, "Configuration Warning", str);
                msg.exec();
            }
        }
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"ouster\" configuration: ";
        str.append(e.what());
        QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
        msg.exec();
        return;
    }

    emit statusMessage("Searching for OUSTER LiDARs...");

    auto sensors = ouster::find_sensors(false, true, false);

    if (sensors.empty())
    {
        QMessageBox msg(QMessageBox::Critical, "LiDAR Error", "No Ouster sensors were detected on the network!");
        msg.exec();
        return;
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
        return;
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
        QMessageBox msg(QMessageBox::Critical, "LiDAR Error", "Could not establish command connection to OUSTER lidar!");
        msg.exec();
        return;
    }


    if (azimuth_min_deg.has_value() && azimuth_max_deg.has_value())
    {
        mCmdStream.setAzimuthWindow(azimuth_min_deg.value(), azimuth_max_deg.value());
    }

    if (mode.has_value())
    {
        mCmdStream.setLidarMode(mode.value());
    }

    mCmdStream.setUdpDestAuto();
    mCmdStream.reinitialize();


    mConfigParameters = mCmdStream.retrieveConfigParam(true);
    mSensorInfo = mCmdStream.retrieveSensorInfo();
    mTimeInfo = mCmdStream.retrieveTimeInfo();
    mBeamIntrinsics = mCmdStream.retrieveBeamIntrinsics();

    mLidarOriginToBeamOrigin_mm = mBeamIntrinsics.lidar_to_beam_origins_mm;
    for (auto azimuth_deg : mBeamIntrinsics.azimuth_angles_deg)
    {
        mBeamAzimuthAngles_rad.push_back( -1.0 * azimuth_deg * nConstants::DEG_TO_RAD);
    }
    for (auto altitude_deg : mBeamIntrinsics.altitude_angles_deg)
    {
        mBeamAltitudeAngles_rad.push_back(altitude_deg * nConstants::DEG_TO_RAD);
    }

    mImuIntrinsics = mCmdStream.retrieveImuIntrinsics();
    mLidarIntrinsics = mCmdStream.retrieveLidarIntrinsics();
    mDataFormat = mCmdStream.retrieveLidarDataFormat();
    mAzimuthWindow = mCmdStream.retrieveAzimuthWindow(true);

    auto imu_port = mCmdStream.retrieveImuUdpPort(true);
    auto lidar_port = mCmdStream.retrieveLidarUdpPort(true);

    emit statusMessage("Trying to establishing IMU connection...");

    if (!cOusterImuStream_Qt::connect_to_sensor(dst_ip, imu_port, use_ipv6))
    {
        QMessageBox msg(QMessageBox::Critical, "LiDAR Error", "Could not establish IMU data connection to OUSTER lidar!");
        msg.exec();
        return;
    }

    cOusterLidarStream_Qt::setDataFormat(mDataFormat);

    emit statusMessage("Trying to establishing LiDAR data connection...");

    if (!cOusterLidarStream_Qt::connect_to_sensor(dst_ip, lidar_port, use_ipv6))
    {
        QMessageBox msg(QMessageBox::Critical, "LiDAR Error", "Could not establish data connection to OUSTER lidar!");
        msg.exec();
        return;
    }

    cOusterImuStream_Qt::clear();
    cOusterLidarStream_Qt::clear();

    mConnected = true;
}

void cOusterModel::update()
{
    if (!mConnected) return;

    cOusterImuStream_Qt::processOneDatagram();
    cOusterLidarStream_Qt::processOneDatagram();
}

void cOusterModel::writeDataHeader(cBlockDataFile& file)
{

}

void cOusterModel::onNewData(const ouster::imu_data_t& data)
{
    mLastImuData = data;
}

void cOusterModel::onNewData(uint16_t frameID, ouster::lidar_data_t& data)
{
    mLastFrameID = frameID;
    mLastLidarData = data;

    if (--mFrameCounter < 1)
    {
        mFrameCounter = 3;
        emit updateView();
    }
}

uint16_t cOusterModel::columnsPerFrame() const
{
    return mDataFormat.columns_per_frame;
}

//   std::vector<int> cLidarModelOuster::pixelShiftByRow() const;

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
    return 22528;
}

uint32_t cOusterModel::maxEncoderCount() const
{
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

ouster::lidar_data_t cOusterModel::lidarData() const
{
    return mLastLidarData;
}

ouster::imu_data_t cOusterModel::imuData() const
{
    return mLastImuData;
}
