
#pragma once

#include "../../SensorController.hpp"
#include "HySpexVNIR_3000N_ControllerNetDecoder.hpp"
#include "HySpexVNIR_3000N_ControllerNetEncoder.hpp"

class cHySpexVNIR_3000N_Model;


class cHySpexVNIR_3000N_Controller : public cSensorController,
    private cHySpexVNIR_3000N_ControllerNetDecoder,
    private cHySpexVNIR_3000N_ControllerNetEncoder
{
	Q_OBJECT

public:
    cHySpexVNIR_3000N_Controller(cHySpexVNIR_3000N_Model* model, QObject* parent = nullptr);

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
    cHySpexVNIR_3000N_Model* mpModel = nullptr;
};


