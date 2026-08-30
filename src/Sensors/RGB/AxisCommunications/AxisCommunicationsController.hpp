
#pragma once


/******************************************************************************
 * Axis Communications Controller
 * 
 * 
 ******************************************************************************/


#include "../../SensorController.hpp"
#include "AxisControllerNetDecoder.hpp"
#include "AxisControllerNetEncoder.hpp"

class cAxisCommunicationsModel;
class cAxisCommunicationsModel_F44;


class cAxisCommunicationsController : public cSensorController,
    private cAxisControllerNetDecoder, private cAxisControllerNetEncoder
{
	Q_OBJECT

public:
    cAxisCommunicationsController(QObject* parent = nullptr);

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
 * Axis Communications Controller F44
 ******************************************************************************/

class cAxisCommunicationsController_F44 : public cSensorController,
    private cAxisControllerNetDecoder, private cAxisControllerNetEncoder
{
    Q_OBJECT

public:
    cAxisCommunicationsController_F44(QObject* parent = nullptr);

signals:
    void queryMode();
    void queryState();
    void queryCameraId();
    void queryImageSize();
    void queryFrameRate();
    void queryLapseInterval();

signals:
    void requestMode(int mode);
    void requestCameraID(int id);
    void requestImageSize(uint16_t width, uint16_t height);
    void requestFrameRate_Hz(int frame_rate_hz);
    void requestLapseInterval_ms(uint32_t interval_ms);
    void requestImage();
    void requestImages(bool update_view);
    void requestPhoto(bool update_view);
    void requestPhoto(bool update_view, bool auto_save);

    void requestSaveState();
    void requestRestoreState();

public slots:
    void photoTaken();
    void modeChanged(int mode);
    void cameraIdChanged(int id);
    void lapseIntervalChanged(int interval_ms);
    void frameRateChanged(int rate_fps);
    void imageSizeChanged(int width, int height);
    void stateUpdated(int mode, int id, int width, int height, double rate_fps, int interval_ms, int min_id, int max_id, double min_fps, double max_fps);

protected:
    /*** Message Handlers from the network decoder */
    void onQueryModeMessage() override;
    void onQueryStateMessage() override;
    void onQueryCameraIdMessage() override;
    void onQueryImageSizeMessage() override;
    void onQueryFrameRateMessage() override;
    void onQueryLapseIntervalMessage() override;

    void onGrabImageMessage() override;
    void onTakePhotoMessage(bool updateView) override;
    void onTakePhotoMessage(bool updateView, bool autoSave) override;

    void setModeMessage(uint8_t mode) override;
    void setCameraIdMessage(uint8_t id) override;
    void setImageSizeMessage(uint16_t width, uint16_t height) override;
    void setFrameRateMessage(uint8_t fps) override;
    void setLapseIntervalMessage(uint32_t interval_ms) override;

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


