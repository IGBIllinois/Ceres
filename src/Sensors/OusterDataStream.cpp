
#include "OusterDataStream.hpp"

#include <QtNetwork/QHostInfo>
#include <iostream>


cOusterLidarStream_Qt::cOusterLidarStream_Qt()
    :
    cOusterLidarStream(),
    mSocket()
{
//    QObject::connect(&mSocket, &QUdpSocket::readyRead, this, &cOusterLidarStream_Qt::processDatagrams);
}

cOusterLidarStream_Qt::~cOusterLidarStream_Qt()
{
    if (mSocket.isOpen())
    {
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}

bool cOusterLidarStream_Qt::try_to_connect(std::string_view host, uint16_t port, bool use_ipv6)
{
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

    mSocket.bind(local_endpoint, port);

    return true;
}

void cOusterLidarStream_Qt::clear()
{
    while (mSocket.hasPendingDatagrams())
    {
        mDatagram = mSocket.receiveDatagram();
    }
}

void cOusterLidarStream_Qt::receive_data()
{
    if (mSocket.waitForReadyRead(0))
        processDatagrams();
}

void cOusterLidarStream_Qt::processOneDatagram()
{
    if (!mSocket.hasPendingDatagrams())
        return;

    mDatagram = mSocket.receiveDatagram();
    mSender = mDatagram.senderAddress();
    mDataBuffer = mDatagram.data();
    process_packet(mDataBuffer.data(), mDataBuffer.size());
}

void cOusterLidarStream_Qt::processDatagrams()
{
    if (!mSocket.isReadable())
        return;

    while (mSocket.hasPendingDatagrams())
    {
        mDatagram = mSocket.receiveDatagram();
        mSender = mDatagram.senderAddress();
        mDataBuffer = mDatagram.data();
        process_packet(mDataBuffer.data(), mDataBuffer.size());
    }
}

