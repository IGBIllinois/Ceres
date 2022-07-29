
#include "RemoteDataModel.hpp"
#include "SensorModel.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QTimer>

#include <sstream>


namespace
{
    std::string to_human_readable_size(std::uintmax_t fs)
    {
        int i = 0;
        double mantissa = fs;
        for (; mantissa >= 1024.0; mantissa /= 1024.0, ++i) {}
        mantissa = std::ceil(mantissa * 10.0) / 10.0;

        std::ostringstream out;
        out.precision(3);
        out << std::fixed << mantissa;
        std::string result = out.str();
        result += "BKMGTPE"[i];
        if (i > 0)
            result += "B";

        return result;
    }
}

cRemoteDataModel::cRemoteDataModel(QObject* parent)
    :
    cDataModel(parent),
    cCeresRemoteClientNetEncoder(4096),
    mThread(this),
    mpTcpServer(nullptr),
    mpClient(nullptr),
    mSerializer(8192), mSpidercamSerializer(1024), mWeatherSerializer(1024)

{
    mWindDataValid = false;
    mWindSpeed_mps = 0.0;
    mWindDirection_deg = 0.0;

    mIsRecording = false;
    mIsExperimentRunning = false;

    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cRemoteDataModel::onStatusUpdate);

    mpTcpServer = new QTcpServer();

    connect(mpTcpServer, &QTcpServer::acceptError, this, &cRemoteDataModel::acceptError);
    connect(mpTcpServer, &QTcpServer::newConnection, this, &cRemoteDataModel::newConnection);

    mpHeartbeatTimer = new QTimer(this);
    connect(mpHeartbeatTimer, &QTimer::timeout, this, &cRemoteDataModel::onHeartbeat);
}

cRemoteDataModel::~cRemoteDataModel()
{
    mpTcpServer->close();
}

std::string cRemoteDataModel::defaultDataPath() const
{
    return mDefaultDataPath.string();
}

void cRemoteDataModel::setDefaultDataPath(const std::string& data_path)
{
    using namespace std::filesystem;

    mDefaultDataPath = data_path;

    if (!exists(mDefaultDataPath))
    {
        create_directories(mDefaultDataPath);
    }
}

bool cRemoteDataModel::startTcpServer(const std::string& ip, uint16_t port)
{
    QHostAddress local_endpoint(ip.c_str());

    return mpTcpServer->listen(local_endpoint, port);
}

void cRemoteDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        connect(pSensor, &cSensorModel::sensorStatusChanging, this, &cRemoteDataModel::updateSensorStatus);
        connect(pSensor, &cSensorModel::sensorNameChanging, this, &cRemoteDataModel::updateSensorName);

        if (!pSensor->initialize())
        {
            emit statusMessage("Sensor failed initialization!");
            return;
        }

        pSensor->moveToThread(&mThread);
        mThread.mSensors.push_back(pSensor);
    }
}

void cRemoteDataModel::startDataThread()
{
    mThread.start();
}

void cRemoteDataModel::stopDataThread()
{
    mThread.stop();
}

void cRemoteDataModel::sendStatusMessage(const QString& msg)
{
    cCeresRemoteClientNetEncoder::sendStatusMessage(msg.toStdString());
}

void cRemoteDataModel::sendStatusMessage(const std::string& msg)
{
    cCeresRemoteClientNetEncoder::sendStatusMessage(msg);
}


void cRemoteDataModel::sendLogMessage(uint8_t type, const QString& device, const QString& msg)
{
    cCeresRemoteClientNetEncoder::sendLogMessage(type, device.toStdString(), msg.toStdString());
}

void cRemoteDataModel::sendLogMessage(uint8_t type, const std::string& device, const std::string& msg)
{
    cCeresRemoteClientNetEncoder::sendLogMessage(type, device, msg);
}

/********************************************************************
 * Packet Handlers
 *******************************************************************/

