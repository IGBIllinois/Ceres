
#pragma once

#include "../IrCameraModel.hpp"
#include "Timers.hpp"
#include "ColorTable.hpp"

#include <TeledyneAtlasConnect/TeledyneAtlasData.hpp>

#include <cbdf/TeledyneFlirSerializer.hpp>

#include <QImage>

#include <vector>
#include <optional>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class cTeledyneFlirCameraModel : public cIrCameraModel
{
    Q_OBJECT

public:
    enum eMode { SINGLE = 0, TIME_LAPSE = 1, CONTINUOUS = 2 };

public:
    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    /*
     * Returns a string used as a data descriptor of the
     * type sensor.
     */
    static const char* data_type() { return "ir"; };

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * General accessors and control of basic camera functions
     */

    eMode mode() const;
    virtual void setMode(eMode mode);

    double frameRate_Hz() const;
    virtual void setFrameRate_Hz(double frame_rate_hz);

    uint32_t frameInterval_ms() const;
    virtual void setFrameInterval_ms(uint32_t frame_interval_ms);

    std::optional<float> minThermalValue_K() const;
    std::optional<float> maxThermalValue_K() const;

    void takePhoto(bool send_image = false);

    const QImage& getCurrentImage() const;

signals:
    void onNewImage(const QImage& image);
    void modeChanged(int mode);
    void frameIntervalChanged(int interval_ms);
    void frameRateChanged(double rate_fps);
    void imageSizeChanged(int width, int height);

public slots:
    void requestMode(int mode);
    void requestImage();
    void requestImages(bool auto_emit);

protected:
    cTeledyneFlirCameraModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cTeledyneFlirCameraModel();

    virtual bool updateFrameInterval(uint32_t frame_interval_ms) = 0;
    virtual bool updateFrameRate(double frame_rate_fps) = 0;

protected:
    eMode mMode = eMode::SINGLE;
    bool mPhotoRequested = false;

    double mFrameRate_fps = 0;
    std::optional<double> mMinFrameRate_fps;
    std::optional<double> mMaxFrameRate_fps;

    std::optional<float> mMinThermalRange_K;
    std::optional<float> mMaxThermalRange_K;

    uint32_t mFrameInterval_ms = 0;
    cOneShotTimer mFrameTimer;

    uint16_t mImageWidth = 0;
    uint16_t mImageHeight = 0;

    bool mImageRequested = false;
    bool mAutoEmitImages = false;

    nTeledyneAtlasConnect::cThermalImage mCurrentImage;

    cColorTable mColorTable;
    QImage mColorizedImage;

    cTeledyneFlirSerializer mSerializer;
};


/******************************************************************************
 *  I M P L E M E N T A T I O N   D E T A I L S
 *****************************************************************************/


inline std::optional<float> cTeledyneFlirCameraModel::minThermalValue_K() const { return mMinThermalRange_K; }
inline std::optional<float> cTeledyneFlirCameraModel::maxThermalValue_K() const { return mMaxThermalRange_K; }

