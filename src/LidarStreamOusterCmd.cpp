
#include "LidarStreamOusterCmd.hpp"

#include <QtNetwork/QHostInfo>
#include <iostream>


cOusterCmdStream_Qt::cOusterCmdStream_Qt()
    :
        mSocket()
{
}

cOusterCmdStream_Qt::~cOusterCmdStream_Qt()
{
    if (mSocket.isOpen())
    {
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}

bool cOusterCmdStream_Qt::try_to_connect(std::string_view hostname, uint16_t port, bool use_ipv6)
{
    QHostInfo info = QHostInfo::fromName(QString(hostname.data()));
    if (info.error() != QHostInfo::NoError)
    {
        std::cerr << info.errorString().toStdString() << std::endl;
        return false;
    }

    QHostAddress local_endpoint;

    auto endpoints = info.addresses();
    for (auto& endpoint : endpoints)
    {
        if (use_ipv6)
        {
            if (QAbstractSocket::IPv6Protocol != endpoint.protocol())
                continue;
            local_endpoint = endpoint;
            break;
        }
        else
        {
            if (QAbstractSocket::IPv4Protocol != endpoint.protocol())
                continue;
            local_endpoint = endpoint;
            break;
        }
    }

    if (local_endpoint.isNull())
        return false;

    mSocket.connectToHost(local_endpoint, port);
    mSocket.waitForConnected();

    return true;
}

int cOusterCmdStream_Qt::send_cmd(const std::string_view msg)
{
    auto len = mSocket.write(msg.data(), msg.size());
    mSocket.flush();
    mSocket.waitForBytesWritten();
    return len;
}

std::string cOusterCmdStream_Qt::recv_reply()
{
    if (!mSocket.waitForReadyRead())
    {
        return std::string();
    }

    mReplyBuffer = mSocket.readAll();

    std::string reply(mReplyBuffer.constData(), mReplyBuffer.size());
    reply.erase(reply.find_last_not_of(" \r\n\t") + 1);
    return reply;
}

void cOusterCmdStream_Qt::errorHandler(QAbstractSocket::SocketError socketError)
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
        std::cerr << "The following error occurred: " << mSocket.errorString().toStdString() << std::endl;
    }

}

