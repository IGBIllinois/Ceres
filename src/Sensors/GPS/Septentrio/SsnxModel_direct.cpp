
#include "SsnxModel_direct.hpp"
#include "RappFieldBoundary.hpp"

#include "../GpsUtils.hpp"

#include <ssnx/ssn_utils.hpp>

#include <QtSerialPort/QSerialPortInfo>

#include <functional>
#include <chrono>


#define USE_LOG_MESSAGE


using namespace ssnx;
using namespace std::chrono;


static std::chrono::nanoseconds TIMEOUT = std::chrono::seconds(10);


cSsnxModel_direct::cSsnxModel_direct(QObject* parent)
:
    cSsnxModel(parent),
    mSerialPort(this),
    mSerialBuffer(1024, '\0')
{
}

cSsnxModel_direct::cSsnxModel_direct(const std::string& instance, QObject* parent)
    :
    cSsnxModel(instance, parent),
    mSerialPort(this),
    mSerialBuffer(1024, '\0')
{
}


cSsnxModel_direct::~cSsnxModel_direct()
{
}

void cSsnxModel_direct::updateViews()
{
    emit sensorStatusChanging(q_name(), q_instance(), getStatus());

    emit pvtCartesianDataValid(mPvtCartesianValid);
    emit pvtGeodeticDataValid(mPvtGeodeticValid);
    emit posCovGeodeticDataValid(mPosCovGeodeticValid);
    emit velCovGeodeticDataValid(mVelCovGeodeticValid);
    emit posProjectedDataValid(mPosProjectedValid);
    emit receiverTimeDataValid(mReceiverTimeValid);
    emit rtcmDatumDataValid(mRtcmDatumValid);
    emit diffCorrDataValid(mDiffCorrValid);
    emit rtcmDatumDataValid(mRtcmDatumValid);
    emit ntripClientDataValid(mNtripClientValid);
    emit receiverStatusDataValid(mReceiverStatusValid);
    emit wifiClientDataValid(mWifiClientValid);
}

bool cSsnxModel_direct::configure(const nlohmann::json& jsonCfg)
{
    if (jsonCfg.contains("antenna_offset"))
    {
        auto antenna_offset = jsonCfg["antenna_offset"];

        if (antenna_offset.contains("x_mm (south is +)"))
            mAntennaOffset.x_mm = antenna_offset["x_mm (south is +)"];

        if (antenna_offset.contains("y_mm (east is +)"))
            mAntennaOffset.y_mm = antenna_offset["y_mm (east is +)"];

        if (antenna_offset.contains("z_mm (up is +)"))
            mAntennaOffset.z_mm = antenna_offset["z_mm (up is +)"];
    }

    emit statusMessage("Connecting to SSNX GPS receiver...");

    std::string portname;

    try
    {
        auto serial_port = jsonCfg["serial_port"];

#if defined(WIN32)
        portname = serial_port["windows"];
#elif defined(__APPLE_CC__)
        portname = serial_port["macOS"];
#else
        portname = serial_port["linux"];
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
        logMessage(logERROR, msg);
        return false;
    }

    auto result = cSsnxModel::configure(jsonCfg);

    return result;
}

