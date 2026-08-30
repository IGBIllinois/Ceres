
#pragma once

#include "../../SensorController.hpp"
#include "TeledyneFlirControllerNetDecoder.hpp"
#include "TeledyneFlirControllerNetEncoder.hpp"

class cTeledyneFlirCameraModel;
class cTeledyneFlirCameraModel_T1K;


class cTeledyneFlirController : public cSensorController,
    protected cTeledyneFlirControllerNetDecoder, protected cTeledyneFlirControllerNetEncoder
{
	Q_OBJECT

public:
    cTeledyneFlirController(QObject* parent = nullptr);

signals:
    void queryMode();
    void queryFrameRate_Hz();
    void queryLapseInterval_ms();
    void queryImageSize();

signals:
    void requestMode(int mode);
    void requestFrameRate_Hz(double frame_rate_hz);
    void requestLapseInterval_ms(uint32_t interval_ms);
    void requestImageSize(int width, int height);

    void requestImage();
    void requestImages(bool update_view);
    void requestPhoto(bool update_view);
    void requestPhoto(bool update_view, bool auto_save);

    void requestSaveState();
    void requestRestoreState();

public slots:
    void photoTaken();
    void modeChanged(int mode);
    void lapseIntervalChanged(int interval_ms);
    void frameRateChanged(double rate_fps);
    void imageSizeChanged(int width, int height);

protected:
    // Message handlers from the decoder
    void onQueryModeMessage() override;
    void onQueryImageSizeMessage() override;
    void onQueryFrameRateMessage() override;
    void onQueryLapseIntervalMessage() override;

    void onSetModeMessage(uint8_t mode) override;
    void onSetImageSizeMessage(uint16_t width, uint16_t height) override;
    void onSetFrameRateMessage(double fps) override;
    void onSetLapseIntervalMessage(uint32_t interval_ms) override;

    void onSaveStateMessage() override;
    void onRestoreStateMessage() override;

protected:
    /**
     * @brief processStream
     * The processStream method converts the byte stream from a TCP
     * receiver port to packet for the decoder.
     */
    void processStream(const void* pBuffer, std::size_t buf_length) override;

    /**
     * @brief sendOutgoingData
     * Sends any outgoing to the property page.
     */
    int sendOutgoingData(const char* data, std::size_t len) override
    {
        return cSensorController::sendOutgoingData(data, len);
    }
};


/******************************************************************************
 * Teledyne FLIR Controller T1K for T1K series of FLIR cameras
 ******************************************************************************/

class cTeledyneFlirController_T1K : public cTeledyneFlirController
{
    Q_OBJECT

public:
    cTeledyneFlirController_T1K(QObject* parent = nullptr);


signals:
    void queryState();
    void queryThermalRange();

    void requestThermalRange(float minValue_K, float maxValue_K);

    void  grabImage();

public slots:
    void stateUpdated(int mode, int width, int height, double fps, double min_fps, double max_fps,
        int interval_ms, float minValue_K, float maxValue_K);

    void thermalRangeUpdated(float minValue_K, float maxValue_K);

    void onPhotoTaken();

protected:
    void onQueryStateMessage() override;
    void onQueryThermalRangeMessage() override;
    void onGrabImageMessage() override;
    void onTakePhotoMessage(bool updateView) override;
    void onTakePhotoMessage(bool updateView, bool autoSave) override;

    void onSetThermalRangeMessage(float min_value_K, float max_value_K) override;
};


