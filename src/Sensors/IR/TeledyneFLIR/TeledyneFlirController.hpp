
#pragma once

#include "../../SensorController.hpp"
#include "TeledyneFlirControllerNetDecoder.hpp"
#include "TeledyneFlirControllerNetEncoder.hpp"

class cTeledyneFlirCameraModel;
class cTeledyneFlirCameraModel_T1K;


class cTeledyneFlirController : public cSensorController,
    private cTeledyneFlirControllerNetDecoder, private cTeledyneFlirControllerNetEncoder
{
	Q_OBJECT

public:
    cTeledyneFlirController(cTeledyneFlirCameraModel* model, QObject* parent = nullptr);

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
    void onQueryState() override;

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

class cTeledyneFlirController_T1K : public cSensorController,
    private cTeledyneFlirControllerNetDecoder, private cTeledyneFlirControllerNetEncoder
{
    Q_OBJECT

public:
    cTeledyneFlirController_T1K(cTeledyneFlirCameraModel_T1K* model, QObject* parent = nullptr);

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;
    const std::string& instance() const override;
    bool has_instance() const override;

public slots:
    void onPhotoTaken();

protected:
    void onQueryState() override;
    void onQueryMode() override;
    void onQueryImageSize() override;
    void onQueryFrameRate() override;
    void onQueryFrameInterval() override;
    void onQueryThermalRange() override;
    void onGrabImage() override;
    void onTakePhoto(bool updateView) override;

    void setMode(uint8_t mode) override;
    void setImageSize(uint16_t width, uint16_t height) override;
    void setFrameRate_Hz(double fps) override;
    void setFrameInterval_ms(uint32_t interval_ms) override;
    void setThermalRange_K(float min_value_K, float max_value_K) override;


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
    cTeledyneFlirCameraModel_T1K* mpModel = nullptr;
};


