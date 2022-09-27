
#include "SensorPropertyPage.hpp"

#include <QAction>
#include <QDialogButtonBox>


cSensorPropertyPage::cSensorPropertyPage(QWidget* parent)
:
    QWidget(parent)
{
    mpShowAction = new QAction(this);

    connect(mpShowAction, &QAction::triggered, this, &cSensorPropertyPage::showPage);


    mpButtons = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, this);
}

cSensorPropertyPage::~cSensorPropertyPage()
{
}

void cSensorPropertyPage::setTitle(const QString& text)
{
    setWindowTitle(text);
    mpShowAction->setText(text);
}

void cSensorPropertyPage::showPage()
{
    setTitle("again");
}

QAction* cSensorPropertyPage::showAction() const
{
    return mpShowAction;
}


/*******************************************************************/
/***          Sensor Property Pages for Local Sensor             ***/
/*******************************************************************/
cSensorPropertyPageLocal::cSensorPropertyPageLocal(QWidget* parent)
    : cSensorPropertyPage(parent)
{}


/*******************************************************************/
/***          Sensor Property Pages for Remote Sensor            ***/
/*******************************************************************/

cSensorPropertyPageRemote::cSensorPropertyPageRemote(QWidget* parent)
    :
    mConnected(false), mSocket(parent), mPort(0)
{
    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
    mSocket.setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);

    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QAbstractSocket::SocketState>();

    QObject::connect(&mSocket, &QTcpSocket::connected, this, &cSensorPropertyPageRemote::connected);
    QObject::connect(&mSocket, &QTcpSocket::disconnected, this, &cSensorPropertyPageRemote::disconnected);
    QObject::connect(&mSocket, &QTcpSocket::errorOccurred, this, &cSensorPropertyPageRemote::errorOccurred);
    QObject::connect(&mSocket, &QTcpSocket::hostFound, this, &cSensorPropertyPageRemote::hostFound);
    QObject::connect(&mSocket, &QTcpSocket::stateChanged, this, &cSensorPropertyPageRemote::stateChanged);
    QObject::connect(&mSocket, &QTcpSocket::readyRead, this, &cSensorPropertyPageRemote::processNewCommand);
}

cSensorPropertyPageRemote::~cSensorPropertyPageRemote()
{
    if (mSocket.isOpen())
    {
        mConnected = false;
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}

bool cSensorPropertyPageRemote::initialize(const std::string& hostname, uint16_t port,
    bool use_ipv6, const QString& local_ip)
{
    if (!local_ip.isEmpty())
    {
        mLocalEndpoint = QHostAddress(local_ip);
        mSocket.bind(mLocalEndpoint, 0);
    }

    QHostInfo info = QHostInfo::fromName(QString::fromStdString(hostname));
    if (info.error() != QHostInfo::NoError)
    {
        return false;
    }

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

    mPort = port;
    mSocket.connectToHost(mRemoteEndpoint, port);

    return true;
}


/**********************************************************
 * TCP Socket Methods
 *********************************************************/

void cSensorPropertyPageRemote::connected()
{
    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);

    mConnected = true;
}

void cSensorPropertyPageRemote::disconnected()
{
    mConnected = false;
}

void cSensorPropertyPageRemote::errorOccurred(QAbstractSocket::SocketError socketError)
{
}

void cSensorPropertyPageRemote::hostFound()
{}

void cSensorPropertyPageRemote::stateChanged(QAbstractSocket::SocketState socketState)
{
}

void cSensorPropertyPageRemote::processNewCommand()
{
    QByteArray buffer = mSocket.readAll();

    if (buffer.isEmpty()) return;

//    decode(buffer.constData(), buffer.size());
}

int cSensorPropertyPageRemote::sendOutgoingData(const char* data, std::size_t len)
{
    if (!mConnected)
        return 0;

    auto n = mSocket.write(data, len);

    mSocket.flush();

    return n;
}