bool cSsnxModel_direct::startCommunications()
{
    emit statusMessage("Trying to establishing GPS connection...");
    logMessage(logINFO, "Trying to establishing GPS connection...");

    if (!mSerialPort.open(QIODevice::ReadWrite))
    {
        logMessage(logERROR, "Could not establish connection to GPS receiver!");
        return false;
    }

    forcePromptRequest();
//    sendPromptRequest();

    if (mSerialPort.waitForBytesWritten(1000))
        setStatus(sensor::eStatus::CONNECTING);
    else
        setStatus(sensor::eStatus::PENDING);

    mLastReceived = std::chrono::high_resolution_clock::now();

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
{}

void cSsnxModel_direct::closeConnection()
{
    mSerialPort.close();
    logMessage(logERROR, "Serial connection to GPS is closed!");
}

bool cSsnxModel_direct::isConnected()
{
    if (!mSerialPort.isOpen())
    {
#ifdef USE_LOG_MESSAGE
        logMessage(logSTATUS, "Serial port is not open.");
#else
        emit statusMessage("Serial port is not open.");
#endif
    }

    return true;
}

void cSsnxModel_direct::communicationError(const std::string& errorString)
{
    QString msg = QString::fromStdString(errorString);
    logMessage(logERROR, msg);
}

void cSsnxModel_direct::newConnectionDescriptor(const std::string& connectionDescriptor)
{
    QString msg = "newConnectionDescriptor: ";
    msg += QString::fromStdString(connectionDescriptor);

#ifdef USE_LOG_MESSAGE
    logMessage(logSTATUS, msg);
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
    logMessage(logSTATUS, msg);
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
    logMessage(logSTATUS, "STOP received from the GPS receiver!");
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
    if (n == 0)
    {
        auto now = std::chrono::high_resolution_clock::now();

        auto interval = now - mLastReceived;

        if (interval > TIMEOUT)
        {
            forcePromptRequest();

            if (mSerialPort.waitForBytesWritten(1000))
                setStatus(sensor::eStatus::CONNECTING);
            else
                setStatus(sensor::eStatus::PENDING);

            mLastReceived = std::chrono::high_resolution_clock::now();
        }

        return 0;
    }

    mLastReceived = std::chrono::high_resolution_clock::now();

    QByteArray buffer = mSerialPort.readAll();

    if (buffer.isEmpty())
        return 0;

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
        emit pvtCartesianDataValid(mPvtCartesianValid);

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
        mSerializer.write(device_id(), pvt);
    }
}

void cSsnxModel_direct::pvtGeodetic(const ssnx::gps::PVT_Geodetic_2_t& pvt)
{
    mPvtGeodeticValid = pvt.dataValid;
    if (mPvtGeodeticValid.HasChanged())
        emit pvtGeodeticDataValid(mPvtGeodeticValid);

    mPvtValid = pvt.dataValid;
    mPvtTimestamp_s = pvt.timestamp_s;

    mSolutionType = static_cast<::gps::eSolutionType>(pvt.Mode);

    emit solutionTypeChanged(::gps::to_int(mSolutionType));

    if (!mPvtValid)
    {
        emit positionChanged(-100000, -100000, -100000);
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
    mHeightComputed = pvt.HeightComputed;
    
    mNumBases = pvt.NrBases.value_or(0);

    mNumSV = pvt.NrSV;

    double height_m = mHeight_m - mUndulation_m;


    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(device_id(), pvt);
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

    auto pos = rfb::fromGPS(mLatitude_rad, mLongitude_rad, height_m);

    // Adjust position here due to antenna offset on dolly
    pos.x_mm -= mAntennaOffset.x_mm;
    pos.y_mm -= mAntennaOffset.y_mm;
    pos.z_mm -= mAntennaOffset.z_mm;

    emit positionChanged(pos.x_mm, pos.y_mm, pos.z_mm);

    if (getStatus() != sensor::eStatus::RUNNING)
        setStatus(sensor::eStatus::RUNNING);
}

void cSsnxModel_direct::posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov)
{
    mPosCovGeodeticValid = cov.dataValid;
    if (mPosCovGeodeticValid.HasChanged())
        emit posCovGeodeticDataValid(mPosCovGeodeticValid);

    if (!cov.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(device_id(), cov);
    }
}

void cSsnxModel_direct::velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov)
{
    mVelCovGeodeticValid = cov.dataValid;
    if (mVelCovGeodeticValid.HasChanged())
        emit velCovGeodeticDataValid(mVelCovGeodeticValid);

    if (!cov.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(device_id(), cov);
    }
}

