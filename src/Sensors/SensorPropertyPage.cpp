
#include "SensorPropertyPage.hpp"

#include <QAction>
#include <QDialogButtonBox>
#include <QAbstractButton>


cSensorPropertyPage::cSensorPropertyPage(QWidget* parent)
:
    QDialog(parent)
{
    mpShowAction = new QAction(this);

    connect(mpShowAction, &QAction::triggered, this, &cSensorPropertyPage::showPage);

    mpButtons = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, this);

    connect(mpButtons, &QDialogButtonBox::clicked, this, &cSensorPropertyPage::buttonClicked);
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
    show();
}

QAction* cSensorPropertyPage::showAction() const
{
    return mpShowAction;
}

void cSensorPropertyPage::doOK()
{
    hide();
}

void cSensorPropertyPage::doCancel()
{
    hide();
}

void cSensorPropertyPage::doApply()
{

}

void cSensorPropertyPage::buttonClicked(QAbstractButton* button)
{
    auto text = button->text().toStdString();
    if (text == "OK")
    {
        doOK();
        return;
    }
    if (text == "Apply")
    {
        doApply();
        return;
    }
    if (text == "Cancel")
    {
        doCancel();
        return;
    }
}

/*******************************************************************/
/** Interface for Remote Sensor Property Pages for Remote Sensor  **/
/*******************************************************************/
cSensorPropertyPageRemoteInterface::cSensorPropertyPageRemoteInterface(QObject* parent)
    : QObject(parent), mConnected(false), mSocket(this), mPort(0)
{
    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
    mSocket.setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);

    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QAbstractSocket::SocketState>();

    QObject::connect(&mSocket, &QTcpSocket::connected, this, &cSensorPropertyPageRemoteInterface::connected);
    QObject::connect(&mSocket, &QTcpSocket::disconnected, this, &cSensorPropertyPageRemoteInterface::disconnected);
    QObject::connect(&mSocket, &QTcpSocket::errorOccurred, this, &cSensorPropertyPageRemoteInterface::errorOccurred);
    QObject::connect(&mSocket, &QTcpSocket::hostFound, this, &cSensorPropertyPageRemoteInterface::hostFound);
    QObject::connect(&mSocket, &QTcpSocket::stateChanged, this, &cSensorPropertyPageRemoteInterface::stateChanged);
    QObject::connect(&mSocket, &QTcpSocket::readyRead, this, &cSensorPropertyPageRemoteInterface::processIncomingData);
}

cSensorPropertyPageRemoteInterface::~cSensorPropertyPageRemoteInterface()
{
    if (mSocket.isOpen())
    {
        mConnected = false;
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}

bool cSensorPropertyPageRemoteInterface::initialize(const std::string& hostname, uint16_t port,
    bool use_ipv6, const std::string& local_ip)
{
    if (!local_ip.empty())
    {
        mLocalEndpoint = QHostAddress(QString::fromStdString(local_ip));
        mSocket.bind(mLocalEndpoint, 0);

        mLocalIpAddress = local_ip;
    }

    QHostInfo info = QHostInfo::fromName(QString::fromStdString(hostname));
    if (info.error() != QHostInfo::NoError)
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

bool cSensorPropertyPageRemoteInterface::openConnection()
{
    if (mRemoteEndpoint.isNull())
    {
        return false;
    }

    mSocket.connectToHost(mRemoteEndpoint, mPort);
    return true;
}

void cSensorPropertyPageRemoteInterface::closeConnection()
{
    mSocket.close();
}


/**********************************************************
 * TCP Socket Methods
 *********************************************************/

void cSensorPropertyPageRemoteInterface::connected()
{
    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
    mConnected = true;

    onConnect();
}

void cSensorPropertyPageRemoteInterface::disconnected()
{
    onDisconnect();
    mConnected = false;
}

void cSensorPropertyPageRemoteInterface::errorOccurred(QAbstractSocket::SocketError socketError)
{
}

void cSensorPropertyPageRemoteInterface::hostFound()
{}

void cSensorPropertyPageRemoteInterface::stateChanged(QAbstractSocket::SocketState socketState)
{
}

void cSensorPropertyPageRemoteInterface::processIncomingData()
{
    QByteArray buffer = mSocket.readAll();

    if (buffer.isEmpty()) return;

    decodeIncomingData(buffer.constData(), buffer.size());
}

int cSensorPropertyPageRemoteInterface::sendOutgoingData(const char* data, std::size_t len)
{
    if (!mConnected)
        return 0;

    auto n = mSocket.write(data, len);

    mSocket.flush();

    return n;
}




