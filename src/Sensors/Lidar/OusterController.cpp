
#include "OusterController.hpp"
#include "OusterModel.hpp"

#include <cassert>


cOusterController::cOusterController(cOusterModel* model, QObject* parent)
    :
    cSensorController(parent), cOusterControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cOusterController::descriptor() const
{
    return mpModel->descriptor();
}

void cOusterController::processStream(const void* pBuffer, std::size_t buf_length)
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

void cOusterController::onQueryState()
{
    auto window = mpModel->getAzimuthWindow();
    auto mode = mpModel->getLidarMode();

    sendCurrentState(true, mode, window.min_deg, window.max_deg);
}

void cOusterController::onSetAzimuthWindow(double min_deg, double max_deg)
{
    if (max_deg < min_deg)
        std::swap(min_deg, max_deg);

    mpModel->setAzimuthWindow(min_deg, max_deg);
}

void cOusterController::onSetMode(ouster::eLIDAR_MODE mode)
{
    mpModel->setLidarMode(mode);
}


