
#include "RemoteDataModel.hpp"

#include "SensorModel.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

cRemoteDataModel::cRemoteDataModel(QObject* parent)
    :
    cDataModel(parent),
    cCeresRemoteClientNetEncoder(4096),
    mThread(this),
    mpTcpServer(nullptr),
    mpClient(nullptr)
{
    mIsRecording = false;

    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cRemoteDataModel::onStatusUpdate);

    mpTcpServer = new QTcpServer();

    QObject::connect(mpTcpServer, &QTcpServer::acceptError, this, &cRemoteDataModel::acceptError);
    QObject::connect(mpTcpServer, &QTcpServer::newConnection, this, &cRemoteDataModel::newConnection);
}

cRemoteDataModel::~cRemoteDataModel()
{
    mpTcpServer->close();
}

const std::string& cRemoteDataModel::defaultDataPath() const
{
    return mDefaultDataPath;
}

void cRemoteDataModel::setDefaultDataPath(const std::string& data_path)
{
    mDefaultDataPath = data_path;

    if (!mDefaultDataPath.empty())
    {
        if (mDefaultDataPath.back() != '/')
            mDefaultDataPath += '/';
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
        if (!pSensor->initialize())
        {
            emit statusMessage("Sensor failed initialization!");
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

void cRemoteDataModel::openDataFile(const std::string& fileName)
{
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

    char timestamp[100];

    std::time_t t = std::time(nullptr);
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", std::localtime(&t));

    qualifiedFileName.insert(ext, "_");
    qualifiedFileName.insert(ext + 1, timestamp);

    std::replace_if(qualifiedFileName.begin(), qualifiedFileName.end(),
        [](QString::value_type c) {return c <= QChar::Space; }, '_');

    std::string fullyQualifiedFileName = mDefaultDataPath + qualifiedFileName;
    mFile.open(fullyQualifiedFileName);

    if (mFile.isOpen())
    {
        writeDataHeaders();
        sendDataFileState(true);
        return;
    }

    sendDataFileState(false);
}

void cRemoteDataModel::closeDataFile()
{
    mSerializer.endTime(time(nullptr));
    mIsRecording = false;

    mSerializer.detach();
    mSpidercamSerializer.detach();
    mWeatherSerializer.detach();

    mFile.close();

    sendDataFileState(false);
}


void cRemoteDataModel::writeDataHeaders()
{
    mSerializer.attach(&mFile);
    mSpidercamSerializer.attach(&mFile);
    mWeatherSerializer.attach(&mFile);

    mSerializer.writeTitle(mExperimentTitle);

    if (!mResearcher.empty())
        mSerializer.writeResearcher(mResearcher);

    if (!mCultivar.empty())
        mSerializer.writeCultivar(mCultivar);

    mSerializer.writeExperimentDoc(mExperimentDoc);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataHeader(mFile);
    }

    mSerializer.startTime(time(nullptr));
}


void cRemoteDataModel::startExperiment()
{
    mSerializer.startTimestamp(timestamp_ns());
}

void cRemoteDataModel::stopExperiment()
{
    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->endDataRecording();
    }

    mSerializer.endTimestamp(timestamp_ns());

    closeDataFile();

    mExperimentTitle.clear();
    mResearcher.clear();
    mCultivar.clear();
    mExperimentDoc.clear();
}

void cRemoteDataModel::experimentInfo(const std::string& title, 
    const std::string& researcher, const std::string& cultivar, const std::string& doc)
{
    mExperimentTitle = title;
    mResearcher = researcher;
    mCultivar = cultivar;
    mExperimentDoc = doc;
}


void cRemoteDataModel::spidercamPosition(const spidercam::sPosition& pos)
{
    emit statusMessage("Receiving spidercam data.");
}

void cRemoteDataModel::weatherData(bool valid, double wind_speed_mps, double wind_direction_deg)
{
    emit statusMessage("Receiving weather data.");
}

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
    QObject::disconnect(mpClient, &QTcpSocket::readyRead, this, &cRemoteDataModel::processNewCommand);
    QObject::disconnect(mpClient, &QTcpSocket::disconnected, this, &cRemoteDataModel::clientDisconnected);
    QObject::disconnect(mpClient, &QTcpSocket::errorOccurred, this, &cRemoteDataModel::clientErrorOccurred);
    QObject::disconnect(mpClient, &QTcpSocket::stateChanged, this, &cRemoteDataModel::clientStateChanged);

    mpClient->deleteLater();
    mpClient = nullptr;

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


