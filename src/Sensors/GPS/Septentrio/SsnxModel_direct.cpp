
#include "SsnxModel_direct.hpp"
#include "RappFieldBoundary.hpp"

#include "../GpsUtils.hpp"

#include <QtSerialPort/QSerialPortInfo>

#include <functional>

#define USE_LOG_MESSAGE


using namespace ssnx;

cSsnxModel_direct::cSsnxModel_direct(QObject* parent)
:
    cSsnxModel(parent),
    mSerialPort(this),
    mSerialBuffer(1024, '\0')
{
}

cSsnxModel_direct::~cSsnxModel_direct()
{
}

void cSsnxModel_direct::updateViews()
{
    emit sensorStatusChanging(q_name(), getStatus());

    emit pvtCartesianStateChanged(mPvtCartesianValid);
    emit pvtGeodeticStateChanged(mPvtGeodeticValid);
    emit posCovGeodeticStateChanged(mPosCovGeodeticValid);
    emit velCovGeodeticStateChanged(mVelCovGeodeticValid);
    emit posProjectedStateChanged(mPosProjectedValid);
    emit receiverTimeStateChanged(mReceiverTimeValid);
    emit rtcmDatumStateChanged(mRtcmDatumValid);
}

bool cSsnxModel_direct::configure(const nlohmann::json& jsonCfg)
{
    emit statusMessage("Connecting to SSNX GPS receiver...");

    try
    {
        auto serial_port = jsonCfg["serial_port"];

#if defined(WIN32)
        std::string portname = serial_port["windows"];
#elif defined(__APPLE_CC__)
        std::string portname = serial_port["macOS"];
#else
        std::string portname = serial_port["linux"];
#endif

        mSerialPort.setPort(QSerialPortInfo(QString::fromStdString(portname)));

        mSerialPort.setBaudRate(QSerialPort::BaudRate::Baud115200);
        mSerialPort.setFlowControl(QSerialPort::FlowControl::NoFlowControl);
        mSerialPort.setParity(QSerialPort::Parity::NoParity);
        mSerialPort.setStopBits(QSerialPort::StopBits::OneStop);
        mSerialPort.setDataBits(QSerialPort::DataBits::Data8);

        auto init_cmds = jsonCfg["initialization"];

        for (auto& cmd : init_cmds)
        {
            sendAsciiCommand(cmd.get<std::string>());
        }
    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"ssnx\" configuration: ";
        msg.append(e.what());
        emit logMessage(logERROR, q_name(), msg);
        return false;
    }

    return cSsnxModel::configure(jsonCfg);
}

bool cSsnxModel_direct::startCommunications()
{
    emit statusMessage("Trying to establishing GPS connection...");

    if (!mSerialPort.open(QIODevice::ReadWrite))
    {
        emit logMessage(logERROR, q_name(), "Could not establish connection to GPS receiver!");
        return false;
    }

    forcePromptRequest();
//    sendPromptRequest();

    setStatus(sensor::eStatus::CONNECTING);

    return true;
}

void cSsnxModel_direct::stopCommunications()
{
    closeConnection();
}

void cSsnxModel_direct::update()
{
    if (!isConnected()) return;
    runOnce();
}

void cSsnxModel_direct::writeDataHeader()
{
}

void cSsnxModel_direct::closeConnection()
{
    mSerialPort.close();
}

bool cSsnxModel_direct::isConnected()
{
    if (!mSerialPort.isOpen())
    {
#ifdef USE_LOG_MESSAGE
        emit logMessage(logSTATUS, q_name(), "Serial port is not open.");
#else
        emit statusMessage("Serial port is not open.");
#endif
    }

    return true;
}

void cSsnxModel_direct::communicationError(const std::string& errorString)
{
    QString msg = QString::fromStdString(errorString);
    emit logMessage(logERROR, q_name(), msg);
}

