
#include "GpsModelSsnx.hpp"
#include <functional>

#include <QMessageBox>

using namespace ssnx;

cGpsModelSsnx::cGpsModelSsnx(QObject* parent)
:
    cGpsModel(parent)
{
    mConnected = false;
//    connect(&mGpsDataStream, &cGpsStreamSsnx::dataUpdated, this, &cGpsModelSsnx::processData);
//    mGpsDataStream.registerDataProcessingCallback(std::bind(&cGpsModelSsnx::processData, this));
}

cGpsModelSsnx::~cGpsModelSsnx()
{
}

QString cGpsModelSsnx::getViewTitle() const
{
    return "SSNX GPS";
}

void cGpsModelSsnx::configure(const nlohmann::json& jsonCfg)
{
    std::string ip;
    uint16_t port = 0;

    try
    {
        cGpsModel::configure(jsonCfg);

        ip = static_cast<std::string>(jsonCfg["host"]);
        port = jsonCfg["port"];
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"ssnx\" configuration: ";
        str.append(e.what());
        QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
        msg.exec();
        return;
    }


    QString msg("Trying to establishing connection to GPS receiver at ");
    msg.append(ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

    if (!try_to_connect(ip, port, false))
    {
        QMessageBox msg(QMessageBox::Critical, "GPS Error", "Could not establish connection to GPS receiver!");
        msg.exec();
        return;
    }
   
    mConnected = isConnected();

    if (!mConnected)
    {
        QMessageBox msg(QMessageBox::Critical, "GPS Error", "Could not establish connection to GPS receiver!");
        msg.exec();
        return;
    }
}

void cGpsModelSsnx::run()
{
    if (!mConnected) return;
    processOneDatagram();
}

void cGpsModelSsnx::writeDataHeader(cDataFile& file)
{
    // The GPS does not have any 
}

void cGpsModelSsnx::pvtGeodetic(const gps::PVT_Geodetic_2_t pvt)
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
        cSensorModel::mDataBuffer.reset();
        cSensorModel::mDataBuffer << mPvtTimestamp_s;
        cSensorModel::mDataBuffer << mLatitude_rad << mLongitude_rad << mHeight_m;
        cSensorModel::mDataBuffer << mVn_mps << mVe_mps << mVu_mps;
        cSensorModel::mDataBuffer << mGroundTrack_deg;

        std::lock_guard<std::mutex> guard(mFileMutex);
        if (mpFile)
            mpFile->writeBlock(BlockIDs::GPS_PVT_1, cSensorModel::mDataBuffer.data(), cSensorModel::mDataBuffer.size());
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
        mGroundTrack_deg, mDatum);
}

void cGpsModelSsnx::posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov)
{}

void cGpsModelSsnx::velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov)
{}

void cGpsModelSsnx::posProjected(const ssnx::gps::POS_Projected_1_t pvt)
{}

void cGpsModelSsnx::receiverTime(const gps::ReceiverTime_1_t pvt)
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
        cSensorModel::mDataBuffer.reset();
        cSensorModel::mDataBuffer << mPvtTimestamp_s;
        cSensorModel::mDataBuffer << mUtcHour << mUtcMinute << mUtcSecond;
        cSensorModel::mDataBuffer << mUtcYear << mUtcMonth << mUtcDay;

        std::lock_guard<std::mutex> guard(mFileMutex);
        if (mpFile)
            mpFile->writeBlock(BlockIDs::GPS_UTC_1, cSensorModel::mDataBuffer.data(), cSensorModel::mDataBuffer.size());
    }

    emit updateUTC(mUtcHour, mUtcMinute, mUtcSecond, mUtcDay, mUtcMonth, mUtcYear);
}

void cGpsModelSsnx::rtcmDatum(const ssnx::gps::RtcmDatum_1_t rtcm)
{}

void cGpsModelSsnx::processDatagram(const void* pBuffer, std::size_t buf_length)
{
    decode(pBuffer, buf_length);
}
