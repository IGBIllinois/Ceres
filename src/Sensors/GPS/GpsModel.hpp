
#pragma once

#include "../SensorModel.hpp"
#include "GpsTypes.hpp"

#include <deque>


class cGpsModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "gps"; };

    bool isPositionValid() const;
    bool isTimeValid() const;

    ::gps::eDatum    getDatum() const;
    ::gps::sPosition getPosition() const;
    ::gps::sVelocity getVelocity() const;
    ::gps::sUtcDate  getDate() const;
    ::gps::sUtcTime  getTime() const;

protected:
    cGpsModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cGpsModel() = default;

    ::gps::eDatum mDatum;

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
    std::deque<::gps::sGpsPoint> mTrack;
};

