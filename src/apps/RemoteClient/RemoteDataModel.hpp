
#pragma once

#include "DataModel.hpp"
#include "RemoteClientNetDecoder.hpp"
#include "RemoteClientNetEncoder.hpp"
#include "RemoteDataThread.hpp"
#include "SensorModel.hpp"
#include "SensorController.hpp"

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/ExperimentSerializer.hpp>
#include <cbdf/SpidercamSerializer.hpp>
#include <cbdf/WeatherSerializer.hpp>

#include <string>
#include <filesystem>

#include <QByteArray>
#include <QtNetwork/QTcpServer>


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

    std::size_t sensorCount() const override;
    void addSensor(cSensorModel* pSensor) override;
    void addSensorController(cSensorController* pController);

    void startDataThread() override;
    void stopDataThread() override;

    void sendStatusMessage(const QString& msg);
    void sendStatusMessage(const std::string& msg);

    void sendLogMessage(uint8_t type, const QString& device, const QString& msg);
    void sendLogMessage(uint8_t type, const std::string& device, const std::string& msg);

    const cRemoteDataThread& getThread() const { return mThread; }

signals:
    void requestDataRecordingState(bool record);
    void logMessage(quint8 type, QString device, QString msg);

    void localStatusMessage(QString msg) const;
    void localLogMessage(quint8 type, QString device, QString msg);

/*
 * Signals handlers from the timer
 */
private slots:
    void onHeartbeat();

/*
 * Signals handlers from the sensors
 */
private slots:
    void updateSensorStatus(QString name, sensor::eStatus status);
    void updateSensorName(QString old_name, QString new_name);

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
    void onExperimentType(const std::string& type) override;

    void onExperimentInfo(const std::string& title, const std::string& researcher,
        const std::string& cultivar, const std::string& doc) override;

    void onExperimentInfo(const std::string& title, const std::string& researcher,
        const std::string& species, const std::string& cultivar, const std::string& doc) override;

    void onExperimentTitle(const std::string& title) override;
    void onMeasurementTitle(const std::string& title) override;
    void onExperimentDocument(const std::string& doc) override;

    void onPrincipalInvestigator(const std::string& pi) override;

    void onStartOfResearcherList() override;
    void onEndOfResearcherList() override;
    void onResearcher(const std::string& researcher) override;

    void onSpecies(const std::string& species) override;
    void onCultivar(const std::string& cultivar) override;
    void onConstructName(const std::string& name) override;

    void onStartOfEventNumberList() override;
    void onEndOfEventNumberList() override;
    void onEventNumber(const std::string& event_num) override;

    void onFieldDesign(const std::string& design) override;
    void onPlantingDate(std::time_t date) override;
    void onHarvestDate(std::time_t date) override;

    void onStartOfTreatmentList() override;
    void onEndOfTreatmentList() override;
    void onTreatment(const std::string& treatment) override;

    void onStartOfCommentList() override;
    void onEndOfCommentList() override;
    void onComment(const std::string& comment) override;

    void onPermitInfo(const std::string& permit) override;

    void onEndOfExperimentInfo() override;

    void onStartExperiment() override;
    void onStopExperiment() override;

    void onOpenDataFile(const std::string& fileName) override;
    void onCloseDataFile() override;
    void onStartDataRecording() override;
    void onStopDataRecording() override;

    void onSpidercamPosition(const spidercam::sPosition_1_t& pos) override;
    void onWindData(bool valid, double wind_speed_mps, double wind_direction_deg) override;
    void onTemperatureData(double temp_C) override;
    void onRelativeHumidityData(double rh_pct) override;
    void onParData(double par_umole) override;

private:
    void clearExperimentInfo();

    int sendOutgoingData(const char* data, std::size_t len) override;

protected:
    std::filesystem::path mDefaultDataPath;
    bool mIsRecording = false;
    bool mIsExperimentRunning = false;

    std::string  mExperimentType;

    std::filesystem::path   mFullyQualifiedFileName;
    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;
    cSpidercamSerializer    mSpidercamSerializer;
    cWeatherSerializer      mWeatherSerializer;

    cRemoteDataThread mThread;

    std::string mLocalIpAddress;
    QTcpServer* mpTcpServer = nullptr;
    QTcpSocket* mpClient = nullptr;

    QTimer* mpHeartbeatTimer = nullptr;

    std::vector<cSensorController*> mSensorControllers;

private:
    // Experiment Info
    std::string  mExperimentTitle;
    std::string  mMeasurementTitle;
    std::string  mPrincipalInvestigator;
    std::vector<std::string> mResearchers;
    std::string  mSpecies;
    std::string  mCultivar;
    std::string  mPermitInfo;
    std::string  mConstructName;
    std::vector<std::string> mEventNumbers;
    std::string  mFieldDesign;
    std::string  mExperimentDoc;

    std::time_t mPlantingDate = {};
    std::time_t mHarvestDate = {};

    std::vector<std::string>  mTreatments;
    std::vector<std::string>  mComments;

    // Spidercam Info
    spidercam::sPosition_1_t mDollyPosition;

    // Weather Info
    bool mWindDataValid = false;
    double mWindSpeed_mps = 0.0;
    double mWindDirection_deg = 0.0;
    double mTemperature_C = 0.0;
    double mRelativeHumidity_pct = 0.0;
    double mPAR_umole = 0.0;
};


