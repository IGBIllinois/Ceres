
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
    cAxisCommunicationsController(cAxisCommunicationsModel* model, QObject* parent = nullptr);

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;
    const std::string& instance() const override;
    bool has_instance() const override;

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

private:
    cAxisCommunicationsModel* mpModel = nullptr;
};


/******************************************************************************
 * Axis Communications Controller F44
 ******************************************************************************/

class cAxisCommunicationsController_F44 : public cSensorController,
    private cAxisControllerNetDecoder, private cAxisControllerNetEncoder
{
    Q_OBJECT

public:
    cAxisCommunicationsController_F44(cAxisCommunicationsModel_F44* model, QObject* parent = nullptr);

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;
    const std::string& instance() const override;
    bool has_instance() const override;

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

protected:
    void onQueryMode() override;
    void onQueryState() override;
    void onQueryCameraId() override;
    void onQueryImageSize() override;
    void onQueryFrameRate() override;
    void onQueryLapseInterval() override;

    void onGrabImage() override;
    void onTakePhoto(bool updateView) override;
    void onTakePhoto(bool updateView, bool autoSave) override;

    void setMode(uint8_t mode) override;
    void setCameraId(uint8_t id) override;
    void setImageSize(uint16_t width, uint16_t height) override;
    void setFrameRate(uint8_t fps) override;
    void setLapseInterval_ms(uint32_t interval_ms) override;

    void onSaveState() override;
    void onRestoreState() override;

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

private:
    cAxisCommunicationsModel_F44* mpModel = nullptr;
};


