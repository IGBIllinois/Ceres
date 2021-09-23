
#include "LidarModelOuster.hpp"

#include <QMessageBox>

cLidarModelOuster::cLidarModelOuster(QObject* parent)
:
    cLidarModel(parent),
    mCmdStream(),
    mLastFrameID(0)
{
    mConnected = false;
}

QString cLidarModelOuster::getViewTitle() const
{
    return "OUSTER LiDAR";
}

void cLidarModelOuster::configure(nlohmann::json& jsonCfg)
{
    try
    {
        cLidarModel::configure(jsonCfg);
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"ouster\" configuration: ";
        str.append(e.what());
        QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
        msg.exec();
        return;
    }


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

    if (!mCmdStream.connect_to_sensor(sensor_ip, use_ipv6))
    {
        QMessageBox msg(QMessageBox::Critical, "LiDAR Error", "Could not establish command connection to OUSTER lidar!");
        msg.exec();
        return;
    }

    mConfigParameters = mCmdStream.retrieveConfigParam(true);
    mSensorInfo = mCmdStream.retrieveSensorInfo();
    mTimeInfo = mCmdStream.retrieveTimeInfo();
    mBeamIntrinsics = mCmdStream.retrieveBeamIntrinsics();

    mLidarOriginToBeamOrigin_mm = mBeamIntrinsics.lidar_to_beam_origins_mm;
    std::vector<double> mBeamAzimuthAngles_rad;
    std::vector<double> mBeamAltitudeAngles_rad;

    mImuIntrinsics = mCmdStream.retrieveImuIntrinsics();
    mLidarIntrinsics = mCmdStream.retrieveLidarIntrinsics();
    mDataFormat = mCmdStream.retrieveLidarDataFormat();
    mAzimuthWindow = mCmdStream.retrieveAzimuthWindow(true);

    auto imu_port = mCmdStream.retrieveImuUdpPort(true);
    auto lidar_port = mCmdStream.retrieveLidarUdpPort(true);

    mCmdStream.setUdpDestAuto();
    mCmdStream.reinitialize();

    /*
        cmdStream.setLidarMode(ouster::eLIDAR_MODE::MODE_1024x10);
        cmdStream.reinitialize();
        cmdStream.saveConfigParams();
    */

    if (!cOusterImuStream_Qt::connect_to_sensor(dst_ip, imu_port, use_ipv6))
    {
        QMessageBox msg(QMessageBox::Critical, "LiDAR Error", "Could not establish IMU data connection to OUSTER lidar!");
        msg.exec();
        return;
    }

    cOusterLidarStream_Qt::setDataFormat(mDataFormat);

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

void cLidarModelOuster::run()
{
    if (!mConnected) return;

    cOusterImuStream_Qt::processOneDatagram();
    cOusterLidarStream_Qt::processOneDatagram();
}

void cLidarModelOuster::writeDataHeader(cDataFile& file)
{

}

void cLidarModelOuster::onNewData(const ouster::imu_data_t& data)
{
    mLastImuData = data;
}

void cLidarModelOuster::onNewData(uint16_t frameID, ouster::lidar_data_t& data)
{
    mLastFrameID = frameID;
    mLastLidarData = data;
}
