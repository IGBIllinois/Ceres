
#include "GpsController.hpp"
#include "GpsModel.hpp"
#include "GpsControllerNetEncoder.hpp"

#include <cassert>


cGpsController::cGpsController(QObject* parent)
    :
    cSensorController(parent), cGpsControllerNetEncoder(512)
{}

void cGpsController::onReferenceComplete()
{
    sendReferenceStateMessage(gps::eReferenceState::COMPLETE_GOOD);
}

void cGpsController::referenceStateUpdated(::gps::eReferenceState state)
{
    sendReferenceStateMessage(state);
}

void cGpsController::referenceParametersUpdated(int min_integration_time_sec, int max_integration_time_sec, int ref_error_threshold_mm)
{
    sendReferenceParametersMessage(min_integration_time_sec, max_integration_time_sec, ref_error_threshold_mm);
}

void cGpsController::referenceDataUpdated(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
    double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{
    sendReferenceDataMessage(valid, avg_lat_rad, avg_lng_rad, avg_height_m,
                        std_lat_rad, std_lng_rad, std_height_m, height_valid);
}

void cGpsController::referencePositionUpdated(int x_mm, int y_mm, int z_mm, double error_mm, int count)
{
    sendReferencePositionMessage(x_mm, y_mm, z_mm, error_mm, count);
}

void cGpsController::onQueryReferenceDataMessage()
{
    emit queryReferenceData();
}

void cGpsController::onQueryReferenceParametersMessage()
{
    emit queryReferenceParameters();
}

void cGpsController::onQueryReferenceStateMessage()
{
    emit queryReferenceState();
}

void cGpsController::onCalcReferenceMessage()
{
    emit startReferenceComputation();
}

void cGpsController::onStopReferenceMessage()
{
    emit abortReferenceCompute();
}

void cGpsController::onSetReferenceParametersMessage(std::uint16_t min_integration_time_sec,
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
