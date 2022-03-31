
#pragma once

#include "GpsModel.hpp"
#include "SsnxGpsStream.hpp"
#include "SsnxSerializer.hpp"

#include <QObject>

#include <ssnx/ssn_net_decoder.hpp>


class cSsnxModel : public cGpsModel, public SsnNetDecoder, private cSsnxGpsStream
{
    Q_OBJECT

public:
    explicit cSsnxModel(QObject* parent = nullptr);
    ~cSsnxModel();

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

 
    bool configure(const nlohmann::json& jsonCfg) override;

    void writeDataHeader(cBlockDataFile& file) override;
    void endDataRecording() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

signals:
    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, cGpsModel::eDatum datum);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);


protected:
    void update() override;

    void pvtGeodetic(const ssnx::gps::PVT_Geodetic_2_t pvt) override;
    void posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov) override;
    void velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov) override;
    void posProjected(const ssnx::gps::POS_Projected_1_t pvt) override;
    void receiverTime(const ssnx::gps::ReceiverTime_1_t pvt) override;
    void rtcmDatum(const ssnx::gps::RtcmDatum_1_t rtcm) override;

protected slots:
    void processDatagram(const void* pBuffer, std::size_t buf_length) override;

private:
    cSsnxSerializer mSerializer;
    bool mConnected;
};

