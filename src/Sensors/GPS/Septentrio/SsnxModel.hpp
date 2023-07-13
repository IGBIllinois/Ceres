
#pragma once

#include "../GpsModel.hpp"

#include <cbdf/SsnxSerializer.hpp>

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

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

signals:
    void pvtCartesianStateChanged(bool valid);
    void pvtGeodeticStateChanged(bool valid);
    void posCovGeodeticStateChanged(bool valid);
    void velCovGeodeticStateChanged(bool valid);
    void posProjectedStateChanged(bool valid);
    void receiverTimeStateChanged(bool valid);
    void rtcmDatumStateChanged(bool valid);

signals:
    void updateGeodeticPVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, ::gps::eDatum datum);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);

protected:
    cSsnxSerializer mSerializer;

};

