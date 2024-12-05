
#include "GpsModel.hpp"


cGpsModel::cGpsModel(const std::string& name, QObject* parent)
:
    cSensorModel(name, parent)
{}

bool cGpsModel::isPositionValid() const
{
    return mPvtValid;
}

bool cGpsModel::isTimeValid() const
{
    return mTimeValid;
}

::gps::eSolutionType cGpsModel::getSolutionType() const
{
    return mSolutionType;
}

gps::eDatum cGpsModel::getDatum() const
{
    return mDatum;
}

gps::sPosition cGpsModel::getPosition() const
{
    return { mPvtTimestamp_s, mLatitude_rad, mLongitude_rad , mHeight_m };
}

gps::sVelocity cGpsModel::getVelocity() const
{
    return { mPvtTimestamp_s, mVn_mps, mVe_mps , mVu_mps };
}

gps::sUtcDate cGpsModel::getDate() const
{
    return { mRxTimestamp_s, mUtcDay, mUtcMonth , mUtcYear };
}

gps::sUtcTime cGpsModel::getTime() const
{
    return { mRxTimestamp_s, mUtcHour, mUtcMinute , mUtcSecond };
}
