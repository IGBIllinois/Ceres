
#include "CtrlDataModelRemote.hpp"
#include "Sensors/SensorModel.hpp"

#include <QtNetwork/QHostInfo>


cCtrlDataModelRemote::cCtrlDataModelRemote(QObject* parent)
:
    cCtrlDataModel(parent),
    mSocket(parent)
{
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

}

void cCtrlDataModelRemote::disconnected()
{

}

void cCtrlDataModelRemote::errorOccurred(QAbstractSocket::SocketError socketError)
{

}

void cCtrlDataModelRemote::hostFound()
{

}

void cCtrlDataModelRemote::stateChanged(QAbstractSocket::SocketState socketState);

bool cCtrlDataModelRemote::try_to_connect(const QString& hostname, uint16_t port, bool use_ipv6)
{
    QHostInfo info = QHostInfo::fromName(hostname);
    if (info.error() != QHostInfo::NoError)
    {
        // qFatal() << info.errorString();
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



