
#include "HySpexVNIR_3000N_Controller.hpp"
#include "HySpexVNIR_3000N_Model.hpp"

#include <cassert>


cHySpexVNIR_3000N_Controller::cHySpexVNIR_3000N_Controller(cHySpexVNIR_3000N_Model* model, QObject* parent)
    :
    cHySpexCamera_Controller(model, parent), cHySpexVNIR_3000N_ControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

void cHySpexVNIR_3000N_Controller::processStream(const void* pBuffer, std::size_t buf_length)
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

        processPacket(hdr, buffer);
    }
}

void cHySpexVNIR_3000N_Controller::onQueryState()
{
    txCurrentState(this);
}

void cHySpexVNIR_3000N_Controller::onQueryLensNames()
{
    txLensNames(this);
}

void cHySpexVNIR_3000N_Controller::onSetAverageFrames(std::uint32_t average_frame)
{
    mpModel->setAverageFrames(average_frame);
}

void cHySpexVNIR_3000N_Controller::onSetFramePeriod_us(std::uint32_t frame_period_us)
{
    mpModel->setFramePeriod_us(frame_period_us);
}

void cHySpexVNIR_3000N_Controller::onSetIntegrationTime_us(std::uint32_t integration_time_us)
{
    mpModel->setIntegrationTime_us(integration_time_us);
}

void cHySpexVNIR_3000N_Controller::onSetLensName(const std::string& lens_name)
{}

void cHySpexVNIR_3000N_Controller::onSetNumOfBackgrounds(int num_backgrounds)
{
    mpModel->setNumOfBackgrounds(num_backgrounds);
}

void cHySpexVNIR_3000N_Controller::onCalcBackground()
{
    mpModel->calcBackground();
}

