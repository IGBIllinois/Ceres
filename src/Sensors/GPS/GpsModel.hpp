
#pragma once

#include "../SensorModel.hpp"
#include "GpsTypes.hpp"

#include "Timers.hpp"

#include <QMetaType>

#include <deque>
#include <vector>


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

    int getRefMinIntegrationTime_sec() const;
    int getRefMaxIntegrationTime_sec() const;
    int getRefErrorThreshold_mm() const;

    void setReferenceIntegrationTimes(int min_integration_time_sec,
        int max_integration_time_sec, int ref_error_threshold_mm);

    ::gps::sReferencePosition getReferencePosition() const;

    bool configure(const nlohmann::json& jsonCfg) override;

    void writeDataHeader() override;

signals:
    void referenceComplete();
    void referenceStateChanged(::gps::eReferenceState state);
    void referenceParametersChanged(int min_integration_time_sec, int max_integration_time_sec, int ref_error_threshold_mm);
    void referenceDataChanged(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
        double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid);

    void referencePositionChanged(int x_mm, int y_mm, int z_mm, double error_mm, int count);

public slots:
    void referenceStateQueried();
    void referenceParametersQueried();
    void referenceDataQueried();

    void updateReferenceParameters(int min_integration_time_sec, int max_integration_time_sec, int error_threshold_mm);

    void startReferenceComputation();
    void abortReferenceCompute();

protected:
    cGpsModel(const std::string& name, QObject* parent = nullptr);
    cGpsModel(const std::string& name, const std::string& instance, QObject* parent = nullptr);
    virtual ~cGpsModel();

protected:
    void calcReferencePosition();

protected:
    ::gps::eSolutionType mSolutionType = ::gps::eSolutionType::NONE;

    ::gps::eDatum mDatum = ::gps::eDatum::WGS84;

    bool    mPvtValid = false;
    double  mPvtTimestamp_s = 0.0;
    double  mLatitude_rad = 0.0;
    double  mLongitude_rad = 0.0;
    double  mHeight_m = 0.0;
    double  mUndulation_m = 0.0;
    bool    mHeightComputed = false;
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

    struct sAntennaOffset_t
    {
        int32_t  x_mm = 0;
        int32_t  y_mm = 0;
        int32_t  z_mm = 0;
    }
    mAntennaOffset;

    bool mRxTimeLocked = false;

    int mRefMinIntegrationTime_sec = 3;
    int mRefMaxIntegrationTime_sec = 10;
    int mRefErrorThreshold_mm = 100;

    cOneShotTimer mRefMinIntegrationTimer;
    cOneShotTimer mRefMaxIntegrationTimer;

    std::vector<double>   mRefLatitudes;
    std::vector<double>   mRefLongitudes;
    std::vector<double>   mRefHeights;

    ::gps::eReferenceState    mReferenceState = ::gps::eReferenceState::WAITING;
    ::gps::sReferencePosition mReferencePosition;

    bool mRecordTrack = false;
    std::deque<::gps::sGpsPoint> mTrack;
};

