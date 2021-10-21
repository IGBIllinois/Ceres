
#include "OusterImuStream.hpp"

#include <QtNetwork/QHostInfo>
#include <iostream>


cOusterImuStream_Qt::cOusterImuStream_Qt()
:
    mSocket(),
    mDataBuffer()
{}

cOusterImuStream_Qt::~cOusterImuStream_Qt()
{
    if (mSocket.isOpen())
    {
        mSocket.disconnectFromHost();
        mSocket.close();
    }
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

    mSocket.bind(local_endpoint, port);

    return true;
}

void cOusterImuStream_Qt::clear()
{
    while (mSocket.hasPendingDatagrams())
    {
        mDatagram = mSocket.receiveDatagram();
    }
}

void cOusterImuStream_Qt::receive_data()
{
    if (mSocket.waitForReadyRead(0))
        processDatagrams();
}

void cOusterImuStream_Qt::processOneDatagram()
{
    if (!mSocket.hasPendingDatagrams())
        return;

    mDatagram = mSocket.receiveDatagram();
    mSender = mDatagram.senderAddress();
    mDataBuffer = mDatagram.data();
    process_packet(mDataBuffer.data(), mDataBuffer.size());
}

void cOusterImuStream_Qt::processDatagrams()
{
    while (mSocket.hasPendingDatagrams())
    {
        mDatagram = mSocket.receiveDatagram();
        mSender = mDatagram.senderAddress();
        mDataBuffer = mDatagram.data();
        process_packet(mDataBuffer.data(), mDataBuffer.size());
    }
}


