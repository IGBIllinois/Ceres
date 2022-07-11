
#include "SsnxModel_net.hpp"
#include <functional>

using namespace ssnx;

cSsnxModel_net::cSsnxModel_net(QObject* parent)
:
    cSsnxModel(parent),
    mSerializer(4096)
{
    mConnected = false;
}

cSsnxModel_net::~cSsnxModel_net()
{
}

bool cSsnxModel_net::configure(const nlohmann::json& jsonCfg)
{
    std::string ip;
    uint16_t port = 0;

    try
    {
        cSensorModel::configure(jsonCfg);

        ip = static_cast<std::string>(jsonCfg["host"]);
        port = jsonCfg["port"];
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"ssnx\" configuration: ";
        str.append(e.what());
        emit errorMessage("Configuration Error", str);
        return false;
    }

    QString msg("Trying to establishing connection to GPS receiver at ");
    msg.append(ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

    if (!try_to_connect(ip, port, false))
    {
        emit errorMessage("GPS Error", "Could not establish connection to GPS receiver!");
        return false;
    }

    return true;
}

bool cSsnxModel_net::startCommunications()
{
    if (!cSsnxGpsStream::startCommunications())
    {
        emit errorMessage("GPS Error", "Could not establish connection to GPS receiver!");
        return false;
    }

    mConnected = isConnected();

    if (!mConnected)
    {
        emit errorMessage("GPS Error", "Could not establish connection to GPS receiver!");
        return false;
    }

    return true;
}

void cSsnxModel_net::stopCommunications()
{
    cSsnxGpsStream::stopCommunications();
}

void cSsnxModel_net::update()
{
    if (!mConnected) return;
    processOneDatagram();
}

void cSsnxModel_net::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cSsnxModel_net::disableDataRecording()
{
    cGpsModel::disableDataRecording();
    mSerializer.detach();
}

void cSsnxModel_net::writeDataHeader()
{
}

void cSsnxModel_net::pvtGeodetic(const gps::PVT_Geodetic_2_t pvt)
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

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
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

void cSsnxModel_net::posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov)
{}

void cSsnxModel_net::velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov)
{}

void cSsnxModel_net::posProjected(const ssnx::gps::POS_Projected_1_t pvt)
{}

void cSsnxModel_net::receiverTime(const gps::ReceiverTime_1_t pvt)
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

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
    }

    emit updateUTC(mUtcHour, mUtcMinute, mUtcSecond, mUtcDay, mUtcMonth, mUtcYear);
}

void cSsnxModel_net::rtcmDatum(const ssnx::gps::RtcmDatum_1_t rtcm)
{}

void cSsnxModel_net::processDatagram(const void* pBuffer, std::size_t buf_length)
{
    decode(pBuffer, buf_length);
}
