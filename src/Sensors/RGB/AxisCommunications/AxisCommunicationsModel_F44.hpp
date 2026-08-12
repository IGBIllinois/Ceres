
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
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Axis Communications F44 RGB Camera
     */
    uint8_t device_id() const override;

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

    int getMinCameraID() const { return mMinCameraID; }
    int getMaxCameraID() const { return mMaxCameraID; }

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

protected:
    // Unlike most other RGB cameras, the Axis Communication F44 system does not support
    // the various modes
    void setMode(eMode mode)  override {};
    void setFrameRate_Hz(double frame_rate_hz) override {};
    void setLapseInterval_ms(uint32_t interval_ms)  override {};

    bool updateLapseInterval(uint32_t interval_ms) override;
    bool updateFrameRate(double frame_rate_fps) override;

private:
    QByteArray mImageData;
    QBuffer mImageBuffer;

    cAxisCamera* mpActiveCamera;

    int mMinCameraID = -1;
    int mMaxCameraID = -1;

    std::vector<cAxisCamera*> mCameras;

private:
    const uint8_t mDeviceID;
};

