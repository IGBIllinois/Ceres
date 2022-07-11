
#pragma once

#include "CtrlDataModel.hpp"
#include "ceres_net_decoder.hpp"
#include "ceres_net_encoder.hpp"

#include <spidercam/spidercam_types.hpp>

#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QDockWidget;
QT_END_NAMESPACE

class cRemoteClientView;



class cCtrlDataModelRemote : public cCtrlDataModel, 
    protected cCeresNetDecoder, protected cCeresNetEncoder
{
    Q_OBJECT

public:
    explicit cCtrlDataModelRemote(QObject* parent = nullptr);
    ~cCtrlDataModelRemote();

    void createView(QDockWidget*& dockWidget);

    bool try_to_connect(const QString& hostname, uint16_t port, 
                        bool use_ipv6, const QString& local_ip);

    void try_reconnection();

    void addExperimentControlModel(cExperimentControlModel* pModel) override;
    void addSensor(cSensorModel* pSensor) override;

    bool openDataFile(const QString& defaultPath) override;
    bool isDataFileOpen() const override;
    void closeDataFile();

    bool loadExperiment(const nlohmann::json& expDoc) override;
    void startExperiment() override;

protected:
    void dataRecordingStateChange(bool record) override;

public slots:
    void updatePosition(spidercam::sPosition pos);
    void updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg);

/*
 * Signals handlers from the TCP socket
 */
private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void hostFound();
    void stateChanged(QAbstractSocket::SocketState socketState);

private:
    void endDataRecording() override;

/*
 * Packet Handlers
 */
private:
    void onDataFileState(bool is_open) override;

private:
    int sendOutgoingData(const char* data, std::size_t len) override;

/*
 *
 */
private:
    bool   mWindSpeedValid;
    double mWindSpeed_mps;
    double mWind_dir_deg;

private:
    bool mConnected;
    bool mDataFileIsOpen;

    cRemoteClientView* mpView;

    QHostAddress mRemoteEndpoint;
    uint16_t   mPort;

    QTcpSocket mSocket;
    QByteArray mReplyBuffer;
};

