
#pragma once

#include "HySpexCamera_Controller.hpp"
#include "HySpexSWIR_384_ControllerNetDecoder.hpp"
#include "HySpexSWIR_384_ControllerNetEncoder.hpp"

class cHySpexSWIR_384_Model;


class cHySpexSWIR_384_Controller : public cHySpexCamera_Controller,
    private cHySpexSWIR_384_ControllerNetDecoder, 
    private cHySpexSWIR_384_ControllerNetEncoder
{
	Q_OBJECT

public:
    cHySpexSWIR_384_Controller(cHySpexSWIR_384_Model* model, QObject* parent = nullptr);

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
    cHySpexSWIR_384_Model* mpModel = nullptr;
};


