
#include "TeledyneFlirController.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirCameraModel_T1K.hpp"

#include <cassert>


cTeledyneFlirController::cTeledyneFlirController(cTeledyneFlirCameraModel* model, QObject* parent)
    :
    cSensorController(parent), cTeledyneFlirControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cTeledyneFlirController::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cTeledyneFlirController::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cTeledyneFlirController::model() const
{
    return mpModel->model();
}

const std::string& cTeledyneFlirController::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cTeledyneFlirController::name() const
{
    return mpModel->name();
}

const std::string& cTeledyneFlirController::instance() const
{
    return mpModel->instance();
}

bool cTeledyneFlirController::has_instance() const
{
    return mpModel->has_instance();
}

void cTeledyneFlirController::processStream(const void* pBuffer, std::size_t buf_length)
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

void cTeledyneFlirController::onQueryState()
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
 * Teledyne FLIR Controller T1K
 ******************************************************************************/

cTeledyneFlirController_T1K::cTeledyneFlirController_T1K(cTeledyneFlirCameraModel_T1K* model, QObject* parent)
    :
    cSensorController(parent), cTeledyneFlirControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cTeledyneFlirController_T1K::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cTeledyneFlirController_T1K::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cTeledyneFlirController_T1K::model() const
{
    return mpModel->model();
}

const std::string& cTeledyneFlirController_T1K::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cTeledyneFlirController_T1K::name() const
{
    return mpModel->name();
}

const std::string& cTeledyneFlirController_T1K::instance() const
{
    return mpModel->instance();
}

bool cTeledyneFlirController_T1K::has_instance() const
{
    return mpModel->has_instance();
}

void cTeledyneFlirController_T1K::processStream(const void* pBuffer, std::size_t buf_length)
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

void cTeledyneFlirController_T1K::onQueryState()
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

void cTeledyneFlirController_T1K::onQueryCameraId()
{
//    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cTeledyneFlirController_T1K::onQueryImageSize()
{
/*
    auto image_size = mpModel->getActiveImageSize();
    sendImageSize(image_size.width, image_size.height);
*/
}

void cTeledyneFlirController_T1K::onQueryFrameRate()
{
//    sendFrameRate(mpModel->getActiveFramesRate_fps());
}

void cTeledyneFlirController_T1K::onGrabImage()
{
//    mpModel->requestImage();
}

void cTeledyneFlirController_T1K::setCameraId(uint8_t id)
{
//    mpModel->setActiveCamera(id);
//    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cTeledyneFlirController_T1K::setImageSize(uint16_t width, uint16_t height)
{
//    rgb::sImageSize_t image_size = {width, height};

//    mpModel->setActiveImageSize(image_size);
//    sendImageSize(image_size.width, image_size.height);
}

void cTeledyneFlirController_T1K::setFrameRate(uint8_t fps)
{
//    mpModel->setActiveFramesRate_fps(fps);
//    sendFrameRate(mpModel->getActiveFramesRate_fps());
}


