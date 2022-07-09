
#include "RemoteDataModel.hpp"

#include "SensorModel.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

cRemoteDataModel::cRemoteDataModel(QObject* parent)
    :
    cDataModel(parent),
    mThread(this),
    mpTcpServer(nullptr),
    mpClient(nullptr)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cRemoteDataModel::onStatusUpdate);

    mpTcpServer = new QTcpServer();

    QObject::connect(mpTcpServer, &QTcpServer::acceptError, this, &cRemoteDataModel::acceptError);
    QObject::connect(mpTcpServer, &QTcpServer::newConnection, this, &cRemoteDataModel::newConnection);
}

cRemoteDataModel::~cRemoteDataModel()
{
    mpTcpServer->close();
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

bool cRemoteDataModel::openDataFile(const QString& fileName)
{
    if (fileName.isEmpty())
        return false;

    if (mFile.isOpen())
        return false;

    QString qualifiedFileName = fileName;

    auto ext = qualifiedFileName.lastIndexOf('.');

    char timestamp[100];

    std::time_t t = std::time(nullptr);
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", std::localtime(&t));

    qualifiedFileName.insert(ext, "_");
    qualifiedFileName.insert(ext + 1, timestamp);

    std::replace_if(qualifiedFileName.begin(), qualifiedFileName.end(),
        [](QString::value_type c) {return c <= QChar::Space; }, '_');

    mFile.open(qualifiedFileName.toStdString());

    mSerializer.attach(&mFile);

    return mFile.isOpen();

}

void cRemoteDataModel::closeDataFile()
{
    mSerializer.endTime(time(nullptr));
    mSerializer.detach();
    mFile.close();
}


void cRemoteDataModel::startExperiment()
{
//    mThread.mpController->writeDataHeader(mFile);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataHeader(mFile);
    }

    mSerializer.startTime(time(nullptr));
}

void cRemoteDataModel::stopExperiment()
{
//    mThread.mpController->stopDataRecording();

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->endDataRecording();
    }

    closeDataFile();

}

void cRemoteDataModel::experimentInfo(const std::string& title, 
    const std::string& researcher, const std::string& cultivar, const std::string& doc)
{

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

