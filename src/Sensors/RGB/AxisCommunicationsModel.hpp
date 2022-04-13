
#pragma once

#include "RgbCameraModel.hpp"
#include "../../Utilities/Timers.hpp"

#include <QNetworkReply>
#include <QUrl>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE


class cAxisCommunicationsModel : public cRgbCameraModel
{
    Q_OBJECT

public:
    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

    /*
     * Returns a string used as a data descriptor of the
     * type sensor.
     */
    static char* data_type() { return "rgb"; };

    /*
     * Returns a string used as a protocol descriptor of the
     * type sensor.
     */
    static char* protocol() { return "http"; };

    QUrl url() const { return mUrl; }

    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader(cBlockDataFile& file) override;

    /*
     * Detach the serializer.
     */
    void endDataRecording() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void update() override;

protected slots:
    void requestReceived(QNetworkReply* pReply);

protected:
    cAxisCommunicationsModel(QObject* parent = nullptr);
    virtual ~cAxisCommunicationsModel();

    void getRequest();

    virtual void processReply(const std::string& reply) {};

protected:
    QNetworkAccessManager* mpHttpManager;
    QUrl mUrl;

    cIntervalTimer	mTimer;
};

