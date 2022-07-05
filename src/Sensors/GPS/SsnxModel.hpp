
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
    char* descriptor() const override;

    virtual bool configure(const nlohmann::json& jsonCfg) = 0;

signals:
    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, cGpsModel::eDatum datum);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);
};

