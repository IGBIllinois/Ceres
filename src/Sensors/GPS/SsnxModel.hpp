
#pragma once

#include "GpsModel.hpp"

#include <QObject>


class cSsnxModel : public cGpsModel
{
    Q_OBJECT

public:
    explicit cSsnxModel(QObject* parent = nullptr);
    ~cSsnxModel();

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

signals:
    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, gps::eDatum datum);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);
};

