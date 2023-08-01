
#include "HySpexCamera_Controller.hpp"
#include "HySpexCamera_Model.hpp"
#include "HySpexCamera_ControllerNetEncoder.hpp"

#include <cassert>


cHySpexCamera_Controller::cHySpexCamera_Controller(cHySpexCameraModel* model, QObject* parent)
    :
    cSensorController(parent), mpModel(model)
{
    assert(mpModel);
}

const char* cHySpexCamera_Controller::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cHySpexCamera_Controller::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cHySpexCamera_Controller::model() const
{
    return mpModel->model();
}

const std::string& cHySpexCamera_Controller::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cHySpexCamera_Controller::name() const
{
    return mpModel->name();
}

void cHySpexCamera_Controller::txCurrentState(cHySpexCamera_ControllerNetEncoder* encoder)
{
    std::uint16_t average_frames = mpModel->getAverageFrames();
    std::uint32_t frame_period_us = mpModel->getFramePeriod_us();
    std::uint32_t min_frame_period_us = mpModel->getMinFramePeriod_us();
    std::uint32_t integration_time_us = mpModel->getIntegrationTime_us();
    std::uint32_t max_integration_time_us = mpModel->getMaxIntegrationTime_us();
    std::uint32_t num_backgrounds = mpModel->getNumOfBackgrounds();
    std::string lens_name = mpModel->getLensName();

    encoder->sendCurrentState(true, average_frames, frame_period_us, min_frame_period_us,
        integration_time_us, max_integration_time_us, num_backgrounds, lens_name);
}

void cHySpexCamera_Controller::txLensNames(cHySpexCamera_ControllerNetEncoder* encoder)
{
    encoder->sendLensNames(mpModel->getLensNames());
}

void cHySpexCamera_Controller::txShutterState(cHySpexCamera_ControllerNetEncoder* encoder)
{
    switch (mpModel->getShutterStatus())
    {
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_OPEN:
        encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_OPEN);
        break;
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_CLOSED:
        encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_CLOSED);
        break;
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_FAIL_CLOSE:
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_FAIL_OPEN:
        encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_ERROR);
        break;
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_UNKNOWN:
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_PENDING_OPEN:
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_PENDING_CLOSE:
        encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_UNKNOWN);
        break;
    }
}

/*
void cHySpexCamera_Controller::processStream(const void* pBuffer, std::size_t buf_length)
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

void cHySpexCamera_Controller::onQueryState()
{
    std::uint16_t average_frames = mpModel->getAverageFrames();
    std::uint32_t frame_period_us = mpModel->getFramePeriod_us();
    std::uint32_t min_frame_period_us = mpModel->getMinFramePeriod_us();
    std::uint32_t integration_time_us = mpModel->getIntegrationTime_us();
    std::uint32_t max_integration_time_us = mpModel->getMaxIntegrationTime_us();
    std::uint32_t num_backgrounds = mpModel->getNumOfBackgrounds();
    std::string lens_name = mpModel->getLensName();

    sendCurrentState(true, average_frames, frame_period_us, min_frame_period_us,
        integration_time_us, max_integration_time_us, num_backgrounds, lens_name);
}

void cHySpexCamera_Controller::onQueryLensNames()
{

}
*/

