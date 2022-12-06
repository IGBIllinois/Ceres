
#pragma once

#include "SsnxModel.hpp"
#include "SsnxGpsStream.hpp"

#include <QObject>

#include <ssnx/ssn_net_decoder.hpp>


class cSsnxModel_net : public cSsnxModel, public SsnNetDecoder, private cSsnxGpsStream
{
    Q_OBJECT

public:
    explicit cSsnxModel_net(QObject* parent = nullptr);
    ~cSsnxModel_net();

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

    void pvtCartesian(const ssnx::gps::PVT_Cartesian_2_t pvt) override;
    void pvtGeodetic(const ssnx::gps::PVT_Geodetic_2_t pvt) override;
    void posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov) override;
    void velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov) override;
    void posProjected(const ssnx::gps::POS_Projected_1_t pvt) override;
    void receiverTime(const ssnx::gps::ReceiverTime_1_t pvt) override;
    void rtcmDatum(const ssnx::gps::RtcmDatum_1_t rtcm) override;

protected slots:
    void processDatagram(const void* pBuffer, std::size_t buf_length) override;

private:
    bool mConnected;
};

