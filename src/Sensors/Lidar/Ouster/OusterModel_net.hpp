
#pragma once

#include "OusterModel.hpp"

#include "OusterDataStream.hpp"
#include "OusterCmdStream.hpp"
#include "OusterImuStream.hpp"

#include <cbdf/OusterSerializer.hpp>

#include <ouster_connect/ouster_defs.h>
#include <ouster_connect/OusterSensorDiscovery.h>

#include <QObject>
#include <QTimer>

#include <queue>

class cOusterAsyncCmd;

class cOusterModel_net : public cOusterModel, private cOusterImuStream_Qt, private cOusterLidarStream_Qt
{
    Q_OBJECT

public:
    cOusterModel_net(QObject* parent = nullptr);
    virtual ~cOusterModel_net();

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;
    bool initialize() override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;
    
    void writeDataHeader() override;

public:
    void setLidarDataFormat(const ouster::lidar_data_format_2_t& format);
    void setLidarMode(ouster::eLIDAR_MODE mode);

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void pauseCommunications();
    void restoreCommunications();
    bool isCommunicationsPaused() const;

// Override of the virtual public slots in cOusterModel
    void changeAzimuthWindow(double min_deg, double max_deg) override;
    void changeLidarMode(QString mode_str) override;

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
    void emitStatusMessage(QString& msg);
    void emitLogMessage(quint8 type, QString msg);

private slots:
    void startCmdQueue();
    void checkCmdQueue();

private:
    bool mConnected;
    bool mPauseCommunications;

    uint16_t mImuPort;
    uint16_t mLidarPort;
    std::string mSensorIpAddress;
    std::string mDstIpAddress;
    bool mUseIpv6;

    cOusterCmdStream_Qt   mCmdStream;

    ouster::sensor_network_info_t mActiveSensor;

    cOusterSerializer mSerializer;

    QTimer mQueueTimer;
    std::queue<cOusterAsyncCmd*> mCmdQueue;

    friend class cOusterAsyncCmd;
};

