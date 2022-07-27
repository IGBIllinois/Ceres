
#include "OusterDataStream.hpp"

#include <QtNetwork/QHostInfo>
#include <iostream>


cOusterLidarStream_Qt::cOusterLidarStream_Qt(QObject* parent)
:
    cOusterLidarStream(),
    mpSocket(nullptr)
{
}

cOusterLidarStream_Qt::~cOusterLidarStream_Qt()
{
    stopCommunications();
}

bool cOusterLidarStream_Qt::determineLocalEndpoint(std::string_view sensor, uint16_t port, bool use_ipv6)
{
    QHostInfo info = QHostInfo::fromName(QString(sensor.data()));
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

    mLocalEndpoint = local_endpoint;

    return true;
}

bool cOusterLidarStream_Qt::startCommunications(std::string_view sensor, uint16_t port, bool use_ipv6)
{
    if (mpSocket) return true;

    mpSocket = new QUdpSocket();

    return cOusterLidarStream::connect_to_sensor(sensor, port, use_ipv6);
}

void cOusterLidarStream_Qt::stopCommunications()
{
    if (mpSocket && mpSocket->isOpen())
    {
        mpSocket->disconnectFromHost();
        mpSocket->close();
    }

    delete mpSocket; mpSocket = nullptr;
}

bool cOusterLidarStream_Qt::try_to_connect(std::string_view host, uint16_t port, bool use_ipv6)
{
/*
    QHostInfo info = QHostInfo::fromName(QString(host.data()));
    if (info.error() != QHostInfo::NoError)
    {
        std::cerr << info.errorString().toStdString() << std::endl;
        return false;
    }

    QHostAddress local_endpoint;

    auto endpoints = info.addresses();
    for(auto & endpoint : endpoints)
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
*/

    return mpSocket->bind(mLocalEndpoint, port);

//    return true;
}

void cOusterLidarStream_Qt::clear()
{
    while (mpSocket->hasPendingDatagrams())
    {
        mDatagram = mpSocket->receiveDatagram();
    }
}

void cOusterLidarStream_Qt::receive_data()
{
    if (mpSocket->waitForReadyRead(0))
        processDatagrams();
}

void cOusterLidarStream_Qt::processOneDatagram()
{
    if (!mpSocket->hasPendingDatagrams())
        return;

    mDatagram = mpSocket->receiveDatagram();
    mSender = mDatagram.senderAddress();
    mDataBuffer = mDatagram.data();
    process_packet(mDataBuffer.data(), mDataBuffer.size());
}

void cOusterLidarStream_Qt::processDatagrams()
{
    if (!mpSocket->isReadable())
        return;

    while (mpSocket->hasPendingDatagrams())
    {
        mDatagram = mpSocket->receiveDatagram();
        mSender = mDatagram.senderAddress();
        mDataBuffer = mDatagram.data();
        process_packet(mDataBuffer.data(), mDataBuffer.size());
    }
}

