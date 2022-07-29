
#include "OusterCmdStream.hpp"

#include <QtNetwork/QHostInfo>


cOusterCmdStream_Qt::cOusterCmdStream_Qt(QObject* parent)
    :
        mSocket(parent)
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

bool cOusterCmdStream_Qt::try_to_connect(std::string_view hostname, uint16_t port, 
                                        bool use_ipv6, std::string_view local_ip)
{
    if (!local_ip.empty())
    {
        QHostAddress local_endpoint(std::string(local_ip).c_str());
        mSocket.bind(local_endpoint);
    }

    QHostInfo info = QHostInfo::fromName(QString(hostname.data()));
    if (info.error() != QHostInfo::NoError)
    {
        qCritical() << info.errorString();
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

    mSocket.connectToHost(remote_endpoint, port);
    mSocket.waitForConnected();

    auto buf_size = mSocket.readBufferSize();
    qInfo() << "Read Buffer Size: " << buf_size;

    return true;
}

int cOusterCmdStream_Qt::send_cmd(const std::string_view msg)
{
    auto len = mSocket.write(msg.data(), msg.size());
//    mSocket.flush();
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

    mReplyBuffer.clear();

    return reply;
}

std::string cOusterCmdStream_Qt::recv_json_reply()
{
    if (!mSocket.waitForReadyRead())
    {
        return std::string();
    }

    std::string reply;

    do
    {
        mReplyBuffer = mSocket.readAll();

        reply.append(mReplyBuffer.constData(), mReplyBuffer.size());

        auto pos = reply.find_last_not_of(" \r\n\t");
        if (pos != std::string::npos)
            reply.erase(pos + 1);

        mReplyBuffer.clear();

        if (0 == mSocket.bytesAvailable())
            break;

    } while (reply.back() != '}');

    return reply;
}

void cOusterCmdStream_Qt::errorHandler(QAbstractSocket::SocketError socketError)
{
    switch (socketError) 
    {
    case QAbstractSocket::RemoteHostClosedError:
        qCritical() << "Remote host closed connection!";
        break;
    case QAbstractSocket::HostNotFoundError:
        qCritical() << "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qCritical() << "The connection was refused by the peer. "
                    << "Make sure the fortune server is running, "
                    << "and check that the host name and port "
                    << "settings are correct.";
        break;
    default:
        qCritical() << "The following error occurred: " << mSocket.errorString();
    }

}

