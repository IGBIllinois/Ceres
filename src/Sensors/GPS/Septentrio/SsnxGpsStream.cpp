
#include "SsnxGpsStream.hpp"

#include <QtNetwork/QHostInfo>
#include <QtCore>
#include <QtLogging>


cSsnxGpsStream::cSsnxGpsStream()
    :
    mpSocket(nullptr),
    mDataBuffer()
{
    mPort = 0;
}

cSsnxGpsStream::~cSsnxGpsStream()
{
    stopCommunications();
}


void cSsnxGpsStream::registerDataProcessingCallback(std::function<void(const void* pBuffer, std::size_t buf_length)> fp)
{
    mProcessingCallback = fp;
}

bool cSsnxGpsStream::isConnected() const
{
    if (!mpSocket) return false;

    auto state = mpSocket->state();
    return  (state == QAbstractSocket::ConnectedState) || (state == QAbstractSocket::BoundState) || (state == QAbstractSocket::ListeningState);
}

bool cSsnxGpsStream::try_to_connect(std::string_view host, uint16_t port, bool use_ipv6)
{
    QHostInfo info = QHostInfo::fromName(QString(host.data()));
    if (info.error() != QHostInfo::NoError)
    {
        qCritical() << info.errorString();
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

    QUdpSocket  socket;

    if (socket.bind(local_endpoint, port))
    {
        mLocalEndpoint = local_endpoint;
        mPort = port;
    }

    return true;
}

bool cSsnxGpsStream::startCommunications()
{
    if (mpSocket) return true;

    mpSocket = new QUdpSocket();
    return mpSocket->bind(mLocalEndpoint, mPort);
}

void cSsnxGpsStream::stopCommunications()
{
    if (mpSocket && mpSocket->isOpen())
    {
        mpSocket->disconnectFromHost();
        mpSocket->close();
    }

    delete mpSocket; mpSocket = nullptr;
}

void cSsnxGpsStream::clear()
{
    if (!mpSocket) return;

    while (mpSocket->hasPendingDatagrams())
    {
        mDatagram = mpSocket->receiveDatagram();
    }
}

void cSsnxGpsStream::receive_data()
{
    if (mpSocket->waitForReadyRead(0))
        processDatagrams();
}

void cSsnxGpsStream::processOneDatagram()
{
    if (!mpSocket->hasPendingDatagrams())
        return;

    mDatagram = mpSocket->receiveDatagram();
    mSender = mDatagram.senderAddress();
    mDataBuffer = mDatagram.data();
    processDatagram(mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxGpsStream::processDatagrams()
{
    if (!mpSocket->isReadable())
        return;

    while (mpSocket->hasPendingDatagrams())
    {
        mDatagram = mpSocket->receiveDatagram();
        mSender = mDatagram.senderAddress();
        mDataBuffer = mDatagram.data();
        if (mProcessingCallback)
            mProcessingCallback(mDataBuffer.data(), mDataBuffer.size());
    }
}

