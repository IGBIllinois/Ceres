
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
}

void cSsnxController::onQueryReferenceParameters()
{
}

void cSsnxController::onSetReferenceParameters(std::uint16_t integration_time_sec, std::uint16_t max_integration_time_sec)
{
}

void cSsnxController::onCalcReference()
{
}

void cSsnxController::onStopReference()
{
}

void cSsnxController::onReferenceComplete()
{
}


