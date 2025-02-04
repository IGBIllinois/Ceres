
#pragma once

#include "../SensorModel.hpp"
#include "GpsTypes.hpp"

#include <deque>


class cGpsModel : public cSensorModel
{
    Q_OBJECT

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

    ::gps::eReferenceState getReferenceState() const;

    int getRefIntegrationTime_sec() const;
    int getRefMaxIntegrationTime_sec() const;

    void setReferenceIntegrationTimes(int integration_time_sec, int max_integration_time_sec);

    ::gps::sReferencePosition getReferencePosition() const;

signals:
    void referenceComplete();

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

    int mRefIntegrationTime_sec = 0;
    int mRefMaxIntegrationTime_sec = 0;
    ::gps::eReferenceState    mReferenceState;
    ::gps::sReferencePosition mReferencePosition;

    bool mRecordTrack = false;
    std::deque<::gps::sGpsPoint> mTrack;
};

