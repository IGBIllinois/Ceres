
#include "GpsModel.hpp"

#include "RappFieldBoundary.hpp"

#include <numeric>


cGpsModel::cGpsModel(const std::string& name, QObject* parent)
    :
    cSensorModel(name, parent)
{
    mRefMinIntegrationTimer.time_sec(mRefMinIntegrationTime_sec);
    mRefMaxIntegrationTimer.time_sec(mRefMaxIntegrationTime_sec);
}

cGpsModel::cGpsModel(const std::string& name, const std::string& instance, QObject* parent)
    :
    cSensorModel(name, instance, parent)
{
    mRefMinIntegrationTimer.time_sec(mRefMinIntegrationTime_sec);
    mRefMaxIntegrationTimer.time_sec(mRefMaxIntegrationTime_sec);
}

cGpsModel::~cGpsModel()
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


int cGpsModel::getRefMinIntegrationTime_sec() const
{
    return mRefMinIntegrationTime_sec;
}

int cGpsModel::getRefMaxIntegrationTime_sec() const
{
    return mRefMaxIntegrationTime_sec;
}

int cGpsModel::getRefErrorThreshold_mm() const
{
    return mRefErrorThreshold_mm;
}

void cGpsModel::setReferenceIntegrationTimes(int min_integration_time_sec, 
    int max_integration_time_sec, int ref_error_threshold_mm)
{
    if (min_integration_time_sec > max_integration_time_sec)
        std::swap(min_integration_time_sec, max_integration_time_sec);

    mRefMinIntegrationTime_sec = min_integration_time_sec;
    mRefMaxIntegrationTime_sec = max_integration_time_sec;
    mRefErrorThreshold_mm = ref_error_threshold_mm;

    mRefMinIntegrationTimer.time_sec(mRefMinIntegrationTime_sec);
    mRefMaxIntegrationTimer.time_sec(mRefMaxIntegrationTime_sec);

    emit referenceParametersChanged(mRefMinIntegrationTime_sec, mRefMaxIntegrationTime_sec, mRefErrorThreshold_mm);
}

gps::sReferencePosition cGpsModel::getReferencePosition() const
{
    return mReferencePosition;
}

void cGpsModel::writeDataHeader()
{
    // Write Data Header is call at the beginning of each measurement run.
    // Here we invalidate the reference position so we don't store an old copy.
    mRefLatitudes.clear();
    mRefLongitudes.clear();
    mRefHeights.clear();

    mRefMinIntegrationTimer.stop();
    mRefMaxIntegrationTimer.stop();

    mReferenceState = gps::eReferenceState::WAITING;

    mReferencePosition.valid = false;
    mReferencePosition.heightValid = false;
}


/******************************************************************************
* S L O T S   H A N D L E R S
*******************************************************************************/

void cGpsModel::referenceStateQueried()
{
    emit referenceStateChanged(mReferenceState);
}

void cGpsModel::referenceParametersQueried()
{
    emit referenceParametersChanged(mRefMinIntegrationTime_sec, mRefMaxIntegrationTime_sec, mRefErrorThreshold_mm);
}

void cGpsModel::referenceDataQueried()
{
    emit referenceDataChanged(mReferencePosition.valid, mReferencePosition.avgLatitude_rad, mReferencePosition.avgLongitude_rad, mReferencePosition.avgHeight_m,
        mReferencePosition.stdLatitude_rad, mReferencePosition.stdLongitude_rad, mReferencePosition.stdHeight_m, mReferencePosition.heightValid);
}

void  cGpsModel::updateReferenceParameters(int min_integration_time_sec, int max_integration_time_sec, int error_threshold_mm)
{
    setReferenceIntegrationTimes(min_integration_time_sec, max_integration_time_sec, error_threshold_mm);
}

void cGpsModel::startReferenceComputation()
{
    logMessage(logSTATUS, "Reference computation start requested.");

    if (getStatus() != sensor::eStatus::RUNNING)
    {
        emit referenceStateChanged(gps::eReferenceState::ABORT);
        return;
    }

    if ((mReferenceState == gps::eReferenceState::START)
        || (mReferenceState == gps::eReferenceState::PENDING))
        return;

    mReferenceState = gps::eReferenceState::START;

    emit referenceStateChanged(mReferenceState);
}

