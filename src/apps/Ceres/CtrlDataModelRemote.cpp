
#include "CtrlDataModelRemote.hpp"
#include "RemoteClientView.hpp"
#include "Spidercam/SpidercamModel.hpp"
#include "SensorFactory.hpp"
#include "SensorModel.hpp"
#include "Weather/WeatherDataModel_Http_Wind.hpp"
#include "TimestampProvider.hpp"
#include "SensorPropertyPage.hpp"
#include "ExperimentStateCreator.hpp"

#include <QDockWidget>

//Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
//Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

cCtrlDataModelRemote::cCtrlDataModelRemote(QObject* parent)
:
    cCtrlDataModel(parent),
    cCeresNetEncoder(4096),
    mConnected(false),
    mSocket(parent),
    mpView(nullptr)
{
    mPort = 0;
    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);
    mSocket.setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);

    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QAbstractSocket::SocketState>();

    QObject::connect(&mSocket, &QTcpSocket::connected, this, &cCtrlDataModelRemote::connected);
    QObject::connect(&mSocket, &QTcpSocket::disconnected, this, &cCtrlDataModelRemote::disconnected);
    QObject::connect(&mSocket, &QTcpSocket::errorOccurred, this, &cCtrlDataModelRemote::errorOccurred);
    QObject::connect(&mSocket, &QTcpSocket::hostFound, this, &cCtrlDataModelRemote::hostFound);
    QObject::connect(&mSocket, &QTcpSocket::stateChanged, this, &cCtrlDataModelRemote::stateChanged);
    QObject::connect(&mSocket, &QTcpSocket::readyRead, this, &cCtrlDataModelRemote::processNewCommand);

    mDataFileIsOpen = false;

    mWindSpeedValid = false;
    mWindSpeed_mps = 0.0;
    mWind_dir_deg = 0.0;
}

cCtrlDataModelRemote::~cCtrlDataModelRemote()
{
    mpView->deleteLater();
    mpView = nullptr;
        
    stopDataThread();

    mStateCreators.clear();

    for (auto* page : mPropertyPages)
    {
        page->deleteLater();
    }
    mPropertyPages.clear();

    if (mSocket.isOpen())
    {
        mConnected = false;
        mSocket.disconnectFromHost();
        mSocket.close();
    }
}

void cCtrlDataModelRemote::createView(QDockWidget*& dockWidget)
{
    mpView = new cRemoteClientView(this, dockWidget);

    dockWidget->setWindowTitle(mpView->windowTitle());
    dockWidget->setWidget(mpView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, mpView, &cRemoteClientView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, mpView, &cRemoteClientView::topLevelChanged);
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

bool cCtrlDataModelRemote::systemReady() const
{
    if (!mConnected)
    {
        QString str = "Not connected to the remote computer!\n";
        str += "Please try to reconnect before running an experiment.";

        emit errorMessage("Error", str);
        return false;
    }

    return cCtrlDataModel::systemReady();
}


void cCtrlDataModelRemote::updatePosition(spidercam::sPosition_1_t pos)
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

    mLocalIpAddress = local_ip.toStdString();

    QHostInfo info = QHostInfo::fromName(hostname);
    if (info.error() != QHostInfo::NoError)
    {
        msg = "Could not establish required connection to the C4 computer!";
        msg += info.errorString();
        emit errorMessage("Remote Error", msg);
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
        msg = "Could not resolve the C4 computer information ";
        msg += hostname;
        msg += ".";
        emit errorMessage("Remote Error", msg);
        return false;
    }

    mPort = port;
    mSocket.connectToHost(mRemoteEndpoint, port);

    return true;
}

void cCtrlDataModelRemote::try_reconnection()
{
    mpView->enableReconnectButton(false);

    mSocket.connectToHost(mRemoteEndpoint, mPort);
}

bool cCtrlDataModelRemote::openDataFile(const QString& defaultPath, const std::string&)
{
    std::time_t t = std::time(nullptr);
    tm* ltm = localtime(&t);

    std::string filename;

    switch (ltm->tm_mon)
    {
    case 0: filename = "Jan"; break;
    case 1: filename = "Feb"; break;
    case 2: filename = "Mar"; break;
    case 3: filename = "Apr"; break;
    case 4: filename = "May"; break;
    case 5: filename = "June"; break;
    case 6: filename = "July"; break;
    case 7: filename = "Aug"; break;
    case 8: filename = "Sept"; break;
    case 9: filename = "Oct"; break;
    case 10: filename = "Nov"; break;
    case 11: filename = "Dec"; break;
    }
    filename += std::to_string(ltm->tm_mday);
    filename += "/";
    filename += mExperimentTitle;

    sendOpenDataFile(filename);

    return true;
}

bool cCtrlDataModelRemote::isDataFileOpen() const
{
    return mDataFileIsOpen;
}

void cCtrlDataModelRemote::endDataRecording()
{
    sendStopExperiment();
}

void cCtrlDataModelRemote::closeDataFile()
{
    sendCloseDataFile();
}

void cCtrlDataModelRemote::dataRecordingStateChange(bool record)
{
    if (record)
    {
        sendStartDataRecording();
    }
    else
    {
        sendStopDataRecording();
    }
}

