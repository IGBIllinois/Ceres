
#pragma once

#include "SensorModel.hpp"

#include <deque>


class cGpsModel : public cSensorModel
{
public:

    enum class eDatum
    {
        WGS84,
        DGNSS,
        ETRS89,
        NAD83,
        NAD83_PA11,
        NAD83_MA11,
        GDA94,
        FIRST_USER_DATUM,
        SECOND_USER_DATUM
    };

    struct sGpsPoint
    {
        double  Timestamp_s;
        double  Latitude_rad;
        double  Longitude_rad;
        double  Height_m;
        double  Vn_mps;
        double  Ve_mps;
        double  Vu_mps;
        double  GroundTrack_deg;
    };

    struct sPosition
    {
        double  Timestamp_s;
        double  Latitude_rad;
        double  Longitude_rad;
        double  Height_m;
    };

    struct sVelocity
    {
        double  Timestamp_s;
        double  Vn_mps;
        double  Ve_mps;
        double  Vu_mps;
    };

    struct sUtcDate
    {
        double  Timestamp_s;
        int  utcDay;
        int  utcMonth;
        int  utcYear;
    };

    struct sUtcTime
    {
        double  Timestamp_s;
        int  utcHour;
        int  utcMinute;
        int  utcSecond;
    };

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "gps"; };

    bool isPositionValid() const;
    bool isTimeValid() const;

    eDatum    getDatum() const;
    sPosition getPosition() const;
    sVelocity getVelocity() const;
    sUtcDate  getDate() const;
    sUtcTime  getTime() const;

protected:
    cGpsModel(QObject* parent = nullptr);
    virtual ~cGpsModel() = default;

    eDatum mDatum;

    bool    mPvtValid;
    double  mPvtTimestamp_s;
    double  mLatitude_rad;
    double  mLongitude_rad;
    double  mHeight_m;
    double  mUndulation_m;
    double  mVn_mps;
    double  mVe_mps;
    double  mVu_mps;
    double  mGroundTrack_deg;

    bool    mTimeValid;
    double  mRxTimestamp_s;

    int mUtcHour;
    int mUtcMinute;
    int mUtcSecond;

    int mUtcDay;
    int mUtcMonth;
    int mUtcYear;

    bool mRxTimeLocked;

    bool mRecordTrack;
    std::deque<sGpsPoint> mTrack;
};

