
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

const std::string& cOusterController::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cOusterController::model() const
{
    return mpModel->model();
}

const std::string& cOusterController::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cOusterController::name() const
{
    return mpModel->name();
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

void cOusterController::onQueryLidarMode()
{
    auto mode = mpModel->getLidarMode();

    sendLidarMode(mode);
}

void cOusterController::onQueryAzimuthWindow()
{
    auto window = mpModel->getAzimuthWindow();

    sendAzimuthWindow(window.min_deg, window.max_deg);
}

void cOusterController::setAzimuthWindow(double min_deg, double max_deg)
{
    if (max_deg < min_deg)
        std::swap(min_deg, max_deg);

    mpModel->setAzimuthWindow(min_deg, max_deg);

    auto window = mpModel->getAzimuthWindow();
    sendAzimuthWindow(window.min_deg, window.max_deg);
}

void cOusterController::setLidarMode(ouster::eLIDAR_MODE mode)
{
    mpModel->setLidarMode(mode);

    sendLidarMode(mpModel->getLidarMode());
}