void cRemoteDataModel::onOpenDataFile(const std::string& fileName)
{
    using namespace std::filesystem;

    if (mFile.isOpen())
    {
        return; // false;
    }

    if (fileName.empty() && mExperimentTitle.empty())
    {
        sendDataFileState(false);
        return; // false;
    }

    std::string qualifiedFileName = fileName;
    if (qualifiedFileName.empty())
    {
        qualifiedFileName = mExperimentTitle;
    }

    auto ext = qualifiedFileName.find_last_of('.');

    char timestamp[100] = {'\0'};

    std::time_t t = std::time(nullptr);
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", std::localtime(&t));

    qualifiedFileName += "_";
    qualifiedFileName += timestamp;
    qualifiedFileName += ".ceres";

    std::replace_if(qualifiedFileName.begin(), qualifiedFileName.end(),
        [](QString::value_type c) {return c <= QChar::Space; }, '_');

    mFullyQualifiedFileName = mDefaultDataPath / qualifiedFileName;

    path testPath = mFullyQualifiedFileName;
    testPath.remove_filename();
    if (!exists(testPath))
    {
        create_directories(testPath);
    }

    QString msg = "Opening File: ";
    msg.append(mFullyQualifiedFileName.c_str());
    emit statusMessage(msg);

    mFile.open(mFullyQualifiedFileName.string());

    if (!mFile.isOpen())
    {
        sendDataFileState(false);
        QString msg = "Failed to open file ";
        msg.append(mFullyQualifiedFileName.c_str());
        sendLogMessage(logERROR, "Remote Client", msg);
        emit statusMessage(msg);
        return;
    }

    mSerializer.attach(&mFile);
    mSpidercamSerializer.attach(&mFile);
    mWeatherSerializer.attach(&mFile);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->enableDataRecording(mFile);
    }

    sendDataFileState(true);
}

void cRemoteDataModel::onCloseDataFile()
{
    using namespace std::filesystem;

    if (!mFile.isOpen())
        return;

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->disableDataRecording();
    }

    mSerializer.endTime(time(nullptr));
    mIsRecording = false;

    mSerializer.detach();
    mSpidercamSerializer.detach();
    mWeatherSerializer.detach();

    mFile.close();

    sendDataFileState(false);
    emit statusMessage("Data file closed.");

    auto fs = file_size(mFullyQualifiedFileName);
    std::string msg = mFullyQualifiedFileName.filename().string();
    msg += ", size = ";
    msg += to_human_readable_size(fs);
    sendLogMessage(logINFO, "Remote Client", msg);
}

void cRemoteDataModel::onStartDataRecording()
{
    if (mIsRecording) return;

    mIsRecording = true;
    emit requestDataRecordingState(true);

    mSerializer.startRecordingTimestamp(timestamp_ns());
    mpHeartbeatTimer->start(1000);

    emit statusMessage("Data recording started.");
}

void cRemoteDataModel::onStopDataRecording()
{
    if (!mIsRecording) return;

    mIsRecording = false;
    mpHeartbeatTimer->stop();

    emit requestDataRecordingState(false);
    mSerializer.endRecordingTimestamp(timestamp_ns());

    emit statusMessage("Data recording stopped.");
}

void cRemoteDataModel::onStartExperiment()
{
    mIsExperimentRunning = true;

    mSerializer.writeBeginHeader();
    mSerializer.writeTitle(mExperimentTitle);

    if (!mResearcher.empty())
        mSerializer.writeResearcher(mResearcher);

    if (!mCultivar.empty())
        mSerializer.writeCultivar(mCultivar);

    mSerializer.writeExperimentDoc(mExperimentDoc);

    mSerializer.writeBeginSensorList();
    for (auto& sensor : mThread.mActiveSensors)
    {
        mSerializer.writeSensorBlockInfo(sensor->data_class_id(), sensor->name());
    }
    mSerializer.writeEndOfSensorList();

    // Write Data Header

    mSpidercamSerializer.write(mDollyPosition);

    mWeatherSerializer.writeWindData_mps(mWindDataValid,
        mWindSpeed_mps, mWindDirection_deg);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataHeader();
    }

    mSerializer.startTime(time(nullptr));
    mSerializer.writeEndOfHeader();

    emit statusMessage("Experiment Started!");
}

void cRemoteDataModel::onStopExperiment()
{
    onStopDataRecording();

    if (static_cast<bool>(mSerializer))
    {
        mSerializer.writeBeginFooter();
        for (auto& sensor : mThread.mActiveSensors)
        {
            sensor->writeDataFooter();
        }
        mSerializer.writeEndOfFooter();
    }

    mExperimentTitle.clear();
    mResearcher.clear();
    mCultivar.clear();
    mExperimentDoc.clear();

    mIsExperimentRunning = false;

    emit statusMessage("Experiment Stopped!");
}

void cRemoteDataModel::onExperimentInfo(const std::string& title, 
    const std::string& researcher, const std::string& cultivar, const std::string& doc)
{
    mExperimentTitle = title;
    mResearcher = researcher;
    mCultivar = cultivar;
    mExperimentDoc = doc;

    if (mSerializer.bufferCapacity() < mExperimentDoc.size())
        mSerializer.setBufferCapacity(mExperimentDoc.size() + 32);
}


