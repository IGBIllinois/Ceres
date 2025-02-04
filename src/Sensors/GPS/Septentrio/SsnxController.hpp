
#pragma once

#include "../GpsController.hpp"
#include "../GpsControllerNetDecoder.hpp"
#include "../GpsControllerNetEncoder.hpp"

class cSsnxModel;
class cGpsControllerNetEncoder;


class cSsnxController : public cGpsController,
    private cGpsControllerNetDecoder,
    private cGpsControllerNetEncoder
{
	Q_OBJECT

public:
    cSsnxController(cSsnxModel* model, QObject* parent = nullptr);

    /*** Messages handlers from the decoder */
protected:
    void onQueryReferenceData() override;
    void onQueryReferenceParameters() override;
    void onSetReferenceParameters(std::uint16_t integration_time_sec, std::uint16_t max_integration_time_sec) override;
    void onCalcReference() override;
    void onStopReference() override;

    void onReferenceComplete() override;

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


