
#pragma once

#include "SsnxModel.hpp"
#include "SsnxSerializer.hpp"

#include <QObject>
#include <QtSerialPort/QSerialPort>

#include <ssnx/ssn_com.hpp>


class cSsnxModel_direct : public cSsnxModel, public SsnCom
{
    Q_OBJECT

public:
    explicit cSsnxModel_direct(QObject* parent = nullptr);
    ~cSsnxModel_direct();

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

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
    
    double  mX_m;
    double  mY_m;
    double  mZ_m;
    double  mVx_mps;
    double  mVy_mps;
    double  mVz_mps;

    cSsnxSerializer mSerializer;
};

