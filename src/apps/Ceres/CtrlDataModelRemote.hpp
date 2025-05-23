
#pragma once

#include "CtrlDataModel.hpp"
#include "ceres_net_decoder.hpp"
#include "ceres_net_encoder.hpp"

#include <spidercam/spidercam_types.hpp>

#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

#include <vector>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QDockWidget;
QT_END_NAMESPACE

class cRemoteClientView;
class cSensorPropertyPage;
class cExperimentStateCreator;


class cCtrlDataModelRemote : public cCtrlDataModel, 
    protected cCeresNetDecoder, protected cCeresNetEncoder
{
    Q_OBJECT

public:
    explicit cCtrlDataModelRemote(QObject* parent = nullptr);
    ~cCtrlDataModelRemote();

    void createView(QDockWidget*& dockWidget);

    bool systemReady() const override;

    void setCommunicationParamters(const QString& hostname, uint16_t port,
        bool use_ipv6, const QString& local_ip);

    bool try_to_connect(const QString& hostname, uint16_t port, 
                        bool use_ipv6, const QString& local_ip);

    void try_reconnection();

    void addExperimentControlModel(cExperimentControlModel* pModel) override;
    void addSensor(cSensorModel* pSensor) override;

    bool openDataFile(const QString& defaultPath, 
                      const std::string& defaultFilename,
                      bool autoSave) override;

    bool isDataFileOpen() const override;
    void closeDataFile();

    bool loadExperiment(const std::string& expName, const nlohmann::json& expDoc) override;
    void startExperiment() override;

    bool isConnected() const;

protected:
    void dataRecordingStateChange(bool record) override;
    void endDataRecording() override;

signals:
    void addSensorPropertyPage(QAction* pAction);
    void removeSensorPropertyPage(QAction* pAction);

public slots:
    void updatePosition(spidercam::sPosition_1_t pos);
    void updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg);
    void updateTemperatureData(double temp_C);
    void updateRelativeHumidityData(double rh_pct);
    void updateParData(double par_umole);

/*
 * Signals handlers from the TCP socket
 */
private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void hostFound();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void processNewCommand();

/*
 * Send data over the TCP socket
 */
private:
    int sendOutgoingData(const char* data, std::size_t len) override;

/*
 * Packet Handlers
 */
private:
    void onExperimentTypeReply() override;

    void onExperimentInfoReply() override;
    void onDataFileState(bool is_open) override;
    void onStatusMessage(const std::string& msg) override;

    void onLogMessage(uint8_t msg_type, const std::string& device, const std::string& msg) override;
    void onLogMessage(uint8_t msg_type, const std::string& device, const std::string& instance, const std::string& msg) override;

    void onSensorStatus(const std::string& sensor, const std::string& status) override;
    void onSensorStatus(const std::string& sensor, const std::string& instance, const std::string& status) override;

    void onSensorNameChange(const std::string& old_name, const std::string& new_name) override;
    void onSensorNameChange(const std::string& old_name, const std::string& new_name, const std::string& instance) override;

    void onSensorPropertyConnectInfo(const std::string& sensor, const std::string& model, uint32_t version,
        const std::string& name, const std::string& ip_address, uint16_t port) override;

    void onSensorPropertyConnectInfo(const std::string& sensor, const std::string& model, uint32_t version,
        const std::string& name, const std::string& instance, const std::string& ip_address, uint16_t port) override;

    void onUnknownID(uint16_t id) override;

/*
 *
 */
private:
    bool   mWindSpeedValid;
    double mWindSpeed_mps;
    double mWind_dir_deg;
    double mTemperature_C;
    double mRH_pct;
    double mPAR_umole;

private:
    bool mConnected = false;
    bool mDataFileIsOpen = false;
    bool mExperimentTypeConfirmed = false;
    bool mExperimentInfoConfirmed = false;

    cRemoteClientView* mpView;

    QString  mHostName;
    uint16_t mPort;
    bool     mUseIpv6;
    QString  mLocalIp;

    std::string  mLocalIpAddress;
    QHostAddress mRemoteEndpoint;

    QTcpSocket mSocket;
    QByteArray mReplyBuffer;

    std::vector<cSensorPropertyPage*>     mPropertyPages;
    std::vector<cExperimentStateCreator*> mStateCreators;
};


inline bool cCtrlDataModelRemote::isConnected() const
{
    return mConnected;
}


