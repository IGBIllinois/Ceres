
#pragma once

#include "../../SensorController.hpp"
#include "LucidRgbControllerNetDecoder.hpp"
#include "LucidRgbControllerNetEncoder.hpp"

class cLucidVisionLabsRgbModel;
class cLucidVisionLabsRgbModel_Triton;


class cLucidVisionLabsRgbController : public cSensorController,
    private cLucidRgbControllerNetDecoder, private cLucidRgbControllerNetEncoder
{
	Q_OBJECT

public:
    cLucidVisionLabsRgbController(cLucidVisionLabsRgbModel* model, QObject* parent = nullptr);

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
//    void onSetAzimuthWindow(double min_deg, double max_deg) override;
//    void onSetMode(ouster::eLIDAR_MODE mode) override;

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
    cLucidVisionLabsRgbModel* mpModel = nullptr;
};


/******************************************************************************
 * Lucid Vision Labs Controller Triton
 ******************************************************************************/

class cLucidVisionLabsRgbController_Triton : public cSensorController,
    private cLucidRgbControllerNetDecoder, private cLucidRgbControllerNetEncoder
{
    Q_OBJECT

public:
    cLucidVisionLabsRgbController_Triton(cLucidVisionLabsRgbModel_Triton* model, QObject* parent = nullptr);

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
    void onQueryMode() override;
    void onQueryImageSize() override;
    void onQueryFrameRate() override;
    void onQueryLapseInterval() override;
    void onGrabImage() override;
    void onTakePhoto(bool updateView) override;
    void onTakePhoto(bool updateView, bool autoSave) override;

    void setMode(uint8_t mode) override;
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
    cLucidVisionLabsRgbModel_Triton* mpModel = nullptr;
};


