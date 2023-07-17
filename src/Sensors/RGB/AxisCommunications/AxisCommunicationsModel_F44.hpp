
#pragma once

#include "AxisCommunicationsModel.hpp"
#include "Timers.hpp"
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

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Attach/Detach the serializer.
     */
    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    int getActiveCameraID() const override;
    int getActiveFramesRate_fps() const override;
    rgb::sImageSize_t getActiveImageSize() const override;

signals:
    void enableCamera(int id);

public slots:
    void setActiveCamera(int id);
    void setActiveImageSize(rgb::sImageSize_t image_size);
    void setActiveFramesRate_fps(int fps);

protected slots:
    void frameGrabbed(int id, QImage* img);
    void imageGrabbed(int id, QImage* img);
    void errorHappend(int id, QString msg);
    void stateChanged(int id, cAxisCamera::GrabbingState newState);

    void requestReceived(QNetworkReply* pReply);

    virtual void processReply(const std::string& reply) {};

private:
    QByteArray mImageData;
    QBuffer mImageBuffer;

    cAxisCamera* mpActiveCamera;

    std::vector<cAxisCamera*> mCameras;
};

