
#include "ExperimentStateRemoteInterface.hpp"


/*******************************************************************/
/**   Interface for Experiment State to Control Remote Sensor     **/
/*******************************************************************/
cExperimentStateRemoteInterface::cExperimentStateRemoteInterface(QObject* parent)
    : QObject(parent), mConnected(false), mpSocket(nullptr), mPort(0)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QAbstractSocket::SocketState>();
}

cExperimentStateRemoteInterface::~cExperimentStateRemoteInterface()
{
    destroy();
}

void cExperimentStateRemoteInterface::destroy()
{
    if (mpSocket)
    {
        if (mpSocket->isOpen())
        {
            mConnected = false;
            mpSocket->disconnectFromHost();
            mpSocket->close();
        }

        QObject::disconnect(mpSocket, &QTcpSocket::connected, this, &cExperimentStateRemoteInterface::connected);
        QObject::disconnect(mpSocket, &QTcpSocket::disconnected, this, &cExperimentStateRemoteInterface::disconnected);
        QObject::disconnect(mpSocket, &QTcpSocket::errorOccurred, this, &cExperimentStateRemoteInterface::errorOccurred);
        QObject::disconnect(mpSocket, &QTcpSocket::hostFound, this, &cExperimentStateRemoteInterface::hostFound);
        QObject::disconnect(mpSocket, &QTcpSocket::stateChanged, this, &cExperimentStateRemoteInterface::stateChanged);
        QObject::disconnect(mpSocket, &QTcpSocket::readyRead, this, &cExperimentStateRemoteInterface::processIncomingData);

        delete mpSocket;
        mpSocket = nullptr;
    }
}

bool cExperimentStateRemoteInterface::initialize(const std::string& hostname, uint16_t port,
    bool use_ipv6, const std::string& local_ip)
{
    mpSocket = new QTcpSocket(this);
    mpSocket->setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
    mpSocket->setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);

    QObject::connect(mpSocket, &QTcpSocket::connected, this, &cExperimentStateRemoteInterface::connected);
    QObject::connect(mpSocket, &QTcpSocket::disconnected, this, &cExperimentStateRemoteInterface::disconnected);
    QObject::connect(mpSocket, &QTcpSocket::errorOccurred, this, &cExperimentStateRemoteInterface::errorOccurred);
    QObject::connect(mpSocket, &QTcpSocket::hostFound, this, &cExperimentStateRemoteInterface::hostFound);
    QObject::connect(mpSocket, &QTcpSocket::stateChanged, this, &cExperimentStateRemoteInterface::stateChanged);
    QObject::connect(mpSocket, &QTcpSocket::readyRead, this, &cExperimentStateRemoteInterface::processIncomingData);

    if (!local_ip.empty())
    {
        mLocalEndpoint = QHostAddress(QString::fromStdString(local_ip));
        mpSocket->bind(mLocalEndpoint, 0);

        mLocalIpAddress = local_ip;
    }

    QHostInfo info = QHostInfo::fromName(QString::fromStdString(hostname));
    auto result = info.error();
    if (result != QHostInfo::NoError)
    {
        return false;
    }

    mHostname = hostname;

    auto endpoints = info.addresses();
    for (auto& endpoint : endpoints)
    {
        if (use_ipv6)
        {
            if (QAbstractSocket::IPv6Protocol != endpoint.protocol())
                continue;
            mRemoteEndpoint = endpoint;
            break;
        }
        else
        {
            if (QAbstractSocket::IPv4Protocol != endpoint.protocol())
                continue;
            mRemoteEndpoint = endpoint;
            break;
        }
    }

    if (mRemoteEndpoint.isNull())
    {
        return false;
    }

    mUse_IpV6 = use_ipv6;
    mPort = port;

    return true;
}

bool cExperimentStateRemoteInterface::openConnection()
{
    if (mRemoteEndpoint.isNull())
    {
        return false;
    }

    mpSocket->connectToHost(mRemoteEndpoint, mPort);
    return true;
}

void cExperimentStateRemoteInterface::closeConnection()
{
    mpSocket->close();
}


/**********************************************************
 * TCP Socket Methods
 *********************************************************/

void cExperimentStateRemoteInterface::connected()
{
    mpSocket->setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
    mConnected = true;

    onConnect();
}

void cExperimentStateRemoteInterface::disconnected()
{
    onDisconnect();
    mConnected = false;
}

void cExperimentStateRemoteInterface::errorOccurred(QAbstractSocket::SocketError socketError)
{
}

void cExperimentStateRemoteInterface::hostFound()
{}

void cExperimentStateRemoteInterface::stateChanged(QAbstractSocket::SocketState socketState)
{
}

void cExperimentStateRemoteInterface::processIncomingData()
{
    QByteArray buffer = mpSocket->readAll();

    if (buffer.isEmpty()) return;

    decodeIncomingData(buffer.constData(), buffer.size());
}

int cExperimentStateRemoteInterface::sendOutgoingData(const char* data, std::size_t len)
{
    if (!mConnected)
        return 0;

    auto n = mpSocket->write(data, len);

    mpSocket->flush();

    return n;
}