void cGpsModel::abortReferenceCompute()
{
    logMessage(logSTATUS, "Reference computation abort requested.");
    
    mReferenceState = gps::eReferenceState::ABORT;

    emit referenceStateChanged(mReferenceState);
}


/******************************************************************************
* HELPER METHODS
*******************************************************************************/

void cGpsModel::calcReferencePosition()
{
    if (mReferenceState == gps::eReferenceState::START)
    {
        mReferenceState = gps::eReferenceState::PENDING;
        mRefMinIntegrationTimer.start();
        mRefMaxIntegrationTimer.start();

        logMessage(logSTATUS, "Reference computation started...");
    }

    if (mReferenceState == gps::eReferenceState::ABORT)
    {
        mRefLatitudes.clear();
        mRefLongitudes.clear();
        mRefHeights.clear();

        mRefMinIntegrationTimer.stop();
        mRefMaxIntegrationTimer.stop();

        mReferenceState = gps::eReferenceState::WAITING;

        emit referenceComplete();

        emit referencePositionChanged(-1, -1, -1, -1.0, -1);

        logMessage(logSTATUS, "Reference computation aborted!");
        return;
    }

    if (mReferenceState != gps::eReferenceState::PENDING)
        return;

    if (mRefMaxIntegrationTimer.elapsed())
    {
        int count = static_cast<int>(mRefLatitudes.size());

        if (mRefLatitudes.empty() || mRefLongitudes.empty())
        {
            mReferenceState = gps::eReferenceState::COMPLETE_FAILED;

            mReferencePosition.valid = false;

            emit referenceComplete();
            emit referencePositionChanged(-1, -1, -1, -1.0, count);
            logMessage(logSTATUS, "Reference computation failed!");

            return;
        }

        double avgLat_rad = std::accumulate(mRefLatitudes.begin(), mRefLatitudes.end(), 0.0) / mRefLatitudes.size();
        double avgLng_rad = std::accumulate(mRefLongitudes.begin(), mRefLongitudes.end(), 0.0) / mRefLongitudes.size();

        double varLat_rad = std::accumulate(mRefLatitudes.begin(), mRefLatitudes.end(), 0.0, 
            [avgLat_rad](double i, double a) { return i + ((a - avgLat_rad) * (a - avgLat_rad)); } ) / mRefLatitudes.size();

        double stdLat_rad = sqrt(varLat_rad);

        double varLng_rad = std::accumulate(mRefLongitudes.begin(), mRefLongitudes.end(), 0.0, 
            [avgLng_rad](double i, double a) { return i + ((a - avgLng_rad) * (a - avgLng_rad)); } ) / mRefLongitudes.size();

        double stdLng_rad = sqrt(varLng_rad);

        mReferencePosition.valid = true;
        mReferencePosition.avgLatitude_rad = avgLat_rad;
        mReferencePosition.avgLongitude_rad = avgLng_rad;
        mReferencePosition.stdLatitude_rad = stdLat_rad;
        mReferencePosition.stdLongitude_rad = stdLng_rad;

        double avgHeight_m = 0;
        double stdHeight_m = 0;

        if (mRefHeights.size() == 0)
        {
            mReferencePosition.avgHeight_m = 0;
            mReferencePosition.stdHeight_m = 0;
            mReferencePosition.heightValid = false;
        }
        else
        {
            avgHeight_m = std::accumulate(mRefHeights.begin(), mRefHeights.end(), 0.0) / mRefHeights.size();

            double varHeight_m = std::accumulate(mRefHeights.begin(), mRefHeights.end(), 0.0, 
                [avgHeight_m](double i, auto a) { return i + ((a - avgHeight_m) * (a - avgHeight_m)); }) / mRefHeights.size();

            stdHeight_m = sqrt(varHeight_m);

            mReferencePosition.avgHeight_m = avgHeight_m;
            mReferencePosition.stdHeight_m = stdHeight_m;
            mReferencePosition.heightValid = true;
        }

        mRefLatitudes.clear();
        mRefLongitudes.clear();
        mRefHeights.clear();

        emit referenceComplete();

        auto p = rfb::fromGPS(avgLat_rad, avgLng_rad, avgHeight_m);

        auto p1 = rfb::fromGPS(avgLat_rad - stdLat_rad, avgLng_rad - stdLng_rad, avgHeight_m - stdHeight_m);
        auto p2 = rfb::fromGPS(avgLat_rad + stdLat_rad, avgLng_rad + stdLng_rad, avgHeight_m + stdHeight_m);

        double dx = p2.x_mm - p1.x_mm;
        double dy = p2.y_mm - p1.y_mm;
        double dz = p2.z_mm - p1.z_mm;

        double error = sqrt(dx * dx + dy * dy + dz * dz);

        emit referencePositionChanged(p.x_mm, p.y_mm, p.z_mm, error, count);

        logMessage(logSTATUS, "Reference computation hit max integration time!");

        mReferenceState = gps::eReferenceState::COMPLETE_GOOD;

        return;
    }

    mRefLatitudes.push_back(mLatitude_rad);
    mRefLongitudes.push_back(mLongitude_rad);

    double height_m = 0.0;
    if (mHeightComputed)
    {
        height_m = mHeight_m - mUndulation_m;
        mRefHeights.push_back(height_m);
    }

    if (!mRefMinIntegrationTimer.elapsed())
        return;

    double avgLat_rad = std::accumulate(mRefLatitudes.begin(), mRefLatitudes.end(), 0.0) / mRefLatitudes.size();
    double avgLng_rad = std::accumulate(mRefLongitudes.begin(), mRefLongitudes.end(), 0.0) / mRefLongitudes.size();

    double varLat_rad = std::accumulate(mRefLatitudes.begin(), mRefLatitudes.end(), 0.0, 
        [avgLat_rad](double i, auto a) { return i + ((a - avgLat_rad) * (a - avgLat_rad)); } ) / mRefLatitudes.size();

    double stdLat_rad = sqrt(varLat_rad);

    double varLng_rad = std::accumulate(mRefLongitudes.begin(), mRefLongitudes.end(), 0.0, 
        [avgLng_rad](double i, auto a) { return i + ((a - avgLng_rad) * (a - avgLng_rad)); } ) / mRefLongitudes.size();

    double stdLng_rad = sqrt(varLng_rad);

    double avgHeight_m = 0.0;
    double stdHeight_m = 0.0;

    if (mHeightComputed)
    {
        avgHeight_m = std::accumulate(mRefHeights.begin(), mRefHeights.end(), 0.0) / mRefHeights.size();

        double varHeight_m = std::accumulate(mRefHeights.begin(), mRefHeights.end(), 0.0,
            [avgHeight_m](double i, auto a) { return i + ((a - avgHeight_m) * (a - avgHeight_m)); }) / mRefHeights.size();

        stdHeight_m = sqrt(varHeight_m);
    }

    auto p1 = rfb::fromGPS(avgLat_rad - stdLat_rad, avgLng_rad - stdLng_rad, avgHeight_m - stdHeight_m);
    auto p2 = rfb::fromGPS(avgLat_rad + stdLat_rad, avgLng_rad + stdLng_rad, avgHeight_m + stdHeight_m);

    double dx = p2.x_mm - p1.x_mm;
    double dy = p2.y_mm - p1.y_mm;
    double dz = p2.z_mm - p1.z_mm;

    double error = sqrt(dx*dx + dy*dy + dz*dz);

    if (error <= mRefErrorThreshold_mm)
    {
        int count = static_cast<int>(mRefLatitudes.size());

        mReferencePosition.valid = true;
        mReferencePosition.avgLatitude_rad = avgLat_rad;
        mReferencePosition.avgLongitude_rad = avgLng_rad;
        mReferencePosition.stdLatitude_rad = stdLat_rad;
        mReferencePosition.stdLongitude_rad = stdLng_rad;

        if (mHeightComputed)
        {
            mReferencePosition.avgHeight_m = avgHeight_m;
            mReferencePosition.stdHeight_m = stdHeight_m;
            mReferencePosition.heightValid = true;
        }
        else
        {
            mReferencePosition.avgHeight_m = 0;
            mReferencePosition.stdHeight_m = 0;
            mReferencePosition.heightValid = false;
        }

        mRefLatitudes.clear();
        mRefLongitudes.clear();
        mRefHeights.clear();

        mReferenceState = gps::eReferenceState::COMPLETE_GOOD;

        emit referenceComplete();

        auto p = rfb::fromGPS(avgLat_rad, avgLng_rad, avgHeight_m);

        emit referencePositionChanged(p.x_mm, p.y_mm, p.z_mm, error, count);
        logMessage(logSTATUS, "Reference computation complete");
    }
}

