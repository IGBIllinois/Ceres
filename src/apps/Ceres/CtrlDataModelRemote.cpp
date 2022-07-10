
#include "CtrlDataModelRemote.hpp"
#include "Spidercam/SpidercamModel.hpp"
#include "SensorModel.hpp"
#include "Weather/WeatherDataModel_Http_Wind.hpp"

#include <QtNetwork/QHostInfo>

//Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
//Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

cCtrlDataModelRemote::cCtrlDataModelRemote(QObject* parent)
:
    cCtrlDataModel(parent),
    cCeresNetEncoder(4096),
    mConnected(false),
    mSocket(parent)
{
    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
    mSocket.setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);

    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QAbstractSocket::SocketState>();

    QObject::connect(&mSocket, &QTcpSocket::connected, this, &cCtrlDataModelRemote::connected);
    QObject::connect(&mSocket, &QTcpSocket::disconnected, this, &cCtrlDataModelRemote::disconnected);
    QObject::connect(&mSocket, &QTcpSocket::errorOccurred, this, &cCtrlDataModelRemote::errorOccurred);
    QObject::connect(&mSocket, &QTcpSocket::hostFound, this, &cCtrlDataModelRemote::hostFound);
    QObject::connect(&mSocket, &QTcpSocket::stateChanged, this, &cCtrlDataModelRemote::stateChanged);

    mDataFileIsOpen = false;

    mWindSpeedValid = false;
    mWindSpeed_mps = 0.0;
    mWind_dir_deg = 0.0;

}

cCtrlDataModelRemote::~cCtrlDataModelRemote()
{
    stopDataThread();

    if (mSocket.isOpen())
    {
        mConnected = false;
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}

void cCtrlDataModelRemote::addExperimentControlModel(cExperimentControlModel* pModel)
{
    cCtrlDataModel::addExperimentControlModel(pModel);

    auto* pSpidercam = dynamic_cast<cSpidercamModel*>(pModel);

    if (pSpidercam)
    {
        sendSpidercamPosition(pSpidercam->currentPosition());

        QObject::connect(pSpidercam, &cSpidercamModel::positionChanged, this, &cCtrlDataModelRemote::updatePosition);
    }
}

void cCtrlDataModelRemote::addSensor(cSensorModel* pSensor)
{
    cCtrlDataModel::addSensor(pSensor);

    auto* pWeather = dynamic_cast<cWeatherDataModel_Http_Wind*>(pSensor);

    if (pWeather)
    {
        if (pWeather->windDataValid())
        {
            mWindSpeedValid = true;
            mWindSpeed_mps = pWeather->windSpeed_mps();
            mWind_dir_deg = pWeather->windDirection_deg();

            sendWeatherData(mWindSpeedValid, mWindSpeed_mps, mWind_dir_deg);
        }

        QObject::connect(pWeather, &cWeatherDataModel_Http_Wind::windDataChanged, this, &cCtrlDataModelRemote::updateWindData);
    }
}

void cCtrlDataModelRemote::updatePosition(spidercam::sPosition pos)
{
    if (!mConnected) return;

    sendSpidercamPosition(pos);
}

void cCtrlDataModelRemote::updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg)
{
    mWindSpeedValid = valid_wind_speed;
    mWindSpeed_mps = wind_speed_mps;
    mWind_dir_deg = wind_dir_deg;

    if (!mConnected) return;

    sendWeatherData(mWindSpeedValid, mWindSpeed_mps, mWind_dir_deg);
}

void cCtrlDataModelRemote::connected()
{
    QString msg("Connection to C4 established.");
    emit statusMessage(msg);

    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);

    mConnected = true;
}

void cCtrlDataModelRemote::disconnected()
{
    if (mConnected)
    {
        QString msg = "Connection to the C4 has been lost!";
        emit errorMessage("Connection Lost", msg);
    }

    mConnected = false;
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
    QString msg("Trying to connect to C4 at ");
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
    sendOpenDataFile(mExperimentTitle);

    return true;
}

bool cCtrlDataModelRemote::isDataFileOpen() const
{
    return mDataFileIsOpen;
}

void cCtrlDataModelRemote::endDataRecording()
{

}

void cCtrlDataModelRemote::closeDataFile()
{
}

bool cCtrlDataModelRemote::loadExperiment(const nlohmann::json& expDoc)
{
    if (!mConnected)
        return false;

    bool result = cCtrlDataModel::loadExperiment(expDoc);

    sendExperimentInfo(mExperimentTitle, mResearcher, mCultivar, mExperimentDoc);

    return result;
}

void cCtrlDataModelRemote::startExperiment()
{
}

/**********************************************************
 * Packet Handlers
 *********************************************************/
void cCtrlDataModelRemote::dataFileState(bool is_open)
{
    mDataFileIsOpen = is_open;
}

int cCtrlDataModelRemote::sendOutgoingData(const char* data, std::size_t len)
{
    if (!mConnected)
        return 0;

    auto n = mSocket.write(data, len);

    mSocket.flush();

    return n;
}


