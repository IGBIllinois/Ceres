
#include "AxisCommunicationsController.hpp"
#include "AxisCommunicationsModel.hpp"
#include "AxisCommunicationsModel_F44.hpp"

#include <cassert>


cAxisCommunicationsController::cAxisCommunicationsController(cAxisCommunicationsModel* model, QObject* parent)
    :
    cSensorController(parent), cAxisControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cAxisCommunicationsController::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cAxisCommunicationsController::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cAxisCommunicationsController::model() const
{
    return mpModel->model();
}

const std::string& cAxisCommunicationsController::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cAxisCommunicationsController::name() const
{
    return mpModel->name();
}

void cAxisCommunicationsController::processStream(const void* pBuffer, std::size_t buf_length)
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

void cAxisCommunicationsController::onQueryState()
{
//    auto window = mpModel->getAzimuthWindow();
//    auto mode = mpModel->getLidarMode();

//    sendCurrentState(true, mode, window.min_deg, window.max_deg);
}

/*
void cAxisCommunicationsController::onSetAzimuthWindow(double min_deg, double max_deg)
{
    if (max_deg < min_deg)
        std::swap(min_deg, max_deg);

    mpModel->setAzimuthWindow(min_deg, max_deg);
}

void cAxisCommunicationsController::onSetMode(ouster::eLIDAR_MODE mode)
{
    mpModel->setLidarMode(mode);
}
*/

/******************************************************************************
 * Axis Communications Controller F44
 ******************************************************************************/

cAxisCommunicationsController_F44::cAxisCommunicationsController_F44(cAxisCommunicationsModel_F44* model, QObject* parent)
    :
    cSensorController(parent), cAxisControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cAxisCommunicationsController_F44::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cAxisCommunicationsController_F44::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cAxisCommunicationsController_F44::model() const
{
    return mpModel->model();
}

const std::string& cAxisCommunicationsController_F44::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cAxisCommunicationsController_F44::name() const
{
    return mpModel->name();
}

void cAxisCommunicationsController_F44::processStream(const void* pBuffer, std::size_t buf_length)
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

void cAxisCommunicationsController_F44::onQueryState()
{
    auto id = mpModel->getActiveCameraID();
    auto image_size = mpModel->getActiveImageSize();
    auto fps = mpModel->getActiveFramesRate_fps();

    bool valid = (id >= 0) && (fps > 0);

    sendCurrentState(valid, id, image_size.width, image_size.height, fps);
}

void cAxisCommunicationsController_F44::onQueryCameraId()
{
    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cAxisCommunicationsController_F44::onQueryImageSize()
{
    auto image_size = mpModel->getActiveImageSize();
    sendImageSize(image_size.width, image_size.height);
}

void cAxisCommunicationsController_F44::onQueryFrameRate()
{
    sendFrameRate(mpModel->getActiveFramesRate_fps());
}

void cAxisCommunicationsController_F44::setCameraId(uint8_t id)
{
    mpModel->setActiveCamera(id);
    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cAxisCommunicationsController_F44::setImageSize(uint16_t width, uint16_t height)
{
    axis::sImageSize_t image_size = {width, height};

    mpModel->setActiveImageSize(image_size);
    sendImageSize(image_size.width, image_size.height);
}

void cAxisCommunicationsController_F44::setFrameRate(uint8_t fps)
{
    mpModel->setActiveFramesRate_fps(fps);
    sendFrameRate(mpModel->getActiveFramesRate_fps());
}


