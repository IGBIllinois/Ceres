
#include "SpidercamCtrl.hpp"


//Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
//Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

cSpidercamController::cSpidercamController(QObject* parent)
    : QObject(parent),
        mpSocket(nullptr)
{
    mPort = 0;
}

cSpidercamController::~cSpidercamController()
{
    stopCommunications();
}

bool cSpidercamController::sendRequestNewPosition(
    double x_mm, double y_mm, double z_mm, double height_mm,
    uint32_t speed_mmps, float pan_deg, float tilt_deg, float roll_deg)
{
    if (speed_mmps == 0)
    {
        mVx_mmps = 0.0;
        mVy_mmps = 0.0;
        mVz_mmps = 0.0;
    }
    else
    {
        auto pos = getLastKnownPosition();

        double dx = x_mm - pos.X_mm;
        double dy = y_mm - pos.Y_mm;
        double dz = z_mm - pos.Z_mm;

        double d = sqrt(dx * dx + dy * dy + dz * dz);
        double t_sec = d / static_cast<double>(speed_mmps);

        mVx_mmps = dx / t_sec;
        mVy_mmps = dx / t_sec;
        mVz_mmps = dx / t_sec;
    }

    return cSpiderCamCom::sendRequestNewPosition(
        x_mm, y_mm, z_mm, height_mm,
        speed_mmps, pan_deg, tilt_deg, roll_deg);
}

bool cSpidercamController::hasRemoteEndpoint() const
{
    return !mRemoteEndpoint.isNull();
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
    return mpSocket && mpSocket->isValid() && (mpSocket->state() == QAbstractSocket::ConnectedState);
}


bool cSpidercamController::try_to_connect(std::string_view hostname, uint16_t port,
                                            std::string_view local_ip, bool use_ipv6)
{
    if (!local_ip.empty())
    {
        mLocalEndpoint = QHostAddress(std::string(local_ip).c_str());
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

    // Test connection parameters
    QTcpSocket socket;
 
    if (!mLocalEndpoint.isNull())
    {
        socket.bind(mLocalEndpoint);
    }

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

    mpSocket = new QTcpSocket(this);

    QObject::connect(mpSocket, &QTcpSocket::connected, this, &cSpidercamController::onConnect);
    QObject::connect(mpSocket, &QTcpSocket::disconnected, this, &cSpidercamController::onDisconnect);
    QObject::connect(mpSocket, &QTcpSocket::errorOccurred, this, &cSpidercamController::errorHandler);

    if (!mLocalEndpoint.isNull())
    {
        mpSocket->bind(mLocalEndpoint);
    }

    mpSocket->connectToHost(mRemoteEndpoint, mPort);

    return mpSocket->waitForConnected();
}

void cSpidercamController::stopCommunications()
{
    if (mpSocket && mpSocket->isOpen())
    {
        QObject::disconnect(mpSocket, &QTcpSocket::connected, this, &cSpidercamController::onConnect);
        QObject::disconnect(mpSocket, &QTcpSocket::disconnected, this, &cSpidercamController::onDisconnect);
        QObject::disconnect(mpSocket, &QTcpSocket::errorOccurred, this, &cSpidercamController::errorHandler);

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

void cSpidercamController::onConnect()
{
    emit connectionStateChange(true);
}

void cSpidercamController::onDisconnect()
{
    emit connectionStateChange(false);
}

void cSpidercamController::errorHandler(QAbstractSocket::SocketError socketError)
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
        qCritical() << "The following error occurred: " << mpSocket->errorString();
    }

}

