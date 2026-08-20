
#include "HySpexSWIR_384_Controller.hpp"
#include "HySpexSWIR_384_Model.hpp"

#include <cassert>


cHySpexSWIR_384_Controller::cHySpexSWIR_384_Controller(cHySpexSWIR_384_Model* model, QObject* parent)
    :
    cHySpexCamera_Controller(model, parent), cHySpexSWIR_384_ControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

void cHySpexSWIR_384_Controller::connectToModel()
{
    connect(mpModel, &cHySpexSWIR_384_Model::backgroundComplete, this, &cHySpexSWIR_384_Controller::onBackgroundComplete);
}

void cHySpexSWIR_384_Controller::processStream(const void* pBuffer, std::size_t buf_length)
{
    if (!pBuffer)
        return;

    net_buffer_view buffer(reinterpret_cast<const std::byte*>(pBuffer), buf_length);

    sPacketHeader_t hdr;

    while (buffer.size() > 0)
    {
        buffer >> hdr;

        if (buf_length < hdr.length)
        {
            break;
        }

        cHySpexCamera_ControllerNetDecoder::processPacket(hdr, buffer);
    }
}

void cHySpexSWIR_384_Controller::onBackgroundComplete()
{
    auto status = mpModel->getBackgroundStatus();
    switch(status)
    {
    case hyspex::BackgroundStatus::HYSPEX_BG_VALID:
        sendBackgroundReply(hyspex_eBackgroundReply::eBackgroundReply_GOOD);
        break;
    case hyspex::BackgroundStatus::HYSPEX_BG_ABORTED:
        sendBackgroundReply(hyspex_eBackgroundReply::eBackgroundReply_ABORTED);
        break;
    default:
        sendBackgroundReply(hyspex_eBackgroundReply::eBackgroundReply_FAILED);
        break;
    }
}

void cHySpexSWIR_384_Controller::onQueryState()
{
    txCurrentState(this);
}

void cHySpexSWIR_384_Controller::onQueryLensNames()
{
    txLensNames(this);
}

void cHySpexSWIR_384_Controller::onQueryShutterState()
{
    txShutterState(this);
}

void cHySpexSWIR_384_Controller::onSetAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    mpModel->setAcquisitionParameters(average_frame, frame_period_us, integration_time_us);
}

void cHySpexSWIR_384_Controller::onSetLensName(const std::string& lens_name)
{}

void cHySpexSWIR_384_Controller::onSetNumOfBackgrounds(int num_backgrounds)
{
    mpModel->setNumOfBackgrounds(num_backgrounds);
}

void cHySpexSWIR_384_Controller::onCalcBackground()
{
    mpModel->calcBackground();
}

void cHySpexSWIR_384_Controller::onStopBackground()
{
    mpModel->stopBackground();
}

void cHySpexSWIR_384_Controller::onOpenShutter()
{
    mpModel->open_shutter();
}

void cHySpexSWIR_384_Controller::onCloseShutter()
{
    mpModel->close_shutter();
}


