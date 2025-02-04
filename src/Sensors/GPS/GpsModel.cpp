
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

gps::eReferenceState cGpsModel::getReferenceState() const
{
    return mReferenceState;
}


int cGpsModel::getRefIntegrationTime_sec() const
{
    return mRefIntegrationTime_sec;
}

int cGpsModel::getRefMaxIntegrationTime_sec() const
{
    return mRefMaxIntegrationTime_sec;
}

void cGpsModel::setReferenceIntegrationTimes(int integration_time_sec, int max_integration_time_sec)
{
    mRefIntegrationTime_sec = integration_time_sec;
    mRefMaxIntegrationTime_sec = max_integration_time_sec;
}

gps::sReferencePosition cGpsModel::getReferencePosition() const
{
    return mReferencePosition;
}


