
#include "LucidVisionLabsRgbController.hpp"
#include "LucidVisionLabsRgbModel.hpp"
#include "LucidVisionLabsRgbModel_Triton.hpp"

#include <cassert>


cLucidVisionLabsRgbController::cLucidVisionLabsRgbController(cLucidVisionLabsRgbModel* model, QObject* parent)
    :
    cSensorController(parent), cLucidRgbControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cLucidVisionLabsRgbController::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cLucidVisionLabsRgbController::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cLucidVisionLabsRgbController::model() const
{
    return mpModel->model();
}

const std::string& cLucidVisionLabsRgbController::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cLucidVisionLabsRgbController::name() const
{
    return mpModel->name();
}

const std::string& cLucidVisionLabsRgbController::instance() const
{
    return mpModel->instance();
}

bool cLucidVisionLabsRgbController::has_instance() const
{
    return mpModel->has_instance();
}

void cLucidVisionLabsRgbController::processStream(const void* pBuffer, std::size_t buf_length)
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

void cLucidVisionLabsRgbController::onQueryState()
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
 * Lucid Vision Labs Controller Triton
 ******************************************************************************/

cLucidVisionLabsRgbController_Triton::cLucidVisionLabsRgbController_Triton(cLucidVisionLabsRgbModel_Triton* model, QObject* parent)
    :
    cSensorController(parent), cLucidRgbControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cLucidVisionLabsRgbController_Triton::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cLucidVisionLabsRgbController_Triton::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cLucidVisionLabsRgbController_Triton::model() const
{
    return mpModel->model();
}

const std::string& cLucidVisionLabsRgbController_Triton::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cLucidVisionLabsRgbController_Triton::name() const
{
    return mpModel->name();
}

const std::string& cLucidVisionLabsRgbController_Triton::instance() const
{
    return mpModel->instance();
}

bool cLucidVisionLabsRgbController_Triton::has_instance() const
{
    return mpModel->has_instance();
}

void cLucidVisionLabsRgbController_Triton::processStream(const void* pBuffer, std::size_t buf_length)
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

void cLucidVisionLabsRgbController_Triton::onQueryState()
{
/*
    auto id = mpModel->getActiveCameraID();
    auto image_size = mpModel->getActiveImageSize();
    auto fps = mpModel->getActiveFramesRate_fps();

    bool valid = (id >= 0) && (fps > 0);

    int minID = mpModel->getMinCameraID();
    int maxID = mpModel->getMaxCameraID();

    sendCurrentState(valid, id, image_size.width, image_size.height, fps, minID, maxID);
*/
}

void cLucidVisionLabsRgbController_Triton::onQueryCameraId()
{
//    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cLucidVisionLabsRgbController_Triton::onQueryImageSize()
{
/*
    auto image_size = mpModel->getActiveImageSize();
    sendImageSize(image_size.width, image_size.height);
*/
}

void cLucidVisionLabsRgbController_Triton::onQueryFrameRate()
{
//    sendFrameRate(mpModel->getActiveFramesRate_fps());
}

void cLucidVisionLabsRgbController_Triton::onGrabImage()
{
    mpModel->requestImage();
}

void cLucidVisionLabsRgbController_Triton::setCameraId(uint8_t id)
{
//    mpModel->setActiveCamera(id);
//    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cLucidVisionLabsRgbController_Triton::setImageSize(uint16_t width, uint16_t height)
{
    rgb::sImageSize_t image_size = {width, height};

//    mpModel->setActiveImageSize(image_size);
//    sendImageSize(image_size.width, image_size.height);
}

void cLucidVisionLabsRgbController_Triton::setFrameRate(uint8_t fps)
{
//    mpModel->setActiveFramesRate_fps(fps);
//    sendFrameRate(mpModel->getActiveFramesRate_fps());
}


