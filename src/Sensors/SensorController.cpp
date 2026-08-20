
#include "SensorController.hpp"

#include <QTcpServer>
#include <QTcpSocket>


cSensorController::cSensorController(QObject* parent)
:
    QObject(parent)
{
    mpTcpServer = new QTcpServer();

    connect(mpTcpServer, &QTcpServer::acceptError, this, &cSensorController::acceptError);
    connect(mpTcpServer, &QTcpServer::newConnection, this, &cSensorController::newConnection);
}

cSensorController::~cSensorController()
{
    mpTcpServer->close();
}

void cSensorController::connectToModel()
{

}

bool cSensorController::startTcpServer(const std::string& ip)
{
    QHostAddress local_endpoint(ip.c_str());
    return mpTcpServer->listen(local_endpoint);
}

std::string cSensorController::serverIpAddress() const
{
    auto str = mpTcpServer->serverAddress().toString();
    return str.toStdString();
}

uint16_t cSensorController::serverPort() const
{
    return mpTcpServer->serverPort();
}

bool cSensorController::hasClient() const
{
    return mpClient;
}

void cSensorController::update()
{
    if (mpClient)
    {
        if (mpClient->bytesAvailable() > 0)
            processNewCommand();
    }
}

int cSensorController::sendOutgoingData(const char* data, std::size_t len)
{
    if (!mpClient)
        return 0;

    if (!data || (len == 0))
        return 0;

    auto n = mpClient->write(data, len);

    mpClient->flush();

    return n;
}


/***   Signals handlers from the TCP server   ***/

void cSensorController::acceptError(QAbstractSocket::SocketError socketError)
{
}

void cSensorController::newConnection()
{
    QTcpSocket* client = mpTcpServer->nextPendingConnection();

    if (mpClient)
    {
        client->close();
        client->deleteLater();
    }

    if (client)
    {
        mpClient = client;
        mpClient->setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);

        QObject::connect(mpClient, &QTcpSocket::readyRead, this, &cSensorController::processNewCommand);
        QObject::connect(mpClient, &QTcpSocket::disconnected, this, &cSensorController::clientDisconnected);
        QObject::connect(mpClient, &QTcpSocket::errorOccurred, this, &cSensorController::clientErrorOccurred);
        QObject::connect(mpClient, &QTcpSocket::stateChanged, this, &cSensorController::clientStateChanged);
    }
}

void cSensorController::processNewCommand()
{
    QByteArray buffer = mpClient->readAll();

    if (buffer.isEmpty()) return;

    processStream(buffer.constData(), buffer.size());
}

void cSensorController::clientDisconnected()
{
    QObject::disconnect(mpClient, &QTcpSocket::readyRead, this, &cSensorController::processNewCommand);
    QObject::disconnect(mpClient, &QTcpSocket::disconnected, this, &cSensorController::clientDisconnected);
    QObject::disconnect(mpClient, &QTcpSocket::errorOccurred, this, &cSensorController::clientErrorOccurred);
    QObject::disconnect(mpClient, &QTcpSocket::stateChanged, this, &cSensorController::clientStateChanged);

    mpClient->close();
    mpClient->deleteLater();
    mpClient = nullptr;
}

void cSensorController::clientErrorOccurred(QAbstractSocket::SocketError socketError)
{}

void cSensorController::clientStateChanged(QAbstractSocket::SocketState socketState)
{}


