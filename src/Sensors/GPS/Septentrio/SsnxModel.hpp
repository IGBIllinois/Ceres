
#pragma once

#include "../GpsModel.hpp"

#include <cbdf/SsnxSerializer.hpp>

#include <QObject>


class cSsnxModel : public cGpsModel
{
    Q_OBJECT

public:
    explicit cSsnxModel(QObject* parent = nullptr);
    explicit cSsnxModel(const std::string& instance, QObject* parent = nullptr);
    ~cSsnxModel();

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Septertino GPS
     */
    uint8_t device_id() const override;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void dataRecordingStateChange(bool record) override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

signals:
    void pvtCartesianDataValid(bool valid);
    void pvtGeodeticDataValid(bool valid);
    void posCovGeodeticDataValid(bool valid);
    void velCovGeodeticDataValid(bool valid);
    void posProjectedDataValid(bool valid);
    void receiverTimeDataValid(bool valid);
    void diffCorrDataValid(bool valid);
    void rtcmDatumDataValid(bool valid);
    void ntripClientDataValid(bool valid);
    void receiverStatusDataValid(bool valid);
    void wifiClientDataValid(bool valid);

    void receiverStatusChanged(int errorCode);
    void ntripClientStatusChanged(int state, int errorCode);
    void wifiClientConnectionChanged(int state, int powerLevel_dBm, int errorCode);
    void solutionTypeChanged(int solution_type);
    void positionChanged(int x_mm, int y_mm, int z_mm);
//    void wifiStateChanged(int state, int powerLevel_dBm);

signals:
    void updateGeodeticPVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, int datum, int num_sv, int num_bases);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);

protected:
    int mNumBases = 0;
    int mNumSV = 0;

    cSsnxSerializer mSerializer;

private:
    const uint8_t mDeviceID;
};

/**
 * Implementation Details
 **/

inline uint8_t cSsnxModel::device_id() const
{
    return mDeviceID;
}

inline uint16_t cSsnxModel::data_class_id() const
{
    return mSerializer.classID();
}

