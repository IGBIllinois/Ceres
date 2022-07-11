
#pragma once

#include "DataModel.hpp"
#include "ceres_remote_client_net_decoder.hpp"
#include "ceres_remote_client_net_encoder.hpp"
#include "RemoteDataThread.hpp"
#include "BlockDataFile.hpp"
#include "ExperimentSerializer.hpp"
#include "SpidercamSerializer.hpp"
#include "WeatherSerializer.hpp"

#include <string>
#include <filesystem>

#include <QByteArray>
#include <QtNetwork/QTcpServer>


// Forward Declarations
class cSensorModel;

/*****************************************************************************
 * 
 * The cRemoteDataModel class is the base class for data acquisition.
 * 
 *****************************************************************************/
class cRemoteDataModel : public cDataModel, 
    private cCeresRemoteClientNetDecoder, private cCeresRemoteClientNetEncoder
{
    Q_OBJECT

public:
    explicit cRemoteDataModel(QObject* parent = nullptr);
    virtual ~cRemoteDataModel();

    std::string defaultDataPath() const;
    void setDefaultDataPath(const std::string& data_path);

    bool startTcpServer(const std::string& ip, uint16_t port);

    void addSensor(cSensorModel* pSensor) override;

    void startDataThread() override;
    void stopDataThread() override;

signals:
    void requestDataRecordingState(bool record);

/*
 * Signals handlers from the TCP server
 */
private slots:
    void acceptError(QAbstractSocket::SocketError socketError);
    void newConnection();

/*
 * Signals handlers from the client TCP socket
 */
private slots:
    void processNewCommand();
    void clientDisconnected();
    void clientErrorOccurred(QAbstractSocket::SocketError socketError);
    void clientStateChanged(QAbstractSocket::SocketState socketState);

/*
 * Packet Handlers
 */
    void onExperimentInfo(const std::string& title, const std::string& researcher,
        const std::string& cultivar, const std::string& doc) override;

    void onStartExperiment() override;
    void onStopExperiment() override;

    void onOpenDataFile(const std::string& fileName) override;
    void onCloseDataFile() override;
    void onStartDataRecording() override;
    void onStopDataRecording() override;

    void onSpidercamPosition(const spidercam::sPosition& pos) override;
    void onWeatherData(bool valid, double wind_speed_mps, double wind_direction_deg) override;

private:
    int sendOutgoingData(const char* data, std::size_t len) override;

protected:
    std::filesystem::path mDefaultDataPath;
    bool mIsRecording;

    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;
    cSpidercamSerializer    mSpidercamSerializer;
    cWeatherSerializer      mWeatherSerializer;

    cRemoteDataThread mThread;

    QTcpServer* mpTcpServer;

    QTcpSocket* mpClient;

private:
    // Experiment Info
    std::string  mExperimentTitle;
    std::string  mResearcher;
    std::string  mCultivar;
    std::string  mExperimentDoc;

    // Spidercam Info
    spidercam::sPosition mDollyPosition;

    // Weather Info
    bool mWindDataValid;
    double mWindSpeed_mps;
    double mWindDirection_deg;
};


