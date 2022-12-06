
#pragma once

#include "../RgbCameraModel.hpp"
#include "AxisCommunicationsUtils.hpp"
#include "AxisCommunicationsSerializer.hpp"

#include <QNetworkReply>
#include <QUrl>
#include <QBitmap>

#include <vector>

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
    const char* descriptor() const override;

    /*
     * Returns a string used as a data descriptor of the
     * type sensor.
     */
    static const char* data_type() { return "rgb"; };

    /*
     * Returns a string used as a protocol descriptor of the
     * type sensor.
     */
    static const char* protocol() { return "http"; };

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool isConnected() const { return mConnected; }

    const QImage& getCurrentImage() const;

    QUrl url() const { return mUrl; }

    int getVapixVersion() const;
    const std::vector<axis::sImageSize_t>& getImageSizes() const;
    const std::vector<axis::eIMAGE_FORMAT>& getImageFormats() const;


    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void update() override;

signals:
    void onNewImage(const QImage& image);

protected slots:
    void requestReceived(QNetworkReply* pReply);

protected:
    cAxisCommunicationsModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cAxisCommunicationsModel();

    bool queryVapixSupport();
    bool querySupportedResolutions();
    bool querySupportedImageFormats();
    axis::sImageSize_t queryImageResolution(uint8_t camera);

    QBitmap getBitmap(int cameraId, axis::sImageSize_t resolution = axis::sImageSize_t());
    QImage getJPEG(int cameraId, axis::sImageSize_t resolution = axis::sImageSize_t());

//    void getRequest();

    QString queryServer(const QNetworkRequest& request);
    virtual void processReply(const std::string& reply) {};

protected:
    bool mConnected;
    QNetworkAccessManager* mpHttpManager;
    QUrl mUrl;

    int mVapixVersion;
    std::vector<axis::sImageSize_t>  mSupportedImageSizes;
    std::vector<axis::eIMAGE_FORMAT> mSupportedImageFormats;

    QImage mCurrentImage;

    cAxisCommunicationsSerializer mSerializer;
};

