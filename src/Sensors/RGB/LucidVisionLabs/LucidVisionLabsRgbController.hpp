
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
//    cLucidVisionLabsRgbController(cLucidVisionLabsRgbModel* model, QObject* parent = nullptr);
    cLucidVisionLabsRgbController(QObject* parent = nullptr);

/*
public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;
    const std::string& instance() const override;
    bool has_instance() const override;
*/

protected:
    /*** Message Handlers from the network decoder */

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

//private:
//    cLucidVisionLabsRgbModel* mpModel = nullptr;
};


/******************************************************************************
 * Lucid Vision Labs Controller Triton
 ******************************************************************************/

class cLucidVisionLabsRgbController_Triton : public cSensorController,
    private cLucidRgbControllerNetDecoder, private cLucidRgbControllerNetEncoder
{
    Q_OBJECT

public:
//    cLucidVisionLabsRgbController_Triton(cLucidVisionLabsRgbModel_Triton* model, QObject* parent = nullptr);
    cLucidVisionLabsRgbController_Triton(QObject* parent = nullptr);

/*
public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;
    const std::string& instance() const override;
    bool has_instance() const override;
*/

protected:
    /*** Message Handlers from the network decoder */
    void onQueryStateMessage() override;
    void onQueryModeMessage() override;
    void onQueryImageSizeMessage() override;
    void onQueryFrameRateMessage() override;
    void onQueryLapseIntervalMessage() override;
    void onGrabImageMessage() override;
    void onTakePhotoMessage(bool updateView) override;
    void onTakePhotoMessage(bool updateView, bool autoSave) override;

    void setModeMessage(uint8_t mode) override;
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

//private:
//    cLucidVisionLabsRgbModel_Triton* mpModel = nullptr;
};


