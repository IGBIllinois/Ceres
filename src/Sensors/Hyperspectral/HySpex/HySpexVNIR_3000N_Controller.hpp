
#pragma once

#include "HySpexCamera_Controller.hpp"
#include "HySpexVNIR_3000N_ControllerNetDecoder.hpp"
#include "HySpexVNIR_3000N_ControllerNetEncoder.hpp"

class cHySpexVNIR_3000N_Model;


class cHySpexVNIR_3000N_Controller : public cHySpexCamera_Controller,
    private cHySpexVNIR_3000N_ControllerNetDecoder,
    private cHySpexVNIR_3000N_ControllerNetEncoder
{
	Q_OBJECT

public:
    cHySpexVNIR_3000N_Controller(cHySpexVNIR_3000N_Model* model, QObject* parent = nullptr);

/*** Qt slot handler */
public:
    void onBackgroundComplete() override;

/*** Messages handlers from the decoder */
protected:
    void onQueryState() override;
    void onQueryLensNames() override;
    void onQueryShutterState() override;
    void onSetAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us) override;
    void onSetLensName(const std::string& lens_name) override;
    void onSetNumOfBackgrounds(int num_backgrounds) override;
    void onCalcBackground() override;
    void onStopBackground() override;

    void onOpenShutter() override;
    void onCloseShutter() override;

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


