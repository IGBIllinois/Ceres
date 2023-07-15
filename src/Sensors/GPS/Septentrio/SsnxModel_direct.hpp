
#pragma once

#include "SsnxModel.hpp"
#include "Utilities.hpp"

#include <QObject>
#include <QtSerialPort/QSerialPort>

#include <ssnx/ssn_com.hpp>


class cSsnxModel_direct : public cSsnxModel, public cSsnCom
{
    Q_OBJECT

public:
    explicit cSsnxModel_direct(QObject* parent = nullptr);
    ~cSsnxModel_direct();

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void writeDataHeader() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

protected:
    void update() override;

    void pvtCartesian(const ssnx::gps::PVT_Cartesian_2_t& pvt) override;
    void pvtGeodetic(const ssnx::gps::PVT_Geodetic_2_t& pvt) override;
    void posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov) override;
    void velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov) override;
    void posProjected(const ssnx::gps::POS_Projected_1_t& pvt) override;
    void receiverTime(const ssnx::gps::ReceiverTime_1_t& pvt) override;
    void rtcmDatum(const ssnx::gps::RtcmDatum_1_t& rtcm) override;

private:
    void closeConnection() override;
    bool isConnected() override;
    void communicationError(const std::string& errorString) override;

    void newConnectionDescriptor(const std::string& connectionDescriptor) override;
    void newCommandReply(const std::string& reply, bool error) override;
    void newFormattedInformationBlock(const std::string& contents, int index, int count) override;
    void newAsciiDisplay(const std::string& asciiDisplay) override;
    void newEvent(const std::string& event) override {};
    void stopReceived() override;
    void sentAsciiCommand(const std::string& command) override;

    int readIncomingData(std::string& data) override;
    int sendOutgoingData(const std::string& data) override;

private:
    QSerialPort mSerialPort;
    QByteArray  mSerialBuffer;

    ///< the command queue in case the receiver is turned off and then back on
    std::queue<std::string> mSavedCommandQueue; 
    
    struct sCartesian_t
    {
        bool dataValid = false;
        double  timestamp_s;
        ::gps::eDatum datum = ::gps::eDatum::WGS84;
        double  X_m = 0;
        double  Y_m = 0;
        double  Z_m = 0;
        double  Vx_mps = 0;
        double  Vy_mps = 0;
        double  Vz_mps = 0;
        double  groundtrack_deg = 0;
        float   hAccuracy_m = -1.0f;
        float   vAccuracy_m = -1.0f;
    };

    sCartesian_t mCartesianPVT;

    struct sPosPojected_t
    {
        bool    dataValid = false;
        double  timestamp_s = 0;
        ::gps::eDatum datum = ::gps::eDatum::WGS84;
        bool    HeightComputed = false;
        double	Northing_m = 0;
        double  Easting_m = 0;
        double  Alt_m = 0;
    };

    sPosPojected_t mPosPojected;

    edge_detect<bool> mPvtCartesianValid;
    edge_detect<bool> mPvtGeodeticValid;
    edge_detect<bool> mPosCovGeodeticValid;
    edge_detect<bool> mVelCovGeodeticValid;
    edge_detect<bool> mPosProjectedValid;
    edge_detect<bool> mReceiverTimeValid;
    edge_detect<bool> mRtcmDatumValid;
};

