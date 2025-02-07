
#include "SsnxController.hpp"
#include "SsnxModel.hpp"
#include "../GpsControllerNetEncoder.hpp"

#include <cassert>


cSsnxController::cSsnxController(cSsnxModel* model, QObject* parent)
    :
    cGpsController(model, parent), cGpsControllerNetEncoder(512)
{
}

void cSsnxController::processStream(const void* pBuffer, std::size_t buf_length)
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

void cSsnxController::onQueryReferenceData()
{
    txReferenceData(this);
}

void cSsnxController::onQueryReferenceParameters()
{
    txReferenceParameters(this);
}

void cSsnxController::onQueryReferenceState()
{
    txReferenceState(this);
}

void cSsnxController::onSetReferenceParameters(std::uint16_t min_integration_time_sec, 
    std::uint16_t max_integration_time_sec, std::uint16_t error_threshold_mm)
{
    mpModel->setReferenceIntegrationTimes(min_integration_time_sec, max_integration_time_sec, error_threshold_mm);
}

void cSsnxController::onCalcReference()
{
    if (mpModel->getStatus() != sensor::eStatus::RUNNING)
    {
        txReferenceState(this, gps::eReferenceState::ABORT);
        return;
    }

    mpModel->startReferenceComputation();
    txReferenceState(this);
}

void cSsnxController::onStopReference()
{
    mpModel->abortReferenceCompute();
}

void cSsnxController::onReferenceComplete()
{
    txReferenceState(this);
}