void cSsnxModel_direct::newConnectionDescriptor(const std::string& connectionDescriptor)
{
    QString msg = "newConnectionDescriptor: ";
    msg += QString::fromStdString(connectionDescriptor);

#ifdef USE_LOG_MESSAGE
    emit logMessage(logSTATUS, q_name(), msg);
#else
    emit statusMessage(msg);
#endif

    setStatus(sensor::eStatus::CONNECTED);
}

void cSsnxModel_direct::newCommandReply(const std::string& reply, bool error)
{
}

void cSsnxModel_direct::newFormattedInformationBlock(const std::string& contents, int index, int count)
{
}

void cSsnxModel_direct::newAsciiDisplay(const std::string& asciiDisplay)
{
    QString msg = "newAsciiDisplay: ";
    msg += QString::fromStdString(asciiDisplay);

#ifdef USE_LOG_MESSAGE
    emit logMessage(logSTATUS, q_name(), msg);
#else
    emit statusMessage(msg);
#endif
}

void cSsnxModel_direct::stopReceived()
{
    clearConnectionDescriptor();

    // If there are any commands left in the queue, move them to our
    // holding queue
    if (!mAsciiCommandQueue.empty())
    {
        mSavedCommandQueue.push(mAsciiCommandQueue.front());
        mAsciiCommandQueue.pop();
    }

    // Move all of the commands back into the active command queue.
    // The commands will be sent when the prompts are sent again.
    mAsciiCommandQueue.swap(mSavedCommandQueue);


#ifdef USE_LOG_MESSAGE
    emit logMessage(logSTATUS, q_name(), "STOP received from the GPS receiver!");
#else
    emit statusMessage("STOP received from the GPS receiver!");
#endif

    setStatus(sensor::eStatus::STOPPED);
}

void cSsnxModel_direct::sentAsciiCommand(const std::string& command)
{
    mSavedCommandQueue.push(command);
}

int cSsnxModel_direct::readIncomingData(std::string& data)
{
    auto n = mSerialPort.bytesAvailable();
    if (n == 0) return 0;

    QByteArray buffer = mSerialPort.readAll();

    data.append(buffer.toStdString());

    if (data.size() > n) n = data.size();

    return n;
}

int cSsnxModel_direct::sendOutgoingData(const std::string& data)
{
    mSerialPort.write(QByteArray::fromStdString(data));

    return 0;
}

void cSsnxModel_direct::pvtCartesian(const ssnx::gps::PVT_Cartesian_2_t& pvt)
{
    mPvtCartesianValid = pvt.dataValid;
    if (mPvtCartesianValid.HasChanged())
        emit pvtCartesianStateChanged(mPvtCartesianValid);

    mCartesianPVT.dataValid = pvt.dataValid;
    mCartesianPVT.timestamp_s = pvt.timestamp_s;

    if (!mCartesianPVT.dataValid) return;

    mCartesianPVT.datum = static_cast<::gps::eDatum>(pvt.Datum);
    mCartesianPVT.X_m = pvt.X_m;
    mCartesianPVT.Y_m = pvt.Y_m;
    mCartesianPVT.Z_m = pvt.Z_m;
    mCartesianPVT.Vx_mps = pvt.Vx_mps;
    mCartesianPVT.Vy_mps = pvt.Vy_mps;
    mCartesianPVT.Vz_mps = pvt.Vz_mps;
    mCartesianPVT.groundtrack_deg = pvt.GroundTrack_deg;

    if (pvt.HAccuracy_m.has_value())
        mCartesianPVT.hAccuracy_m = pvt.HAccuracy_m.value();

    if (pvt.VAccuracy_m.has_value())
        mCartesianPVT.vAccuracy_m = pvt.VAccuracy_m.value();

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
    }
}

