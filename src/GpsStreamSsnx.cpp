
#include "GpsStreamSsnx.hpp"

#include <QtNetwork/QHostInfo>
#include <iostream>


//cGpsStreamSsnx::cGpsStreamSsnx(QObject* parent)
cGpsStreamSsnx::cGpsStreamSsnx()
    :
//    QObject(parent),
//    mSocket(this),
    mSocket(),
    mDataBuffer()
{
}

cGpsStreamSsnx::~cGpsStreamSsnx()
{
    if (mSocket.isOpen())
    {
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}


void cGpsStreamSsnx::registerDataProcessingCallback(std::function<void(const void* pBuffer, std::size_t buf_length)> fp)
{
    mProcessingCallback = fp;
}

bool cGpsStreamSsnx::isConnected() const
{
    auto state = mSocket.state();
    return  (state == QAbstractSocket::ConnectedState) || (state == QAbstractSocket::BoundState) || (state == QAbstractSocket::ListeningState);
}

bool cGpsStreamSsnx::try_to_connect(std::string_view host, uint16_t port, bool use_ipv6)
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

void cGpsStreamSsnx::clear()
{
    while (mSocket.hasPendingDatagrams())
    {
        mDatagram = mSocket.receiveDatagram();
    }
}

void cGpsStreamSsnx::receive_data()
{
    if (mSocket.waitForReadyRead(0))
        processDatagrams();
}

void cGpsStreamSsnx::processOneDatagram()
{
    if (!mSocket.hasPendingDatagrams())
        return;

    mDatagram = mSocket.receiveDatagram();
    mSender = mDatagram.senderAddress();
    mDataBuffer = mDatagram.data();
    processDatagram(mDataBuffer.data(), mDataBuffer.size());
//    emit dataUpdated(mDataBuffer.data(), mDataBuffer.size());
//    if (mProcessingCallback)
//        mProcessingCallback(mDataBuffer.data(), mDataBuffer.size());
}

void cGpsStreamSsnx::processDatagrams()
{
    if (!mSocket.isReadable())
        return;

    while (mSocket.hasPendingDatagrams())
    {
        mDatagram = mSocket.receiveDatagram();
        mSender = mDatagram.senderAddress();
        mDataBuffer = mDatagram.data();
        if (mProcessingCallback)
            mProcessingCallback(mDataBuffer.data(), mDataBuffer.size());
    }
}