bool cCtrlDataModelRemote::loadExperiment(const std::string& expName, const nlohmann::json& expDoc)
{
    if (!mConnected)
        return false;

    bool result = cCtrlDataModel::loadExperiment(expName, expDoc);

    sendExperimentInfo(mExperimentTitle, mResearcher, mCultivar, mExperimentDoc);

    return result;
}

void cCtrlDataModelRemote::startExperiment()
{
    if (isExperimentRunning())
    {
        mThread.mpController->startExperiment();
        return;
    }

    sendStartExperiment();

    mThread.mpController->startExperiment();

    QString msg = "Running experiment: ";
    msg += QString::fromStdString(mExperimentTitle);
    emit statusMessage(msg);
}

/**********************************************************
 * TCP Socket Methods
 *********************************************************/

void cCtrlDataModelRemote::connected()
{
    QString msg("Connection to C4 established.");
    emit statusMessage(msg);

    mSocket.setSocketOption(QAbstractSocket::SocketOption::LowDelayOption, 1);

    mConnected = true;
}

void cCtrlDataModelRemote::disconnected()
{
    if (isExperimentRunning())
    {
        terminateExperiment();
    }

    if (mConnected)
    {
        QString msg = "Connection to the C4 has been lost!";
        emit errorMessage("Connection Lost", msg);
        mpView->enableReconnectButton(true);
        mpView->removeAllSensors();
    }

    for (auto* creator : mStateCreators)
    {
        mThread.mpController->removeStateCreator(creator);
    }
    mStateCreators.clear();

    for (auto* page : mPropertyPages)
    {
        emit removeSensorPropertyPage(page->showAction());
        page->deleteLater();
    }
    mPropertyPages.clear();

    mDataFileIsOpen = false;
    mConnected = false;
}

void cCtrlDataModelRemote::errorOccurred(QAbstractSocket::SocketError socketError)
{
    /*
        if (mConnected)
        {
            QString msg = "Connection to the C4 has been lost!";
            emit errorMessage("Connection Lost", msg);
        }

        mConnected = false;
    */
}

void cCtrlDataModelRemote::hostFound()
{}

void cCtrlDataModelRemote::stateChanged(QAbstractSocket::SocketState socketState)
{
    if (mpView)
    {
        switch (socketState)
        {
        case QAbstractSocket::SocketState::BoundState:
            mpView->setConnectionStatus("Bound");
            break;
        case QAbstractSocket::SocketState::ClosingState:
            mpView->setConnectionStatus("Closing");
            break;
        case QAbstractSocket::SocketState::ConnectedState:
            mpView->setConnectionStatus("Connected");
            break;
        case QAbstractSocket::SocketState::HostLookupState:
            mpView->setConnectionStatus("Host Lookup");
            break;
        case QAbstractSocket::SocketState::ListeningState:
            mpView->setConnectionStatus("Listening");
            break;
        case QAbstractSocket::SocketState::UnconnectedState:
            mpView->enableReconnectButton(true);
            break;
        }
    }
}

void cCtrlDataModelRemote::processNewCommand()
{
    QByteArray buffer = mSocket.readAll();

    if (buffer.isEmpty()) return;

    decode(buffer.constData(), buffer.size());
}

int cCtrlDataModelRemote::sendOutgoingData(const char* data, std::size_t len)
{
    if (!mConnected)
        return 0;

    auto n = mSocket.write(data, len);

    mSocket.flush();

    return n;
}


/**********************************************************
 * Packet Handlers
 *********************************************************/
void cCtrlDataModelRemote::onDataFileState(bool is_open)
{
    mDataFileIsOpen = is_open;
}

void cCtrlDataModelRemote::onStatusMessage(const std::string& msg)
{
    mpView->updateStatusMsg(QString::fromStdString(msg));
}

void cCtrlDataModelRemote::onLogMessage(uint8_t msg_type, const std::string& device, const std::string& msg)
{
    mpView->updateLogMsg(msg_type, QString::fromStdString(device), QString::fromStdString(msg));
}

void cCtrlDataModelRemote::onSensorStatus(const std::string& sensor, const std::string& status)
{
    mpView->updateSensorStatus(QString::fromStdString(sensor),
        QString::fromStdString(status));
}

void cCtrlDataModelRemote::onSensorNameChange(const std::string& old_name, const std::string& new_name)
{
    auto name = QString::fromStdString(old_name);
    mpView->sensorNameChange(name, QString::fromStdString(new_name));

    for (auto* page : mPropertyPages)
    {
        if (page->windowTitle() == name)
        {
            page->setTitle(QString::fromStdString(new_name));
            break;
        }
    }

}

void cCtrlDataModelRemote::onSensorPropertyConnectInfo(const std::string& sensor, uint32_t version,
    const std::string& name, const std::string& ip_address, uint16_t port)
{
    cSensorPropertyPage* page = create_sensor_property_page(sensor, version, ip_address, port, mLocalIpAddress);

    if (!page) return;

    page->setTitle(QString::fromStdString(name));

    mPropertyPages.push_back(page);

    emit addSensorPropertyPage(page->showAction());

    cExperimentStateCreator* creator = dynamic_cast<cExperimentStateCreator*>(page);

    if (creator)
    {
        mThread.mpController->addStateCreator(creator);
        mStateCreators.push_back(creator);
    }
}

