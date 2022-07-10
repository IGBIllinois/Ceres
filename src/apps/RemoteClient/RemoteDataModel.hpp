
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

    const std::string& defaultDataPath() const;
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
 * Command handlers
 */
private:
    void startExperiment();
    void stopExperiment();

/*
 * Packet Handlers
 */
    void experimentInfo(const std::string& title, const std::string& researcher,
        const std::string& cultivar, const std::string& doc) override;

    void openDataFile(const std::string& fileName) override;
    void closeDataFile() override;

    void spidercamPosition(const spidercam::sPosition& pos) override;
    void weatherData(bool valid, double wind_speed_mps, double wind_direction_deg) override;

private:
    int sendOutgoingData(const char* data, std::size_t len) override;

private:
    void writeDataHeaders();

protected:
    std::string mDefaultDataPath;
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
};


