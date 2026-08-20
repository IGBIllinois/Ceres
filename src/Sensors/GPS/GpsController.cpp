
#include "GpsController.hpp"
#include "GpsModel.hpp"
#include "GpsControllerNetEncoder.hpp"

#include <cassert>


cGpsController::cGpsController(cGpsModel* model, QObject* parent)
    :
    cSensorController(parent), cGpsControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

void cGpsController::connectToModel()
{
    connect(mpModel, &cGpsModel::referenceComplete,          this, &cGpsController::onReferenceComplete);
    connect(mpModel, &cGpsModel::referenceStateChanged,      this, &cGpsController::referenceStateUpdated);
    connect(mpModel, &cGpsModel::referenceParametersChanged, this, &cGpsController::referenceParametersUpdated);
    connect(mpModel, &cGpsModel::referenceDataChanged,       this, &cGpsController::referenceDataUpdated);
    connect(mpModel, &cGpsModel::referencePositionChanged,   this, &cGpsController::referencePositionUpdated);

    connect(this, &cGpsController::queryReferenceState,       mpModel, &cGpsModel::referenceStateQueried);
    connect(this, &cGpsController::queryReferenceParameters,  mpModel, &cGpsModel::referenceParametersQueried);
    connect(this, &cGpsController::queryReferenceData,        mpModel, &cGpsModel::referenceDataQueried);
    connect(this, &cGpsController::updateReferenceParameters, mpModel, &cGpsModel::updateReferenceParameters);
    connect(this, &cGpsController::startReferenceComputation, mpModel, &cGpsModel::startReferenceComputation);
    connect(this, &cGpsController::abortReferenceCompute,     mpModel, &cGpsModel::abortReferenceCompute);
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

const std::string& cGpsController::instance() const
{
    return mpModel->instance();
}

bool cGpsController::has_instance() const
{
    return mpModel->has_instance();
}

void cGpsController::onReferenceComplete()
{
    sendReferenceState(gps::eReferenceState::COMPLETE_GOOD);
}

void cGpsController::referenceStateUpdated(::gps::eReferenceState state)
{
    sendReferenceState(state);
}

void cGpsController::referenceParametersUpdated(int min_integration_time_sec, int max_integration_time_sec, int ref_error_threshold_mm)
{
    sendReferenceParameters(min_integration_time_sec, max_integration_time_sec, ref_error_threshold_mm);
}

void cGpsController::referenceDataUpdated(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
    double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{
    sendReferenceData(valid, avg_lat_rad, avg_lng_rad, avg_height_m,
                        std_lat_rad, std_lng_rad, std_height_m, height_valid);
}

void cGpsController::referencePositionUpdated(int x_mm, int y_mm, int z_mm, double error_mm, int count)
{
    sendReferencePosition(x_mm, y_mm, z_mm, error_mm, count);
}

void cGpsController::onQueryReferenceData()
{
    emit queryReferenceData();
}

void cGpsController::onQueryReferenceParameters()
{
    emit queryReferenceParameters();
}

void cGpsController::onQueryReferenceState()
{
    emit queryReferenceState();
}

void cGpsController::onCalcReference()
{
    emit startReferenceComputation();
}

void cGpsController::onStopReference()
{
    emit abortReferenceCompute();
}

void cGpsController::onSetReferenceParameters(std::uint16_t min_integration_time_sec,
    std::uint16_t max_integration_time_sec, std::uint16_t error_threshold_mm)
{
    emit updateReferenceParameters(min_integration_time_sec, max_integration_time_sec, error_threshold_mm);
}

void cGpsController::processStream(const void* pBuffer, std::size_t buf_length)
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

        cGpsControllerNetDecoder::processPacket(hdr, buffer);
    }
}
