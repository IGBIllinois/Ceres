
#pragma once

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

protected:
    void onQueryState() override;
    void onQueryCameraId() override;
    void onQueryImageSize() override;
    void onQueryFrameRate() override;

    void setCameraId(uint8_t id) override;
    void setImageSize(uint16_t width, uint16_t height) override;
    void setFrameRate(uint8_t fps) override;


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


