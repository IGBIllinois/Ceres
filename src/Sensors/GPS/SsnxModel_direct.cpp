
#include "SsnxModel_direct.hpp"

#include <QtSerialPort/QSerialPortInfo>

#include <functional>


using namespace ssnx;

cSsnxModel_direct::cSsnxModel_direct(QObject* parent)
:
    cSsnxModel(parent),
    mSerialPort(this),
    mSerialBuffer(1024, '\0'),
    mSerializer(4096)
{
}

cSsnxModel_direct::~cSsnxModel_direct()
{
}

uint16_t cSsnxModel_direct::data_class_id() const
{
    return mSerializer.classID();
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

    sendPromptRequest();

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

void cSsnxModel_direct::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cSsnxModel_direct::disableDataRecording()
{
    cGpsModel::disableDataRecording();
    mSerializer.detach();
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
        emit statusMessage("Serial port is not open.");
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
    emit statusMessage(msg);

    setStatus(sensor::eStatus::CONNECTING);
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
    emit statusMessage(msg);
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

    emit statusMessage("STOP received from the GPS receiver!");

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


void cSsnxModel_direct::pvtGeodetic(const ssnx::gps::PVT_Geodetic_2_t& pvt)
{
    mPvtValid = pvt.dataValid;
    mPvtTimestamp_s = pvt.timestamp_s;

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

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.write(pvt);
    }

    if (mRecordTrack)
    {
        ::gps::sGpsPoint point = { mPvtTimestamp_s,
            mLatitude_rad, mLongitude_rad, mHeight_m,
            mVn_mps, mVe_mps, mVu_mps,
            mGroundTrack_deg };

        mTrack.emplace_back(point);
    }

    emit updatePVT(mPvtTimestamp_s,
        mLatitude_rad, mLongitude_rad, mHeight_m,
        mVn_mps, mVe_mps, mVu_mps,
        mGroundTrack_deg, mDatum);

    if (status() != sensor::eStatus::RUNNING)
        setStatus(sensor::eStatus::RUNNING);
}

void cSsnxModel_direct::posCovGeodetic(const ssnx::gps::PosCovGeodetic_1_t& cov)
{
}

void cSsnxModel_direct::velCovGeodetic(const ssnx::gps::VelCovGeodetic_1_t& cov)
{
}

void cSsnxModel_direct::posProjected(const ssnx::gps::POS_Projected_1_t& pvt)
{
}

void cSsnxModel_direct::receiverTime(const ssnx::gps::ReceiverTime_1_t& pvt)
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

void cSsnxModel_direct::rtcmDatum(const ssnx::gps::RtcmDatum_1_t& rtcm)
{
}


