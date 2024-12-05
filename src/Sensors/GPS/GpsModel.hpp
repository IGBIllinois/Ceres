
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

    ::gps::eSolutionType getSolutionType() const;
    ::gps::eDatum        getDatum() const;
    ::gps::sPosition     getPosition() const;
    ::gps::sVelocity     getVelocity() const;
    ::gps::sUtcDate      getDate() const;
    ::gps::sUtcTime      getTime() const;

protected:
    cGpsModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cGpsModel() = default;

    ::gps::eSolutionType mSolutionType = ::gps::eSolutionType::NONE;

    ::gps::eDatum mDatum = ::gps::eDatum::WGS84;

    bool    mPvtValid = false;
    double  mPvtTimestamp_s = 0.0;
    double  mLatitude_rad = 0.0;
    double  mLongitude_rad = 0.0;
    double  mHeight_m = 0.0;
    double  mUndulation_m = 0.0;
    double  mVn_mps = 0.0;
    double  mVe_mps = 0.0;
    double  mVu_mps = 0.0;
    double  mGroundTrack_deg = 0.0;

    bool    mTimeValid = false;
    double  mRxTimestamp_s = 0.0;

    int mUtcHour = 0;
    int mUtcMinute = 0;
    int mUtcSecond = 0;

    int mUtcDay = 0;
    int mUtcMonth = 0;
    int mUtcYear = 0;

    bool mRxTimeLocked = false;

    bool mRecordTrack = false;
    std::deque<::gps::sGpsPoint> mTrack;
};

