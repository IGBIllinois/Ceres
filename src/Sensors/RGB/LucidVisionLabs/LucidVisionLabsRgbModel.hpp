
#pragma once

#include "../RgbTypes.hpp"
#include "../RgbCameraModel.hpp"

#include <LucidVisionLabsConnect/LucidVisionLabsData.hpp>
#include <LucidVisionLabsConnect/LucidVisionLabsTypes.hpp>

#include <cbdf/LucidTritonSerializer.hpp>

#include <QImage>


class cLucidVisionLabsRgbModel : public cRgbCameraModel
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
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

//    bool isConnected() const { return mConnected; }

    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    uint16_t imageWidth() const;
    uint16_t imageHeight() const;

    const QImage& getCurrentImage() const;

signals:
    void onNewImage(const QImage& image);
    void photoTaken();
    void modeChanged(int mode);
    void lapseIntervalChanged(int interval_ms);
    void frameRateChanged(double rate_fps);
    void imageSizeChanged(int width, int height);

public slots:
    virtual void requestMode(int mode) = 0;
    virtual void requestFrameRate_Hz(double frame_rate_hz) = 0;
    virtual void requestLapseInterval_ms(uint32_t interval_ms) = 0;
    virtual void requestImage() = 0;
    virtual void requestImages(bool update_view) = 0;
    virtual void takePhoto(bool update_view = false) = 0;

protected:
    cLucidVisionLabsRgbModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cLucidVisionLabsRgbModel();

protected:
    std::string mFamilyName;
    std::string mModelVersion;
    std::string mFirmwareVersion;
    std::string mMacAddress;
    std::string mIpAddress;

    bool mIsRunning = false;

    bool mPhotoRequested = false;

    uint16_t mImageWidth = 0;
    uint16_t mImageHeight = 0;

    bool mImageRequested = false;
    bool mAutoEmitImages = false;

    nLucidVisionLabsConnect::cRgbImage mCurrentImage;

    QImage mImage;
    std::vector<uint8_t> mImageBuffer;

    cLucidTritonSerializer mSerializer;
};


/******************************************************************************
 *  I M P L E M E N T A T I O N   D E T A I L S
 *****************************************************************************/

inline uint16_t cLucidVisionLabsRgbModel::imageWidth() const { return mImageWidth; }
inline uint16_t cLucidVisionLabsRgbModel::imageHeight() const { return mImageHeight; }

