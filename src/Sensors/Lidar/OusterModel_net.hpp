
#pragma once

#include "OusterModel.hpp"

#include "OusterDataStream.hpp"
#include "OusterCmdStream.hpp"
#include "OusterImuStream.hpp"
#include "OusterSerializer.hpp"

#include <ouster/ouster_defs.h>
#include <ouster/OusterSensorDiscovery.h>

#include <QObject>

class cOusterModel_net : public cOusterModel, private cOusterImuStream_Qt, private cOusterLidarStream_Qt
{
    Q_OBJECT

public:
    cOusterModel_net(QObject* parent = nullptr);
    virtual ~cOusterModel_net() = default;

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;
    bool initialize() override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;
    
    void writeDataHeader() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

public slots:
    void setAzimuthWindow(double min_deg, double max_deg);
    //bool setLidarMode(ouster::eLIDAR_MODE mode);
    void setLidarMode(QString mode_str);

protected:
    void onNewData(const ouster::imu_data_t& new_data) override;
    void onNewData(uint16_t frameID, const cOusterLidarData& data) override;

protected:
    void update() override;

private:
    void retrieveConfigParam();
    void retrieveSensorInfo();
    bool retrieveBeamIntrinsics();
    bool retrieveImuIntrinsics();
    bool retrieveLidarIntrinsics();
    bool retrieveLidarDataFormat();

    void retrieveLidarMode();
    void retrieveAzimuthWindow();

private:
    bool mConnected;

    uint16_t mImuPort;
    uint16_t mLidarPort;
    std::string mSensorIpAddress;
    std::string mDstIpAddress;
    bool mUseIpv6;

    cOusterCmdStream_Qt   mCmdStream;

    ouster::sensor_network_info_t mActiveSensor;

    cOusterSerializer mSerializer;
};

