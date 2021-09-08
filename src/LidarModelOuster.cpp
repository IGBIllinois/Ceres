
#include "LidarModelOuster.hpp"

#include <QMessageBox>

cLidarModelOuster::cLidarModelOuster(QObject* parent)
:
    cLidarModel(parent),
    mCmdStream(parent),
    mDataStream(parent),
    mImuStream(parent)
{
}

void cLidarModelOuster::configure(nlohmann::json& jsonCfg)
{
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
        return;
    }

    mConfigParameters = mCmdStream.retrieveConfigParam(true);
    mSensorInfo = mCmdStream.retrieveSensorInfo();
    mTimeInfo = mCmdStream.retrieveTimeInfo();
    mBeamIntrinsics = mCmdStream.retrieveBeamIntrinsics();
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

    QObject::connect(&mImuStream, &cOusterImuStream_Qt::dataUpdated, this, &cLidarModelOuster::processImuData);

    if (!mImuStream.connect_to_sensor(dst_ip, imu_port, use_ipv6))
    {
        return;
    }

    mDataStream.setDataFormat(mDataFormat);
    QObject::connect(&mDataStream, &cOusterLidarStream_Qt::dataUpdated, this, &cLidarModelOuster::processLidarData);

    if (!mDataStream.connect_to_sensor(dst_ip, lidar_port, use_ipv6))
    {
        return;
    }

    mImuStream.clear();
    mDataStream.clear();

/*
    std::chrono::milliseconds ms(10);
    for (int i = 0; i < 3000; ++i)
    {
        imuStream.receive_data();
        lidarStream.receive_data();
        std::this_thread::sleep_for(ms);
    }
*/
}

void cLidarModelOuster::writeDataHeader(cDataFile& file)
{

}

void cLidarModelOuster::processImuData(ouster::imu_data_t data)
{
    mLastImuData = data;
}

void cLidarModelOuster::processLidarData(uint16_t frameID, ouster::lidar_data_t data)
{
    mLastFrameID = frameID;
    mLastLidarData = data;
}
