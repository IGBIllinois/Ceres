
#include "CtrlDataModelRemote.hpp"
#include "Sensors/SensorModel.hpp"

#include <QtNetwork/QHostInfo>

//Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
//Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

cCtrlDataModelRemote::cCtrlDataModelRemote(QObject* parent)
:
    cCtrlDataModel(parent),
    mConnected(false),
    mSocket(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QAbstractSocket::SocketState>();

    QObject::connect(&mSocket, &QTcpSocket::connected, this, &cCtrlDataModelRemote::connected);
    QObject::connect(&mSocket, &QTcpSocket::disconnected, this, &cCtrlDataModelRemote::disconnected);
    QObject::connect(&mSocket, &QTcpSocket::errorOccurred, this, &cCtrlDataModelRemote::errorOccurred);
    QObject::connect(&mSocket, &QTcpSocket::hostFound, this, &cCtrlDataModelRemote::hostFound);
    QObject::connect(&mSocket, &QTcpSocket::stateChanged, this, &cCtrlDataModelRemote::stateChanged);
}

cCtrlDataModelRemote::~cCtrlDataModelRemote()
{
    stopDataThread();

    if (mSocket.isOpen())
    {
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}

void cCtrlDataModelRemote::connected()
{
    QString msg("Connection to C4 established.");
    emit statusMessage(msg);

    mConnected = true;
}

void cCtrlDataModelRemote::disconnected()
{
    mConnected = false;
    
    QString msg = "Connection to the C4 has been lost!";
    emit errorMessage("Connection Lost", msg);
}

void cCtrlDataModelRemote::errorOccurred(QAbstractSocket::SocketError socketError)
{
    if (mConnected)
    {
        QString msg = "Connection to the C4 has been lost!";
        emit errorMessage("Connection Lost", msg);
    }

    mConnected = false;
}

void cCtrlDataModelRemote::hostFound()
{}

void cCtrlDataModelRemote::stateChanged(QAbstractSocket::SocketState socketState)
{

}

bool cCtrlDataModelRemote::try_to_connect(const QString& hostname, uint16_t port, 
                                            bool use_ipv6, const QString& local_ip)
{
    QString msg("Testing connection to remote computer at ");
    msg.append(hostname);
    msg.append("...");

    emit statusMessage(msg);

    if (!local_ip.isEmpty())
    {
        QHostAddress local_endpoint(local_ip);
        mSocket.bind(local_endpoint,0);
    }

    QHostInfo info = QHostInfo::fromName(hostname);
    if (info.error() != QHostInfo::NoError)
    {
        msg = "Could not establish required connection to the C4 computer!";
        msg += info.errorString();
        emit errorMessage("Remote Error", msg);
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
    {
        msg = "Could not resolve the C4 computer information ";
        msg += hostname;
        msg += ".";
        emit errorMessage("Remote Error", msg);
        return false;
    }

    mSocket.connectToHost(remote_endpoint, port);

    return true;

}

bool cCtrlDataModelRemote::openDataFile(const QString& defaultPath)
{
    return false;
}

void cCtrlDataModelRemote::closeDataFile()
{
}

void cCtrlDataModelRemote::startExperiment()
{
}