void cSsnxModel_direct::posProjected(const ssnx::gps::POS_Projected_1_t& pvt)
{
    mPosProjectedValid = pvt.dataValid;
    if (mPosProjectedValid.HasChanged())
        emit posProjectedDataValid(mPosProjectedValid);

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
        mSerializer.write(device_id(), pvt);
    }
}

void cSsnxModel_direct::receiverTime(const ssnx::gps::ReceiverTime_1_t& pvt)
{
    mReceiverTimeValid = pvt.dataValid;
    if (mReceiverTimeValid.HasChanged())
        emit receiverTimeDataValid(mReceiverTimeValid);

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
        mSerializer.write(device_id(), pvt);
    }

    emit updateUTC(mUtcHour, mUtcMinute, mUtcSecond, mUtcDay, mUtcMonth, mUtcYear);
}

void cSsnxModel_direct::diffCorrIn(const ssnx::gps::DiffCorrIn_1_t& diff_corr)
{
    mDiffCorrValid = diff_corr.dataValid;
    if (mDiffCorrValid.HasChanged())
        emit diffCorrDataValid(mDiffCorrValid);
}

void cSsnxModel_direct::rtcmDatum(const ssnx::gps::RtcmDatum_1_t& rtcm)
{
    mRtcmDatumValid = rtcm.dataValid;
    if (mRtcmDatumValid.HasChanged())
        emit rtcmDatumDataValid(mRtcmDatumValid);

    if (!rtcm.dataValid) return;

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(device_id(), rtcm);
    }
}

void cSsnxModel_direct::receiverStatus(const ssnx::gps::ReceiverStatus_2_t& status)
{
    mReceiverStatusValid = status.dataValid;
    if (mReceiverStatusValid.HasChanged())
        emit receiverStatusDataValid(mReceiverStatusValid);

    int error_code = 0;
    error_code |= status.RxError.Congestion ? 0x01 : 0x00;
    error_code |= status.RxError.CpuOverload ? 0x02 : 0x00;
    error_code |= status.RxError.InvalidConfig ? 0x04 : 0x00;
    error_code |= status.RxError.MissedEvent ? 0x08 : 0x00;
    error_code |= status.RxError.OutOfGeoFence ? 0x10 : 0x00;
    error_code |= status.RxError.Software ? 0x20 : 0x00;
    error_code |= status.RxError.Watchdog ? 0x40 : 0x00;

    mReceiverError = error_code;
    if (mReceiverError.HasChanged())
        emit receiverStatusChanged(mReceiverError);
}

void cSsnxModel_direct::ntripClientStatus(const ssnx::gps::NTRIP_ClientStatus_1_t& status)
{
    mNtripClientValid = status.dataValid;
    if (mNtripClientValid.HasChanged())
        emit ntripClientDataValid(mNtripClientValid);
    
    if (status.clients.empty())
    {
        mNtripClientStatus = 0;
        mNtripClientError = 0;
    }
    else
    {
        auto& client = status.clients.front();
        mNtripClientStatus = ssnx::to_uint8(client.Status);
        mNtripClientError = ssnx::to_uint8(client.ErrorCode);
    }

    if (mNtripClientStatus.HasChanged() || mNtripClientError.HasChanged())
        emit ntripClientStatusChanged(mNtripClientStatus, mNtripClientError);
}

void cSsnxModel_direct::wifiClientStatus(const ssnx::gps::WIFI_ClientStatus_1_t& status)
{
    mWifiClientValid = status.dataValid;
    if (mWifiClientValid.HasChanged())
        emit wifiClientDataValid(mWifiClientValid);

    if (status.dataValid)
        mWifiConnection = ssnx::to_uint8(status.Status);
    else
        mWifiConnection = 0;

    mWifiPowerLevel = status.PowerLevel_dBm;

    mWifiErrorCode = static_cast<uint8_t>(status.ErrorCode);

    if (mWifiConnection.HasChanged() || mWifiPowerLevel.HasChanged() || mWifiErrorCode.HasChanged())
        emit wifiClientConnectionChanged(mWifiConnection, mWifiPowerLevel, mWifiErrorCode);
}


