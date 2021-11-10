
#include "GpsModel.hpp"


cGpsModel::cGpsModel()
{
    mDatum = eDatum::WGS84;

    mPvtValid = false;
    mPvtTimestamp_s = 0.0;
    mLatitude_rad = 0.0;
    mLongitude_rad = 0.0;
    mHeight_m = 0.0;
    mUndulation_m = 0.0;
    mVn_mps = 0.0;
    mVe_mps = 0.0;
    mVu_mps = 0.0;
    mGroundTrack_deg = 0.0;

    mTimeValid = false;
    mRxTimestamp_s = 0.0;

    mUtcHour = 0;
    mUtcMinute = 0;
    mUtcSecond = 0;

    mUtcDay = 0;
    mUtcMonth = 0;
    mUtcYear = 0;

    mRxTimeLocked = false;

    mRecordTrack = false;
}

bool cGpsModel::isPositionValid() const
{
    return mPvtValid;
}

bool cGpsModel::isTimeValid() const
{
    return mTimeValid;
}

cGpsModel::eDatum cGpsModel::getDatum() const
{
    return mDatum;
}

cGpsModel::sPosition cGpsModel::getPosition() const
{
    return { mPvtTimestamp_s, mLatitude_rad, mLongitude_rad , mHeight_m };
}

cGpsModel::sVelocity cGpsModel::getVelocity() const
{
    return { mPvtTimestamp_s, mVn_mps, mVe_mps , mVu_mps };
}

cGpsModel::sUtcDate cGpsModel::getDate() const
{
    return { mRxTimestamp_s, mUtcDay, mUtcMonth , mUtcYear };
}

cGpsModel::sUtcTime cGpsModel::getTime() const
{
    return { mRxTimestamp_s, mUtcHour, mUtcMinute , mUtcSecond };
}
