
#include "SsnxModel_net.hpp"

#include "../GpsUtils.hpp"

#include <functional>

using namespace ssnx;

cSsnxModel_net::cSsnxModel_net(QObject* parent)
:
    cSsnxModel(parent)
{
    mConnected = false;
}

cSsnxModel_net::~cSsnxModel_net()
{
}

void cSsnxModel_net::updateViews()
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
        emit logMessage(logERROR, q_name(), str);
        return false;
    }

    QString msg("Trying to establishing connection to GPS receiver at ");
    msg.append(ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

    if (!try_to_connect(ip, port, false))
    {
        emit logMessage(logERROR, q_name(), "Could not establish connection to GPS receiver!");
        return false;
    }

    return cSsnxModel::configure(jsonCfg);
}

bool cSsnxModel_net::startCommunications()
{
    if (!cSsnxGpsStream::startCommunications())
    {
        emit logMessage(logERROR, q_name(), "Could not establish connection to GPS receiver!");
        return false;
    }

    mConnected = isConnected();

    if (!mConnected)
    {
        emit logMessage(logERROR, q_name(), "Could not establish connection to GPS receiver!");
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

void cSsnxModel_net::writeDataHeader()
{
/*
    if (((mReferenceState == ::gps::eReferenceState::COMPLETE_GOOD)
        || (mReferenceState == ::gps::eReferenceState::COMPLETE_FAILED))
        && mReferencePosition.valid)
    {
        mSerializer.writeReferencePoint(mReferencePosition.avgLatitude_rad, 
            mReferencePosition.avgLongitude_rad, mReferencePosition.avgHeight_m,
            mReferencePosition.stdLatitude_rad, mReferencePosition.stdLongitude_rad, mReferencePosition.stdHeight_m,
            mReferencePosition.heightValid);

        mReferencePosition.valid = false;
        mReferenceState = ::gps::eReferenceState::WAITING;
    }
*/
}

void cSsnxModel_net::pvtCartesian(const ssnx::gps::PVT_Cartesian_2_t pvt)
{
    if (!pvt.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
    }
}

void cSsnxModel_net::pvtGeodetic(const ssnx::gps::PVT_Geodetic_2_t pvt)
{
    mPvtValid = pvt.dataValid;
    mPvtTimestamp_s = pvt.timestamp_s;

    mSolutionType = static_cast<::gps::eSolutionType>(pvt.Mode);

    emit solutionTypeChanged(::gps::to_int(mSolutionType));

    if (!mPvtValid) return;

    mDatum = static_cast<::gps::eDatum>(pvt.Datum);

    mLatitude_rad = pvt.Lat_rad;
    mLongitude_rad = pvt.Lon_rad;
    mHeight_m = pvt.Height_m;;
    mUndulation_m = pvt.Undulation_m;
    mVn_mps = pvt.Vn_mps;
    mVe_mps = pvt.Ve_mps;
    mVu_mps = pvt.Vu_mps;
    mGroundTrack_deg = pvt.GroundTrack_deg;
    mHeightComputed = pvt.HeightComputed;

    mNumBases = pvt.NrBases.value_or(0);
    mNumSV = pvt.NrSV;

    double height_m = mHeight_m - mUndulation_m;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
    }
    else
        calcReferencePosition();

    if (mRecordTrack)
    {
        ::gps::sGpsPoint point = { mPvtTimestamp_s,
            mLatitude_rad, mLongitude_rad, height_m,
            mVn_mps, mVe_mps, mVu_mps,
            mGroundTrack_deg };

        mTrack.emplace_back(point);
    }

    emit updateGeodeticPVT(mPvtTimestamp_s,
        mLatitude_rad, mLongitude_rad, height_m,
        mVn_mps, mVe_mps, mVu_mps,
        mGroundTrack_deg, ::gps::to_int(mDatum), mNumSV, mNumBases);
}

void cSsnxModel_net::posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov)
{
    if (!cov.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(cov);
    }
}

void cSsnxModel_net::velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov)
{
    if (!cov.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(cov);
    }
}

void cSsnxModel_net::posLocal(const ssnx::gps::POS_Local_1_t pos)
{
    if (!pos.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pos);
    }
}

void cSsnxModel_net::posProjected(const ssnx::gps::POS_Projected_1_t pos)
{
    if (!pos.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pos);
    }
}

void cSsnxModel_net::receiverTime(const ssnx::gps::ReceiverTime_1_t pvt)
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
{
    if (!rtcm.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(rtcm);
    }
}

void cSsnxModel_net::processDatagram(const void* pBuffer, std::size_t buf_length)
{
    decode(pBuffer, buf_length);
}
