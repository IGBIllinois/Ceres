
#include "GpsModelSsnx.hpp"


cGpsModelSsnx::cGpsModelSsnx(QObject* parent)
:
    cGpsModel(parent)
{
}

cGpsModelSsnx::~cGpsModelSsnx()
{
}


void cGpsModelSsnx::configure(nlohmann::json& jsonCfg)
{
    std::string ip = static_cast<std::string>(jsonCfg["host"]);
    uint16_t port = jsonCfg["port"];
    mGpsDataStream.try_to_connect(ip, port, false);
}

void cGpsModelSsnx::writeDataHeader(cDataFile& file)
{
    // The GPS does not have any 
}

void cGpsModelSsnx::pvtGeodetic(const gps::PVT_Geodetic_2_0_t pvt)
{
    mPvtValid = pvt.dataValid;
    mPvtTimestamp_s = pvt.timestamp_s;

    if (!mPvtValid) return;

    mDatum = static_cast<cGpsModel::eDatum>(pvt.Datum);

    mLatitude_rad = pvt.Lat_rad;
    mLongitude_rad = pvt.Lon_rad;
    mHeight_m = pvt.Height_m;;
    mUndulation_m = pvt.Undulation_m;
    mVn_mps = pvt.Vn_mps;
    mVe_mps = pvt.Ve_mps;
    mVu_mps = pvt.Vu_mps;
    mGroundTrack_deg = pvt.GroundTrack_deg;

    if (isRecording())
    {
        mDataBuffer.reset();
        mDataBuffer << mPvtTimestamp_s;
        mDataBuffer << mLatitude_rad << mLongitude_rad << mHeight_m;
        mDataBuffer << mVn_mps << mVe_mps << mVu_mps;
        mDataBuffer << mGroundTrack_deg;

        std::lock_guard<std::mutex> guard(mFileMutex);
        if (mpFile)
            mpFile->writeBlock(BlockIDs::GPS_PVT_1, mDataBuffer.data(), mDataBuffer.size());
    }

    if (mRecordTrack)
    {
        sGpsPoint point = { mPvtTimestamp_s,
            mLatitude_rad, mLongitude_rad, mHeight_m,
            mVn_mps, mVe_mps, mVu_mps,
            mGroundTrack_deg };

        mTrack.emplace_back(point);
    }

    emit updatePVT(mPvtTimestamp_s,
        mLatitude_rad, mLongitude_rad, mHeight_m,
        mVn_mps, mVe_mps, mVu_mps,
        mGroundTrack_deg);
}

void cGpsModelSsnx::pvtGeodetic(const gps::PVT_Geodetic_2_1_t pvt)
{
    pvtGeodetic(static_cast<const gps::PVT_Geodetic_2_0_t>(pvt));
}

void cGpsModelSsnx::pvtGeodetic(const gps::PVT_Geodetic_2_2_t pvt)
{
    pvtGeodetic(static_cast<const gps::PVT_Geodetic_2_0_t>(pvt));
}

void cGpsModelSsnx::receiverTime(const gps::ReceiverTime_t pvt)
{
    mTimeValid = pvt.dataValid;
    mRxTimestamp_s = pvt.timestamp_s;

    if (!mTimeValid) return;

    mUtcHour = pvt.utcHour;
    mUtcMinute = pvt.utcMinute;
    mUtcSecond = pvt.utcSecond;
    mUtcDay = pvt.utcDay;
    mUtcMonth = pvt.utcMonth;
    mUtcYear = pvt.utcYear;
    mRxTimeLocked = pvt.TimeOfWeekWithin20ms || pvt.TimeOfWeekWithinThreshold;

    if (isRecording())
    {
        mDataBuffer.reset();
        mDataBuffer << mPvtTimestamp_s;
        mDataBuffer << mUtcHour << mUtcMinute << mUtcSecond;
        mDataBuffer << mUtcYear << mUtcMonth << mUtcDay;

        std::lock_guard<std::mutex> guard(mFileMutex);
        if (mpFile)
            mpFile->writeBlock(BlockIDs::GPS_UTC_1, mDataBuffer.data(), mDataBuffer.size());
    }

    emit updateUTC(mUtcHour, mUtcMinute, mUtcSecond, mUtcDay, mUtcMonth, mUtcYear);
}

void cGpsModelSsnx::processData(const void* pBuffer, std::size_t buf_length)
{
    decode(pBuffer, buf_length);
}
