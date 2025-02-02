
#include "GpsController.hpp"
#include "GpsModel.hpp"
#include "GpsControllerNetEncoder.hpp"

#include <cassert>


cGpsController::cGpsController(cGpsModel* model, QObject* parent)
    :
    cSensorController(parent), mpModel(model)
{
    assert(mpModel);
}

const char* cGpsController::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cGpsController::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cGpsController::model() const
{
    return mpModel->model();
}

const std::string& cGpsController::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cGpsController::name() const
{
    return mpModel->name();
}

void cGpsController::txCurrentState(cGpsControllerNetEncoder* encoder)
{
    //std::uint16_t average_frames = mpModel->getAverageFrames();
    //std::uint32_t frame_period_us = mpModel->getFramePeriod_us();
    //std::uint32_t min_frame_period_us = mpModel->getMinFramePeriod_us();
    //std::uint32_t integration_time_us = mpModel->getIntegrationTime_us();
    //std::uint32_t max_integration_time_us = mpModel->getMaxIntegrationTime_us();
    //std::uint32_t num_backgrounds = mpModel->getNumOfBackgrounds();
    //std::string lens_name = mpModel->getLensName();

    //encoder->sendCurrentState(true, average_frames, frame_period_us, min_frame_period_us,
    //    integration_time_us, max_integration_time_us, num_backgrounds, lens_name);
}

void cGpsController::txLensNames(cGpsControllerNetEncoder* encoder)
{
    //encoder->sendLensNames(mpModel->getLensNames());
}

void cGpsController::txShutterState(cGpsControllerNetEncoder* encoder)
{
    //switch (mpModel->getShutterStatus())
    //{
    //case hyspex::ShutterStatus::HYSPEX_SHUTTER_OPEN:
    //    encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_OPEN);
    //    break;
    //case hyspex::ShutterStatus::HYSPEX_SHUTTER_CLOSED:
    //    encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_CLOSED);
    //    break;
    //case hyspex::ShutterStatus::HYSPEX_SHUTTER_FAIL_CLOSE:
    //case hyspex::ShutterStatus::HYSPEX_SHUTTER_FAIL_OPEN:
    //    encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_ERROR);
    //    break;
    //case hyspex::ShutterStatus::HYSPEX_SHUTTER_UNKNOWN:
    //case hyspex::ShutterStatus::HYSPEX_SHUTTER_PENDING_OPEN:
    //case hyspex::ShutterStatus::HYSPEX_SHUTTER_PENDING_CLOSE:
    //    encoder->sendShutterStateReply(hyspex_eShutterState::eShutterState_UNKNOWN);
    //    break;
    //}
}



