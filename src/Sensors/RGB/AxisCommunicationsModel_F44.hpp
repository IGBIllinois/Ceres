
#pragma once

#include "AxisCommunicationsModel.hpp"
#include "../../Utilities/Timers.hpp"
#include "AxisCamera.hpp"

//#include <QNetworkReply>
//#include <QUrl>
#include <QImage>

#include <vector>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
QT_END_NAMESPACE


class cAxisCommunicationsModel_F44 : public cAxisCommunicationsModel
{
    Q_OBJECT

public:
    cAxisCommunicationsModel_F44(QObject* parent = nullptr);
    virtual ~cAxisCommunicationsModel_F44();

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

signals:
    void enableCamera(int id);

public slots:
    void setActiveCamera(int id);

protected slots:
    void imageGrabbed(int id, QImage* img);
    void errorHappend(int id, QString msg);
    void stateChanged(int id, cAxisCamera::GrabbingState newState);

    void requestReceived(QNetworkReply* pReply);

    void getRequest();

    virtual void processReply(const std::string& reply) {};

private:
    int mCurrentCameraId;

private:
    cAxisCamera* mpActiveCamera;

    std::vector<cAxisCamera*> mCameras;
};

