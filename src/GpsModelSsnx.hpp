
#pragma once

#include "GpsModel.hpp"
#include "GpsStreamSsnx.hpp"

#include <QObject>
#include <ssnx/ssn_net_decoder.hpp>

class cGpsModelSsnx : public cGpsModel, public SsnNetDecoder, private cGpsStreamSsnx
{
    Q_OBJECT

public:
    explicit cGpsModelSsnx(QObject* parent = nullptr);
    ~cGpsModelSsnx();

    /*
     * Returns the preferred window title for the corresponding view.
     */
    QString getViewTitle() const override;

    void configure(nlohmann::json& jsonCfg) override;
    void writeDataHeader(cDataFile& file) override;

signals:
    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, cGpsModel::eDatum datum);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);


protected:
    void run() override;

    void pvtGeodetic(const gps::PVT_Geodetic_2_0_t pvt) override;
    void pvtGeodetic(const gps::PVT_Geodetic_2_1_t pvt) override;
    void pvtGeodetic(const gps::PVT_Geodetic_2_2_t pvt) override;
    void receiverTime(const gps::ReceiverTime_t pvt) override;

protected slots:
    void processDatagram(const void* pBuffer, std::size_t buf_length) override;

private:
    bool mConnected;

//    cGpsStreamSsnx mGpsDataStream;
};

