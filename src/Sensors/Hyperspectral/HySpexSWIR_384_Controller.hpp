
#pragma once

#include "../SensorController.hpp"
#include "HySpexSWIR_384_ControllerNetDecoder.hpp"
#include "HySpexSWIR_384_ControllerNetEncoder.hpp"

class cHySpexSWIR_384_Model;


class cHySpexSWIR_384_Controller : public cSensorController,
    private cHySpexSWIR_384_ControllerNetDecoder, 
    private cHySpexSWIR_384_ControllerNetEncoder
{
	Q_OBJECT

public:
    cHySpexSWIR_384_Controller(cHySpexSWIR_384_Model* model, QObject* parent = nullptr);

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;

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
    cHySpexSWIR_384_Model* mpModel = nullptr;
};


