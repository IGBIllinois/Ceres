
#include "OusterCmdStream.hpp"

#include <QtNetwork/QHostInfo>

namespace
{
    std::pair<int, int> test_json(const std::string& buffer)
    {
        int open = 0;
        int close = 0;

        for (auto c : buffer)
        {
            if (c == '{')
                ++open;
            else if (c == '}')
                ++close;
        }

        return { open, close };
    }
}

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

void cOusterCmdStream_Qt::enableSocketLogging()
{
    if (mSocketLog.is_open())
    {
        mSocketLogging = true;
        return;
    }
        
    mSocketLog.open("OusterCmdStream_Qt.txt");
    if (!mSocketLog.is_open())
    {
        mSocketLogging = false;
        return;
    }

    mSocketLogging = true;
}

void cOusterCmdStream_Qt::disableSocketLogging()
{
    mSocketLog.close();
    mSocketLogging = false;
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

    if (mSocketLogging)
    {
        auto buf_size = mSocket.readBufferSize();
        mSocketLog << "Ouster Cmd Stream Qt Read Buffer Size: " << buf_size << std::endl;
    }

    return true;
}

int cOusterCmdStream_Qt::send_cmd(const std::string_view msg)
{
    if (mSocketLogging)
    {
        mSocketLog << "Tx: " << msg << std::endl;
    }

    auto len = mSocket.write(msg.data(), msg.size());
    mSocket.flush();
    mSocket.waitForBytesWritten();
    return len;
}

bool cOusterCmdStream_Qt::recv_data_available()
{
    return (mSocket.bytesAvailable() > 0);
}

std::string cOusterCmdStream_Qt::recv_reply()
{
    if (!mSocket.waitForReadyRead())
    {
        return std::string();
    }

    mReplyBuffer = mSocket.readAll();

    //qInfo() << "Reply Buffer: " << mReplyBuffer;

    std::string reply(mReplyBuffer.constData(), mReplyBuffer.size());
    reply.erase(reply.find_last_not_of(" \r\n\t") + 1);

    mReplyBuffer.clear();

    if (mSocketLogging)
    {
        mSocketLog << "Rx: " << reply << std::endl;
    }

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

    if (mSocketLogging)
    {
        mSocketLog << "Json Rx: " << reply << std::endl;
    }

    return reply;
}

void cOusterCmdStream_Qt::flush()
{
    while (mSocket.bytesAvailable() > 0)
    {
        mReplyBuffer = mSocket.readAll();

        if (mSocketLogging)
        {
            std::string reply;
            reply.append(mReplyBuffer.constData(), mReplyBuffer.size());

            mSocketLog << "Flush: " << reply << std::endl;
        }
    }

    mReplyBuffer.clear();
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

