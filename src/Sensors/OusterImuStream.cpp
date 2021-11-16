
#include "OusterImuStream.hpp"

#include <QtNetwork/QHostInfo>
#include <iostream>


cOusterImuStream_Qt::cOusterImuStream_Qt()
:
    mpSocket(nullptr),
    mDataBuffer()
{}

cOusterImuStream_Qt::~cOusterImuStream_Qt()
{
    stopCommunications();
}

bool cOusterImuStream_Qt::startCommunications(std::string_view sensor, uint16_t port, bool use_ipv6)
{
    if (mpSocket) return true;

    mpSocket = new QUdpSocket();

    return cOusterImuStream::connect_to_sensor(sensor, port, use_ipv6);
}

void cOusterImuStream_Qt::stopCommunications()
{
    if (mpSocket && mpSocket->isOpen())
    {
        mpSocket->disconnectFromHost();
        mpSocket->close();
    }

    delete mpSocket; mpSocket = nullptr;
}

bool cOusterImuStream_Qt::try_to_connect(std::string_view host, uint16_t port, bool use_ipv6)
{
    QHostInfo info = QHostInfo::fromName(QString(host.data()));
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

    mpSocket->bind(local_endpoint, port);

    return true;
}

void cOusterImuStream_Qt::clear()
{
    while (mpSocket->hasPendingDatagrams())
    {
        mDatagram = mpSocket->receiveDatagram();
    }
}

void cOusterImuStream_Qt::receive_data()
{
    if (mpSocket->waitForReadyRead(0))
        processDatagrams();
}

void cOusterImuStream_Qt::processOneDatagram()
{
    if (!mpSocket->hasPendingDatagrams())
        return;

    mDatagram = mpSocket->receiveDatagram();
    mSender = mDatagram.senderAddress();
    mDataBuffer = mDatagram.data();
    process_packet(mDataBuffer.data(), mDataBuffer.size());
}

void cOusterImuStream_Qt::processDatagrams()
{
    while (mpSocket->hasPendingDatagrams())
    {
        mDatagram = mpSocket->receiveDatagram();
        mSender = mDatagram.senderAddress();
        mDataBuffer = mDatagram.data();
        process_packet(mDataBuffer.data(), mDataBuffer.size());
    }
}


