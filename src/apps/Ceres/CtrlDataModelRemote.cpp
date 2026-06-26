
#include "CtrlDataModelRemote.hpp"
#include "RemoteClientView.hpp"
#include "Spidercam/SpidercamModel.hpp"
#include "SensorFactory.hpp"
#include "SensorModel.hpp"
#include "Weather/WeatherDataModel_Http_Wind.hpp"
#include "Weather/WeatherDataModel_Http_Wind_T_RH_PAR.hpp"
#include "TimestampProvider.hpp"
#include "SensorPropertyPage.hpp"
#include "ExperimentStateCreator.hpp"
#include "MarkerExperimentStates.hpp"
#include "MissingSensorsDlg.hpp"

#include <QDockWidget>
#include <QTime>
#include <QCoreApplication>
#include <QAbstractEventDispatcher>
#include <QMessageBox>


//Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
//Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

cCtrlDataModelRemote::cCtrlDataModelRemote(QObject* parent)
:
    cCtrlDataModel(parent),
    cCeresNetEncoder(8192),
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
    mTemperature_C = 0.0;
    mRH_pct = 0.0;
    mPAR_umole = 0.0;
}

cCtrlDataModelRemote::~cCtrlDataModelRemote()
{
    mpView->deleteLater();
    mpView = nullptr;
        
    stopDataThread();

//    mStateCreators.clear();

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

    auto* pWind = dynamic_cast<cWeatherDataModel_Http_Wind*>(pSensor);

    if (pWind)
    {
        if (pWind->windDataValid())
        {
            mWindSpeedValid = true;
            mWindSpeed_mps = pWind->windSpeed_mps();
            mWind_dir_deg = pWind->windDirection_deg();

            sendWindData(mWindSpeedValid, mWindSpeed_mps, mWind_dir_deg);
        }

        QObject::connect(pWind, &cWeatherDataModel_Http_Wind::windDataChanged, this, &cCtrlDataModelRemote::updateWindData);
    }

    auto* pWind_T_RH_PAR = dynamic_cast<cWeatherDataModel_Http_Wind_T_RH_PAR*>(pSensor);

    if (pWind_T_RH_PAR)
    {
/*
        if (pWind_T_RH_PAR->windDataValid())
        {
            mWindSpeedValid = true;
            mWindSpeed_mps = pWind_T_RH_PAR->windSpeed_mps();
            mWind_dir_deg = pWind_T_RH_PAR->windDirection_deg();

            sendWindData(mWindSpeedValid, mWindSpeed_mps, mWind_dir_deg);
        }
*/

        mWindSpeedValid = pWind_T_RH_PAR->windDataValid();
        mWindSpeed_mps = pWind_T_RH_PAR->windSpeed_mps();
        mWind_dir_deg = pWind_T_RH_PAR->windDirection_deg();

        mTemperature_C = pWind_T_RH_PAR->temperature_C();
        mRH_pct = pWind_T_RH_PAR->relativeHumidity_pct();
        mPAR_umole = pWind_T_RH_PAR->par_umole();

/*
        encodeTemperatureData(mTemperature_C);
        encodeRelativeHumidityData(mRH_pct);
        encodeParData(mPAR_umole);
        sendData();
*/
        sendWeatherData(mWindSpeedValid, mWindSpeed_mps, mWind_dir_deg, mTemperature_C, mRH_pct, mPAR_umole);

        QObject::connect(pWind_T_RH_PAR, &cWeatherDataModel_Http_Wind_T_RH_PAR::windDataChanged, this, &cCtrlDataModelRemote::updateWindData);
        QObject::connect(pWind_T_RH_PAR, &cWeatherDataModel_Http_Wind_T_RH_PAR::temperatureChanged, this, &cCtrlDataModelRemote::updateTemperatureData);
        QObject::connect(pWind_T_RH_PAR, &cWeatherDataModel_Http_Wind_T_RH_PAR::relativeHumidityChanged, this, &cCtrlDataModelRemote::updateRelativeHumidityData);
        QObject::connect(pWind_T_RH_PAR, &cWeatherDataModel_Http_Wind_T_RH_PAR::parChanged, this, &cCtrlDataModelRemote::updateParData);
    }
}

bool cCtrlDataModelRemote::systemReady() const
{
 
    if (!isConnected())
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
    mDollyPosition = pos;

    if (!mConnected) return;

    sendSpidercamPosition(pos);
}

