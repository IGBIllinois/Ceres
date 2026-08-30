
#include "LucidVisionLabsRgbController.hpp"
#include "LucidVisionLabsRgbModel.hpp"
#include "LucidVisionLabsRgbModel_Triton.hpp"

#include <cassert>

/*
cLucidVisionLabsRgbController::cLucidVisionLabsRgbController(cLucidVisionLabsRgbModel* model, QObject* parent)
    :
    cSensorController(parent), cLucidRgbControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}
*/
cLucidVisionLabsRgbController::cLucidVisionLabsRgbController(QObject* parent)
    :
    cSensorController(parent), cLucidRgbControllerNetEncoder(512)
{}

/*
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
*/

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

/**
 * Message Handlers
 */


/******************************************************************************
 * Lucid Vision Labs Controller Triton
 ******************************************************************************/

/*
cLucidVisionLabsRgbController_Triton::cLucidVisionLabsRgbController_Triton(cLucidVisionLabsRgbModel_Triton* model, QObject* parent)
    :
    cSensorController(parent), cLucidRgbControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}
*/
cLucidVisionLabsRgbController_Triton::cLucidVisionLabsRgbController_Triton(QObject* parent)
    :
    cSensorController(parent), cLucidRgbControllerNetEncoder(512)
{
}

/*
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
*/

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

/**
 * Message Handlers
 */

void cLucidVisionLabsRgbController_Triton::onQueryStateMessage()
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

void cLucidVisionLabsRgbController_Triton::onQueryModeMessage()
{
//    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cLucidVisionLabsRgbController_Triton::onQueryImageSizeMessage()
{
/*
    auto image_size = mpModel->getActiveImageSize();
    sendImageSize(image_size.width, image_size.height);
*/
}

void cLucidVisionLabsRgbController_Triton::onQueryFrameRateMessage()
{
//    sendFrameRate(mpModel->getActiveFramesRate_fps());
}

void cLucidVisionLabsRgbController_Triton::onQueryLapseIntervalMessage()
{
    //    sendFrameRate(mpModel->getActiveFramesRate_fps());
}

void cLucidVisionLabsRgbController_Triton::onGrabImageMessage()
{
//    mpModel->requestImage();
}

void cLucidVisionLabsRgbController_Triton::onTakePhotoMessage(bool updateView)
{}

void cLucidVisionLabsRgbController_Triton::onTakePhotoMessage(bool updateView, bool autoSave)
{}

void cLucidVisionLabsRgbController_Triton::setModeMessage(uint8_t mode)
{
//    mpModel->setActiveCamera(id);
//    sendActiveCameraId(mpModel->getActiveCameraID());
}

void cLucidVisionLabsRgbController_Triton::setImageSizeMessage(uint16_t width, uint16_t height)
{
    rgb::sImageSize_t image_size = {width, height};

//    mpModel->setActiveImageSize(image_size);
//    sendImageSize(image_size.width, image_size.height);
}

void cLucidVisionLabsRgbController_Triton::setFrameRateMessage(uint8_t fps)
{
//    mpModel->setActiveFramesRate_fps(fps);
//    sendFrameRate(mpModel->getActiveFramesRate_fps());
}

void cLucidVisionLabsRgbController_Triton::setLapseIntervalMessage(uint32_t interval_ms)
{
    //    mpModel->setActiveFramesRate_fps(fps);
    //    sendFrameRate(mpModel->getActiveFramesRate_fps());
}

void cLucidVisionLabsRgbController_Triton::onSaveStateMessage()
{}

void cLucidVisionLabsRgbController_Triton::onRestoreStateMessage()
{}


