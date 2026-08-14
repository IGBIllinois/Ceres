
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

const std::string& cAxisCommunicationsController::instance() const
{
    return mpModel->instance();
}

bool cAxisCommunicationsController::has_instance() const
{
    return mpModel->has_instance();
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

const std::string& cAxisCommunicationsController_F44::instance() const
{
    return mpModel->instance();
}

bool cAxisCommunicationsController_F44::has_instance() const
{
    return mpModel->has_instance();
}

void cAxisCommunicationsController_F44::photoTaken()
{
    sendTakePhotoReply();
}

void cAxisCommunicationsController_F44::modeChanged(int mode)
{
    sendCameraMode(mode);
}

void cAxisCommunicationsController_F44::cameraIdChanged(int id)
{
    sendActiveCameraId(id);
}

void cAxisCommunicationsController_F44::lapseIntervalChanged(int interval_ms)
{
    sendLapseInterval_ms(interval_ms);
}

void cAxisCommunicationsController_F44::frameRateChanged(int rate_fps)
{
    sendFrameRate(rate_fps);
}

void cAxisCommunicationsController_F44::imageSizeChanged(int width, int height)
{
    sendImageSize(width, height);
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

void cAxisCommunicationsController_F44::onQueryMode()
{
    auto mode = mpModel->mode();
    sendCameraMode(static_cast<uint8_t>(mode));
}

void cAxisCommunicationsController_F44::onQueryState()
{
    auto mode = mpModel->mode();
    auto id = mpModel->getActiveCameraID();
    auto image_size = mpModel->getActiveImageSize();
    auto fps = mpModel->getActiveFramesRate_fps();
    auto interval_ms = mpModel->lapseInterval_ms();

    bool valid = (id >= 0) && (fps > 0);

    int minID = mpModel->getMinCameraID();
    int maxID = mpModel->getMaxCameraID();

    auto minFPS = mpModel->minFrameRate_fps();
    auto maxFPS = mpModel->maxFrameRate_fps();

    sendCurrentState(valid, mode, id, image_size.width, image_size.height, fps, interval_ms, minID, maxID, minFPS, maxFPS);
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

void cAxisCommunicationsController_F44::onQueryLapseInterval()
{
    sendLapseInterval_ms(mpModel->lapseInterval_ms());
}

void cAxisCommunicationsController_F44::onGrabImage()
{
    mpModel->requestImage();
}

void cAxisCommunicationsController_F44::onTakePhoto(bool updateView)
{
    mpModel->takePhoto(updateView);
}

void cAxisCommunicationsController_F44::onTakePhoto(bool updateView, bool autoSave)
{
    mpModel->takePhoto(updateView, autoSave);
}

void cAxisCommunicationsController_F44::setMode(uint8_t mode)
{
    emit requestMode(mode);
}

void cAxisCommunicationsController_F44::setCameraId(uint8_t id)
{
    emit requestCameraID(id);
}

void cAxisCommunicationsController_F44::setImageSize(uint16_t width, uint16_t height)
{
    emit requestImageSize(width, height);
}

void cAxisCommunicationsController_F44::setFrameRate(uint8_t fps)
{
    emit requestFrameRate_Hz(fps);
}
void cAxisCommunicationsController_F44::setLapseInterval_ms(uint32_t interval_ms)
{
    emit requestLapseInterval_ms(interval_ms);
}

void cAxisCommunicationsController_F44::onSaveState()
{
    emit requestSaveState();
}

void cAxisCommunicationsController_F44::onRestoreState()
{
    emit requestRestoreState();
}