void cSsnxModel_direct::pvtGeodetic(const ssnx::gps::PVT_Geodetic_2_t& pvt)
{
    mPvtGeodeticValid = pvt.dataValid;
    if (mPvtGeodeticValid.HasChanged())
        emit pvtGeodeticStateChanged(mPvtGeodeticValid);

    mPvtValid = pvt.dataValid;
    mPvtTimestamp_s = pvt.timestamp_s;

    mSolutionType = static_cast<::gps::eSolutionType>(pvt.Mode);

    emit solutionTypeChanged(::gps::to_int(mSolutionType));

    if (!mPvtValid)
    {
        emit positionChanged(-1, -1, -1);
        return;
    }

    mDatum = static_cast<::gps::eDatum>(pvt.Datum);

    mLatitude_rad = pvt.Lat_rad;
    mLongitude_rad = pvt.Lon_rad;
    mHeight_m = pvt.Height_m;;
    mUndulation_m = pvt.Undulation_m;
    mVn_mps = pvt.Vn_mps;
    mVe_mps = pvt.Ve_mps;
    mVu_mps = pvt.Vu_mps;
    mGroundTrack_deg = pvt.GroundTrack_deg;
    
    mNumBases = pvt.NrBases.value_or(0);

    mNumSV = pvt.NrSV;

    double height_m = mHeight_m - mUndulation_m;


    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
    }

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

    auto pos = rfb::fromGPS(mLatitude_rad, mLongitude_rad, height_m);

    // Adjust position here due to antenna offset on dolly
    //pos.x_mm + or - ???;
    //pos.y_mm + or - ???;
    pos.z_mm -= 400;

    emit positionChanged(pos.x_mm, pos.y_mm, pos.z_mm);

    if (getStatus() != sensor::eStatus::RUNNING)
        setStatus(sensor::eStatus::RUNNING);
}

void cSsnxModel_direct::posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov)
{
    mPosCovGeodeticValid = cov.dataValid;
    if (mPosCovGeodeticValid.HasChanged())
        emit posCovGeodeticStateChanged(mPosCovGeodeticValid);

    if (!cov.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(cov);
    }
}

void cSsnxModel_direct::velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov)
{
    mVelCovGeodeticValid = cov.dataValid;
    if (mVelCovGeodeticValid.HasChanged())
        emit velCovGeodeticStateChanged(mVelCovGeodeticValid);

    if (!cov.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(cov);
    }
}

void cSsnxModel_direct::posProjected(const ssnx::gps::POS_Projected_1_t& pvt)
{
    mPosProjectedValid = pvt.dataValid;
    if (mPosProjectedValid.HasChanged())
        emit posProjectedStateChanged(mPosProjectedValid);

    mPosPojected.dataValid = pvt.dataValid;
    mPosPojected.timestamp_s = pvt.timestamp_s;

    if (!mPosPojected.dataValid) return;

    mPosPojected.datum = static_cast<::gps::eDatum>(pvt.Datum);
    mPosPojected.HeightComputed = pvt.HeightComputed;
    mPosPojected.Northing_m = pvt.Northing_m;
    mPosPojected.Easting_m = pvt.Easting_m;
    mPosPojected.Alt_m = pvt.Alt_m;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
    }
}

void cSsnxModel_direct::receiverTime(const ssnx::gps::ReceiverTime_1_t& pvt)
{
    mReceiverTimeValid = pvt.dataValid;
    if (mReceiverTimeValid.HasChanged())
        emit receiverTimeStateChanged(mReceiverTimeValid);

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

void cSsnxModel_direct::rtcmDatum(const ssnx::gps::RtcmDatum_1_t& rtcm)
{
    mRtcmDatumValid = rtcm.dataValid;
    if (mRtcmDatumValid.HasChanged())
        emit rtcmDatumStateChanged(mRtcmDatumValid);

    if (!rtcm.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(rtcm);
    }
}

void cSsnxModel_direct::receiverStatus(const ssnx::gps::ReceiverStatus_2_t& status)
{
}

void cSsnxModel_direct::ntripClientStatus(const ssnx::gps::NTRIP_ClientStatus_1_t& status)
{
}