void cCtrlDataModelRemote::updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg)
{
    mWindSpeedValid = valid_wind_speed;
    mWindSpeed_mps = wind_speed_mps;
    mWind_dir_deg = wind_dir_deg;

    if (!mConnected) return;

    sendWindData(mWindSpeedValid, mWindSpeed_mps, mWind_dir_deg);
}

void cCtrlDataModelRemote::updateTemperatureData(double temp_C)
{
    mTemperature_C = temp_C;

    if (!mConnected) return;

    sendTemperatureData(mTemperature_C);
}

void cCtrlDataModelRemote::updateRelativeHumidityData(double rh_pct)
{
    mRH_pct = rh_pct;

    if (!mConnected) return;

    sendRelativeHumidityData(mRH_pct);
}

void cCtrlDataModelRemote::updateParData(double par_umole)
{
    mPAR_umole = par_umole;

    if (!mConnected) return;

    sendParData(mPAR_umole);
}

void cCtrlDataModelRemote::setCommunicationParamters(const QString& hostname, uint16_t port,
    bool use_ipv6, const QString& local_ip)
{
    mHostName = hostname;
    mPort = port;
    mUseIpv6 = use_ipv6;
    mLocalIp = local_ip;
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

bool cCtrlDataModelRemote::openDataFile(const QString& defaultPath, const std::string& defaultFilename, bool autoSave)
{
    std::time_t t = std::time(nullptr);
    tm* ltm = localtime(&t);

    std::string filename;
    std::string measurement_title = mMeasurementTitle;

    if (measurement_title.empty())
        measurement_title = mExperimentTitle;

    std::replace_if(measurement_title.begin(), measurement_title.end(),
        [](std::string::value_type c)
        {
            if (c == '/') return true;
            if (c == '*') return true;
            if (c == '\\') return true;
            if (c == '<') return true;
            if (c == '>') return true;
            if (c == ':') return true;
            if (c == '|') return true;
            if (c == '?') return true;
            return c <= ' ';
        }, '_');

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
    filename += measurement_title;

    // We are going to try an open a data file on the remote computer
    // three times.

    for (int i = 0; i < 3; ++i)
    {
        sendOpenDataFile(filename);

        QTime delayTime = QTime::currentTime().addSecs(3);
        while (QTime::currentTime() < delayTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (mDataFileIsOpen)
                return true;
        }
    }

    return mDataFileIsOpen;
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

bool cCtrlDataModelRemote::hasRequiredSensors(const std::string& exp_name, const nlohmann::json& expDoc)
{
    auto remote_sensors = mpView->getSensorStatus();

    auto required_sensors = expDoc["sensors"];
    std::vector<std::string> missing_sensors;

    for (const auto& required_sensor : required_sensors)
    {
        std::string sensor_model;
        std::string sensor_name;

        if (expDoc.contains(required_sensor))
        {
            auto sensor_info = expDoc[required_sensor];

            if (sensor_info.contains("Model"))
                sensor_model = sensor_info["Model"];

            if (sensor_info.contains("Name"))
                sensor_name = sensor_info["Name"];
        }

        missing_sensors.push_back(required_sensor.get<std::string>());

        for (const auto& sensor : mThread.mActiveSensors)
        {
            if (required_sensor == sensor->descriptor())
            {
                if (sensor->getStatus() == sensor::eStatus::RUNNING)
                {
                    auto it = std::remove(missing_sensors.begin(), missing_sensors.end(), required_sensor.get<std::string>());
                    missing_sensors.erase(it, missing_sensors.end());
                }
            }
        }

        for (const auto& sensor : remote_sensors)
        {
            if ((required_sensor == sensor.sensor_name) || (sensor_name == sensor.sensor_name) || (sensor_model == sensor.sensor_name))
            {
                if (sensor.sensor_status == "RUNNING")
                {
                    auto it = std::remove(missing_sensors.begin(), missing_sensors.end(), required_sensor.get<std::string>());
                    missing_sensors.erase(it, missing_sensors.end());
                }
            }
        }
    }

    if (missing_sensors.size() > 0)
    {
        QString title = "Measurement: ";
        title += QString::fromStdString(exp_name);
        cMissingSensorsDlg dlg(title);

        for (const auto& name : missing_sensors)
        {
            auto it = std::find_if(mThread.mActiveSensors.begin(), mThread.mActiveSensors.end(), [name](const cSensorModel* sensor) { return name == sensor->descriptor(); });

            if (it == mThread.mActiveSensors.end())
            {
                if (expDoc.contains(name))
                {
                    std::string sensor_manufacturer;
                    std::string sensor_model;
                    std::string sensor_name;

                    auto sensor_info = expDoc[name];

                    if (sensor_info.contains("Manufacturer"))
                        sensor_manufacturer = sensor_info["Manufacturer"];

                    if (sensor_info.contains("Model"))
                        sensor_model = sensor_info["Model"];

                    if (sensor_info.contains("Name"))
                        sensor_name = sensor_info["Name"];

                    auto remote_it = std::find_if(remote_sensors.begin(), remote_sensors.end(), [name, sensor_name, sensor_model](const cRemoteClientView::sRemoteSensorStatus_t& sensor)
                        { return (name == sensor.sensor_name) || (sensor_name == sensor.sensor_name) || (sensor_model == sensor.sensor_name); });

                    if (remote_it == remote_sensors.end())
                        dlg.addMissingSensor(name, sensor_manufacturer, sensor_model, sensor_name);
                    else
                        dlg.addSensor(name, sensor_manufacturer, sensor_model, sensor_name, remote_it->sensor_status);
                }
                else
                    dlg.addMissingSensor(name);
            }
            else
            {
                const cSensorModel* sensor = *it;

                std::string manufacturer = sensor->manufacturer();
                std::string model = sensor->model();
                std::string sensor_name = sensor->name();
                std::string status = sensor::to_string(sensor->getStatus());

                dlg.addSensor(name, manufacturer, model, sensor_name, status);
            }
        }

        auto result = dlg.exec();

        if (result == QDialog::Rejected)
        {
            emit statusMessage("Measurement terminated.");
            return false;
        }
    }


    return true;
}

bool cCtrlDataModelRemote::loadExperiment(const std::string& exp_path, const std::string& exp_name, const nlohmann::json& expDoc)
{
    if (!isConnected())
        return false;

    bool result = cCtrlDataModel::loadExperiment(exp_path, exp_name, expDoc);
    if (!result) return false;

    mExperimentTypeConfirmed = false;
    mExperimentInfoConfirmed = false;

    // We are going to try send the experiment info on the remote computer
    // three times.
    for (int i = 0; i < 3; ++i)
    {
        encodeExperimentTitle(mExperimentTitle);

        if (!mMeasurementTitle.empty())
            encodeMeasurementTitle(mMeasurementTitle);

        if (!mPrincipalInvestigator.empty())
            encodePrincipalInvestigator(mPrincipalInvestigator);

        if (mResearchers.size() > 1)
        {
            encodeResearchers(mResearchers);
        }
        else if (mResearchers.size() == 1)
        {
            encodeResearcher(mResearchers[0]);
        }

        if (!mSpecies.empty())
            encodeSpecies(mSpecies);

        if (!mCultivar.empty())
            encodeCultivar(mCultivar);

        if (!mConstructName.empty())
            encodeConstructName(mConstructName);

        if (!mEventNumbers.empty())
        {
            encodeEventNumbers(mEventNumbers);
        }
    
        if (!mFieldDesign.empty())
            encodeFieldDesign(mFieldDesign);

        if (!mAuthorization.empty() && !mPermit.empty())
            encodePermitInfo(mAuthorization, mPermit);
        else if (!mPermit.empty())
            encodePermitInfo(mPermit);

        if (mPlantingDate > 0)
            encodePlantingDate(mPlantingDate);

        if (mHarvestDate > 0)
            encodeHarvestDate(mHarvestDate);

        if (!mTreatments.empty())
        {
            encodeTreatments(mTreatments);
        }

        if (!mComments.empty())
        {
            encodeComments(mComments);
        }

        encodeDocument(mExperimentDoc);

        encodeEndOfExperimentInfo();

        sendData();

        QTime delayTime = QTime::currentTime().addSecs(5);
        while (QTime::currentTime() < delayTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

            if (mExperimentInfoConfirmed)
                return result;
        }
    }

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

cExperimentState* cCtrlDataModelRemote::createState(const std::string& type, const nlohmann::json& stateDoc, QObject* parent)
{
    if (type == "marker")
    {
        std::string marker_type = stateDoc["marker type"];

        if (marker_type == "start position")
            return new cExperimentState_MarkStartPosition(this);

        if (marker_type == "end position")
            return new cExperimentState_MarkEndPosition(this);

        if (marker_type == "start reference")
            return new cExperimentState_MarkStartReference(this);

        if (marker_type == "end reference")
            return new cExperimentState_MarkEndReference(this);
    }

    return nullptr;
}

void cCtrlDataModelRemote::recordStartPosition()
{
    if (!mConnected) return;

    sendSpidercamStartPosition(mDollyPosition);
}

void cCtrlDataModelRemote::recordEndPosition()
{
    if (!mConnected) return;

    sendSpidercamEndPosition(mDollyPosition);
}

void cCtrlDataModelRemote::recordStartReference()
{
    if (!mConnected) return;

    sendMarkerReferenceStart();
}

void cCtrlDataModelRemote::recordEndReference()
{
    if (!mConnected) return;

    sendMarkerReferenceEnd();
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

    if (isConnected())
    {
        QString msg = "Connection to the C4 has been lost!";
        emit errorMessage("Connection Lost", msg);
        mpView->enableReconnectButton(true);
        mpView->removeAllSensors();
    }

    mThread.mpController->clearStateCreators();

    for (auto* page : mPropertyPages)
    {
        removeSensorPropertyPage(page);
        page->deleteLater();
    }
    mPropertyPages.clear();

    cNetworkEncoder::clear();

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
void cCtrlDataModelRemote::onExperimentTypeReply()
{
    mExperimentTypeConfirmed = true;
}

void cCtrlDataModelRemote::onExperimentInfoReply()
{
    mExperimentInfoConfirmed = true;
}

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

void cCtrlDataModelRemote::onLogMessage(uint8_t msg_type, const std::string& device, const std::string& instance, const std::string& msg)
{
    std::string name = device + ":" + instance;
    mpView->updateLogMsg(msg_type, QString::fromStdString(name), QString::fromStdString(msg));
}

void cCtrlDataModelRemote::onSensorStatus(const std::string& sensor, const std::string& status)
{
    mpView->updateSensorStatus(QString::fromStdString(sensor), QString::fromStdString(status));
}

void cCtrlDataModelRemote::onSensorStatus(const std::string& sensor, const std::string& instance, const std::string& status)
{
    mpView->updateSensorStatus(QString::fromStdString(sensor), QString::fromStdString(instance), QString::fromStdString(status));
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

void cCtrlDataModelRemote::onSensorNameChange(const std::string& old_name, const std::string& new_name, const std::string& instance)
{
    auto oname = QString::fromStdString(old_name);
    oname += ":";
    oname += QString::fromStdString(instance);

    auto nname = QString::fromStdString(new_name);
    nname += ":";
    nname += QString::fromStdString(instance);

    mpView->sensorNameChange(QString::fromStdString(old_name), QString::fromStdString(new_name), QString::fromStdString(instance));

    for (auto* page : mPropertyPages)
    {
        if (page->windowTitle() == oname)
        {
            page->setTitle(nname);
            break;
        }
    }
}

void cCtrlDataModelRemote::onSensorPropertyConnectInfo(const std::string& sensor, 
    const std::string& model, uint32_t version, const std::string& name,
    const std::string& ip_address, uint16_t port)
{
    cSensorPropertyPage* page = create_sensor_property_page(sensor, 
        model, version, ip_address, port, mLocalIpAddress);

    if (!page) return;

    page->setTitle(QString::fromStdString(name));
    page->createWidgets();
    page->doLayout();

    connect(page, &cSensorPropertyPage::statusMessage, this, &cDataModel::statusMessage);
    mPropertyPages.push_back(page);

    addSensorPropertyPage(page);
}

void cCtrlDataModelRemote::onSensorPropertyConnectInfo(const std::string& sensor,
    const std::string& model, uint32_t version, const std::string& name, const std::string& instance,
    const std::string& ip_address, uint16_t port)
{
    cSensorPropertyPage* page = nullptr;

    if (instance.empty())
        page = create_sensor_property_page(sensor, model, version, ip_address, port, mLocalIpAddress);
    else
        page = create_sensor_property_page(sensor, instance, model, version, ip_address, port, mLocalIpAddress);

    if (!page) return;

    QString title = QString::fromStdString(name);
    title += ":";
    title += QString::fromStdString(instance);

    page->setTitle(title);
    page->createWidgets();
    page->doLayout();

    connect(page, &cSensorPropertyPage::statusMessage, this, &cDataModel::statusMessage);
    mPropertyPages.push_back(page);

    addSensorPropertyPage(page);
}

void cCtrlDataModelRemote::onUnknownID(uint16_t id)
{
    QString msg("Received unknown packet id from remote computer: ");
    msg += QString::number(id);
    emit statusMessage(msg);
}
