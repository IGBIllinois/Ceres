
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

void cGpsController::txReferenceData(cGpsControllerNetEncoder* encoder)
{
    auto pos = mpModel->getReferencePosition();

    bool valid = pos.valid;
    double avg_lat_rad = pos.avgLatitude_rad;
    double avg_lng_rad = pos.avgLongitude_rad;
    double avg_height_m = pos.avgHeight_m;
    double std_lat_rad = pos.stdLatitude_rad;
    double std_lng_rad = pos.stdLongitude_rad;
    double std_height_m = pos.stdHeight_m;
    bool height_valid = pos.heightValid;

    encoder->sendReferenceData(valid, avg_lat_rad, avg_lng_rad, avg_height_m,
        std_lat_rad, std_lng_rad, std_height_m, height_valid);
}

void cGpsController::txReferenceParameters(cGpsControllerNetEncoder* encoder)
{
    int min_integration_time_sec = mpModel->getRefMinIntegrationTime_sec();
    int max_integration_time_sec = mpModel->getRefMaxIntegrationTime_sec();
    int ref_error_threshold_mm = mpModel->getRefErrorThreshold_mm();

    encoder->sendReferenceParameters(min_integration_time_sec, max_integration_time_sec, ref_error_threshold_mm);
}

void cGpsController::txReferenceState(cGpsControllerNetEncoder* encoder)
{
    auto state = mpModel->getReferenceState();

    encoder->sendReferenceState(state);
}

void cGpsController::txReferenceState(cGpsControllerNetEncoder* encoder, gps::eReferenceState state)
{
    encoder->sendReferenceState(state);
}


