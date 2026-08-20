
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
    cTeledyneFlirController(cTeledyneFlirCameraModel* model, QObject* parent = nullptr);

    void connectToModel() override;

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
    void requestFrameRate_Hz(double frame_rate_hz);
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
    void lapseIntervalChanged(int interval_ms);
    void frameRateChanged(double rate_fps);
    void imageSizeChanged(int width, int height);

protected:
    void onQueryMode() override;
    void onQueryImageSize() override;
    void onQueryFrameRate() override;
    void onQueryLapseInterval() override;

    void setMode(uint8_t mode) override;
    void setImageSize(uint16_t width, uint16_t height) override;
    void setFrameRate_Hz(double fps) override;
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
    cTeledyneFlirCameraModel* mpModel = nullptr;
};


/******************************************************************************
 * Teledyne FLIR Controller T1K for T1K series of FLIR cameras
 ******************************************************************************/

class cTeledyneFlirController_T1K : public cTeledyneFlirController
{
    Q_OBJECT

public:
    cTeledyneFlirController_T1K(cTeledyneFlirCameraModel_T1K* model, QObject* parent = nullptr);

    void connectToModel() override;

public slots:
    void onPhotoTaken();

protected:
    void onQueryState() override;
    void onQueryThermalRange() override;
    void onGrabImage() override;
    void onTakePhoto(bool updateView) override;
    void onTakePhoto(bool updateView, bool autoSave) override;

    void setThermalRange_K(float min_value_K, float max_value_K) override;

private:
    cTeledyneFlirCameraModel_T1K* mpModel = nullptr;
};


