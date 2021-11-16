
#include "SpidercamCtrl.hpp"

#include <iostream>


cSpidercamController::cSpidercamController(QObject* parent)
    :
        mpSocket(nullptr)
//    mSocket()
{
    mPort = 0;
}

cSpidercamController::~cSpidercamController()
{
    stopCommunications();
}

QString cSpidercamController::remoteEndpoint() const
{
    return mRemoteEndpoint.toString();
}

uint16_t cSpidercamController::remotePort() const
{
    return mPort;
}

bool cSpidercamController::isConnected() const
{
    return mpSocket->isValid() && (mpSocket->state() == QAbstractSocket::ConnectedState);
}


bool cSpidercamController::try_to_connect(std::string_view hostname, uint16_t port, bool use_ipv6)
{
    QHostInfo info = QHostInfo::fromName(QString(hostname.data()));
    if (info.error() != QHostInfo::NoError)
    {
        std::cerr << info.errorString().toStdString() << std::endl;
        return false;
    }

    QHostAddress remote_endpoint;

    auto endpoints = info.addresses();
    for (auto& endpoint : endpoints)
    {
        if (use_ipv6)
        {
            if (QAbstractSocket::IPv6Protocol != endpoint.protocol())
                continue;
            remote_endpoint = endpoint;
            break;
        }
        else
        {
            if (QAbstractSocket::IPv4Protocol != endpoint.protocol())
                continue;
            remote_endpoint = endpoint;
            break;
        }
    }

    if (remote_endpoint.isNull())
        return false;

    // Test connection parameters
    QTcpSocket socket;
 
    socket.connectToHost(remote_endpoint, port);

    bool result = socket.waitForConnected();

    if (result)
    {
        mPort = port;
        mRemoteEndpoint = remote_endpoint;
    }

    return result;
}

bool cSpidercamController::startCommunications()
{
    if (mpSocket) return true;

    mpSocket = new QTcpSocket();

    mpSocket->connectToHost(mRemoteEndpoint, mPort);

    return mpSocket->waitForConnected();
}

void cSpidercamController::stopCommunications()
{
    if (mpSocket && mpSocket->isOpen())
    {
        mpSocket->disconnectFromHost();
        mpSocket->close();
    }

    delete mpSocket; mpSocket = nullptr;
}

void cSpidercamController::clearIncomingBuffer()
{
    QByteArray response_buffer;
    auto len = mpSocket->bytesAvailable();
    while (len > 0)
    {
        response_buffer = mpSocket->readAll();
        response_buffer.clear();
        len = mpSocket->bytesAvailable();
    }

    mReplyBuffer.clear();
}

bool cSpidercamController::checkForReply()
{
    return mpSocket->bytesAvailable() > 0;
}

int cSpidercamController::send_cmd(const std::string_view msg)
{
    auto len = mpSocket->write(msg.data(), msg.size());
    mpSocket->flush();
    mpSocket->waitForBytesWritten();
    return len;
}

std::string cSpidercamController::recv_reply()
{
    if (!mpSocket->waitForReadyRead(1))
    {
        return std::string();
    }

    mReplyBuffer = mpSocket->readAll();

    std::string reply(mReplyBuffer.constData(), mReplyBuffer.size());
    auto pos = reply.find("</root>");

    if (pos != std::string::npos)
    {
        reply.erase(pos);
    }
    
    return reply;
}

void cSpidercamController::errorHandler(QAbstractSocket::SocketError socketError)
{
    switch (socketError) 
    {
    case QAbstractSocket::RemoteHostClosedError:
        std::cerr << "Remote host closed connection!" << std::endl;
        break;
    case QAbstractSocket::HostNotFoundError:
        std::cerr << "The host was not found. Please check the host name and port settings." << std::endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        std::cerr << "The connection was refused by the peer. ";
        std::cerr << "Make sure the fortune server is running, ";
        std::cerr << "and check that the host name and port ";
        std::cerr << "settings are correct." << std::endl;
        break;
    default:
        std::cerr << "The following error occurred: " << mpSocket->errorString().toStdString() << std::endl;
    }

}

