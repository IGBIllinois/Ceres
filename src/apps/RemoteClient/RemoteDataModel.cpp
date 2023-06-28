
#include "RemoteDataModel.hpp"
#include "SensorModel.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QTimer>

#include <sstream>
#include <ctime>


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
    mLocalIpAddress = ip;

    for( auto* pController : mSensorControllers)
    {
        pController->startTcpServer(mLocalIpAddress);
        //          pController->moveToThread(&mThread);
    }

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

void cRemoteDataModel::addSensorController(cSensorController* pController)
{
    if (pController)
    {
        if (!mLocalIpAddress.empty())
        {
            pController->startTcpServer(mLocalIpAddress);
            //          pController->moveToThread(&mThread);
        }
        mSensorControllers.push_back(pController);
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
        sendDataFileState(true);
        return;
    }

    if (fileName.empty() && mExperimentTitle.empty())
    {
        sendDataFileState(false);
        return;
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
        sendDataFileState(false);
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
    if (!mFile.isOpen())
    {
        sendDataFileState(false);
        return;
    }

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

    if (static_cast<bool>(mSerializer))
        mSerializer.endRecordingTimestamp(timestamp_ns());

    emit statusMessage("Data recording stopped.");
}

void cRemoteDataModel::onStartExperiment()
{
    mIsExperimentRunning = true;

    if (!static_cast<bool>(mSerializer))
    {
        emit statusMessage("Experiment Started: no data recording!");
        return;
    }

    mSerializer.writeBeginHeader();
    mSerializer.writeTitle(mExperimentTitle);

    if (!mResearchers.empty())
    {
        if (mResearchers.size() == 1)
            mSerializer.writeResearcher(mResearchers[0]);
        else
            mSerializer.writeResearchers(mResearchers);
    }

    if (!mSpecies.empty())
        mSerializer.writeSpecies(mSpecies);

    if (!mCultivar.empty())
        mSerializer.writeCultivar(mCultivar);

    if (!mPermitInfo.empty())
        mSerializer.writePermitInfo(mPermitInfo);

    if (!mPrincipalInvestigator.empty())
        mSerializer.writePrincipalInvestigator(mPrincipalInvestigator);

    if (!mConstructName.empty())
        mSerializer.writeConstructName(mConstructName);

    if (!mEventNumber.empty())
        mSerializer.writeEventNumber(mEventNumber);

    if (!mFieldDesign.empty())
        mSerializer.writeFieldDesign(mFieldDesign);

    if (mPlantingDate > 0)
    {
        auto tm = localtime(&mPlantingDate);
        std::uint16_t year = tm->tm_year + 1900;
        std::uint8_t month = tm->tm_mon + 1;
        std::uint8_t day = tm->tm_mday;
        std::uint16_t doy = tm->tm_yday;

        mSerializer.writePlantingDate(year, month, day, doy);
    }

    if (mHarvestDate > 0)
    {
        auto tm = localtime(&mHarvestDate);
        std::uint16_t year = tm->tm_year + 1900;
        std::uint8_t month = tm->tm_mon + 1;
        std::uint8_t day = tm->tm_mday;
        std::uint16_t doy = tm->tm_yday;

        mSerializer.writePlantingDate(year, month, day, doy);
    }

    if (!mTreatments.empty())
        mSerializer.writeTreatments(mTreatments);

    if (!mComments.empty())
        mSerializer.writeComments(mComments);
    
    time_t t = time(nullptr);
    auto tm = localtime(&t);
    std::uint16_t year = tm->tm_year + 1900;
    std::uint8_t month = tm->tm_mon + 1;
    std::uint8_t day = tm->tm_mday;

    std::uint16_t doy = tm->tm_yday;

    std::uint8_t hour = tm->tm_hour;
    std::uint8_t minute = tm->tm_min;
    std::uint8_t sec = tm->tm_sec;

    mSerializer.writeFileDate(year, month, day);
    mSerializer.writeFileTime(hour, minute, sec);
    mSerializer.writeDayOfYear(doy);

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

    clearExperimentInfo();

    mIsExperimentRunning = false;

    emit statusMessage("Experiment Stopped!");
}

void cRemoteDataModel::onExperimentInfo(const std::string& title, 
    const std::string& researcher, const std::string& cultivar, const std::string& doc)
{
    mExperimentTitle = title;

    if (!researcher.empty())
        mResearchers.push_back(researcher);

    mSpecies.clear();
    mCultivar = cultivar;
    mExperimentDoc = doc;

    if (mSerializer.bufferCapacity() < mExperimentDoc.size())
        mSerializer.setBufferCapacity(mExperimentDoc.size() + 32);
}

void cRemoteDataModel::onExperimentInfo(const std::string& title,
    const std::string& researcher, const std::string& species,
    const std::string& cultivar, const std::string& doc)
{
    mExperimentTitle = title;

    if (!researcher.empty())
        mResearchers.push_back(researcher);

    mSpecies = species;
    mCultivar = cultivar;
    mExperimentDoc = doc;

    if (mSerializer.bufferCapacity() < mExperimentDoc.size())
        mSerializer.setBufferCapacity(mExperimentDoc.size() + 32);
}

void cRemoteDataModel::onPrincipalInvestigator(const std::string& pi)
{
    mPrincipalInvestigator = pi;

}

void cRemoteDataModel::onResearcher(const std::string& researcher)
{
    mResearchers.push_back(researcher);

}

void cRemoteDataModel::onConstructName(const std::string& name)
{
    mConstructName = name;
}

void cRemoteDataModel::onEventNumber(const std::string& event_num)
{
    mEventNumber = event_num;
}

void cRemoteDataModel::onFieldDesign(const std::string& design)
{
    mFieldDesign = design;
}

void cRemoteDataModel::onPlantingDate(std::time_t date)
{
    mPlantingDate = date;
}

void cRemoteDataModel::onHarvestDate(std::time_t date)
{
    mHarvestDate = date;
}

void cRemoteDataModel::onTreatment(const std::string& treatment)
{
    mTreatments.push_back(treatment);
}

void cRemoteDataModel::onComment(const std::string& comment)
{
    mComments.push_back(comment);
}

void cRemoteDataModel::onPermitInfo(const std::string& permit)
{
    mPermitInfo = permit;
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

        emit statusMessage("Sent Sensor Status.");
    }
}

void cRemoteDataModel::updateSensorName(QString old_name, QString new_name)
{
    if (mpClient)
    {
        sendSensorNameChange(old_name.toStdString(),
            new_name.toStdString());

        emit statusMessage("Sent Sensor Name Change.");
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

        for (auto& controller : mSensorControllers)
        {
            std::string sensor = controller->descriptor();
            std::string model = controller->model();
            auto version = controller->version();
            std::string name = controller->name();
            auto ip_address = controller->serverIpAddress();
            auto port = controller->serverPort();
            encodeSensorPropertyConnectInfo(sensor, model, version, name, ip_address, port);
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

    clearExperimentInfo();

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

void cRemoteDataModel::clearExperimentInfo()
{
    mExperimentTitle.clear();
    mPrincipalInvestigator.clear();
    mResearchers.clear();
    mSpecies.clear();
    mCultivar.clear();
    mPermitInfo.clear();
    mConstructName.clear();
    mEventNumber.clear();
    mFieldDesign.clear();
    mExperimentDoc.clear();
    mPlantingDate = 0;
    mHarvestDate = 0;
    mTreatments.clear();
    mComments.clear();

}

