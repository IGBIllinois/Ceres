
#pragma once

#include "HokuyoModel.hpp"

#include <cbdf/HokuyoSerializer.hpp>


#include <QObject>
#include <QTimer>

#include <queue>


class cHokuyoModel_UST10LX : public cHokuyoModel
{
    Q_OBJECT

public:
    cHokuyoModel_UST10LX(QObject* parent = nullptr);
    virtual ~cHokuyoModel_UST10LX();

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;
    bool initialize() override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;
    
    void writeDataHeader() override;

public:
    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void pauseCommunications();
    void restoreCommunications();
    bool isCommunicationsPaused() const;

// Override of the virtual public slots in cHokuyoModel

protected:

protected:
    void update() override;

private:

private:
    void emitStatusMessage(QString& msg);
    void emitLogMessage(quint8 type, QString msg);

private:
    bool mConnected;
    bool mPauseCommunications;

    uint16_t mLidarPort;
    std::string mSensorIpAddress;
    std::string mDstIpAddress;

    cHokuyoSerializer mSerializer;
};