void cRemoteDataModel::onSpidercamPosition(const spidercam::sPosition_1_t& pos)
{
    mDollyPosition = pos;

    if (mIsRecording)
    {
        mSpidercamSerializer.write(mDollyPosition);
    }
}

void cRemoteDataModel::onWeatherData(bool valid, double wind_speed_mps, double wind_direction_deg)
{
    mWindDataValid = valid;
    mWindSpeed_mps = wind_speed_mps;
    mWindDirection_deg = wind_direction_deg;

    if (mIsRecording)
    {
        mWeatherSerializer.writeWindData_mps(mWindDataValid, 
            mWindSpeed_mps, mWindDirection_deg);
    }
}

void cRemoteDataModel::onHeartbeat()
{
    if (mIsRecording)
    {
        mSerializer.heartbeatTimestamp(timestamp_ns());
    }
}

/***   Signals handlers from the sensors   ****/
void cRemoteDataModel::updateSensorStatus(QString name, sensor::eStatus status)
{
    if (mpClient)
    {
        sendSensorStatus(name.toStdString(),
            to_string(status));

        emit statusMessage("updateSensorStatus");
    }
}

void cRemoteDataModel::updateSensorName(QString old_name, QString new_name)
{
    if (mpClient)
    {
        sendSensorNameChange(old_name.toStdString(),
            new_name.toStdString());

        emit statusMessage("updateSensorName");
    }
}


/***   Signals handlers from the TCP server   ***/

void cRemoteDataModel::acceptError(QAbstractSocket::SocketError socketError)
{
}

void cRemoteDataModel::newConnection()
{
    QTcpSocket* client = mpTcpServer->nextPendingConnection();

    if (mpClient)
    {
        client->close();
        client->deleteLater();
    }

    if (client)
    {
        emit statusMessage("Connected to client.");
        mpClient = client;
        mpClient->setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);

        QObject::connect(mpClient, &QTcpSocket::readyRead, this, &cRemoteDataModel::processNewCommand);
        QObject::connect(mpClient, &QTcpSocket::disconnected, this, &cRemoteDataModel::clientDisconnected);
        QObject::connect(mpClient, &QTcpSocket::errorOccurred, this, &cRemoteDataModel::clientErrorOccurred);
        QObject::connect(mpClient, &QTcpSocket::stateChanged, this, &cRemoteDataModel::clientStateChanged);

        for (auto& sensor : mThread.mSensors)
        {
            encodeSensorStatus(sensor->name(), to_string(sensor->status()));
        }
        cNetworkEncoder::sendData();
        emit statusMessage("Sent sensor status.");
    }
}

void cRemoteDataModel::processNewCommand()
{
    QByteArray buffer = mpClient->readAll();

    if (buffer.isEmpty()) return;

    decode(buffer.constData(), buffer.size());
}

void cRemoteDataModel::clientDisconnected()
{
    qInfo() << "Disconnecting client...";

    QObject::disconnect(mpClient, &QTcpSocket::readyRead, this, &cRemoteDataModel::processNewCommand);
    QObject::disconnect(mpClient, &QTcpSocket::disconnected, this, &cRemoteDataModel::clientDisconnected);
    QObject::disconnect(mpClient, &QTcpSocket::errorOccurred, this, &cRemoteDataModel::clientErrorOccurred);
    QObject::disconnect(mpClient, &QTcpSocket::stateChanged, this, &cRemoteDataModel::clientStateChanged);

    qInfo() << "Client delete later...";
    mpClient->close();
    mpClient->deleteLater();
    mpClient = nullptr;

    if (mIsRecording)
    {
        qInfo() << "onStopDataRecording...";
        onStopDataRecording();
    }

    if (mIsExperimentRunning)
    {
        qInfo() << "onStopExperiment...";
        onStopExperiment();
    }

    qInfo() << "onCloseDataFile...";
    onCloseDataFile();

    qInfo() << "Client is disconnected!";

    emit statusMessage("Client is disconnected!");
}

void cRemoteDataModel::clientErrorOccurred(QAbstractSocket::SocketError socketError)
{

}

void cRemoteDataModel::clientStateChanged(QAbstractSocket::SocketState socketState)
{

}


int cRemoteDataModel::sendOutgoingData(const char* data, std::size_t len)
{
    if (!mpClient)
        return 0;

    auto n = mpClient->write(data, len);

    mpClient->flush();

    return n;
}


