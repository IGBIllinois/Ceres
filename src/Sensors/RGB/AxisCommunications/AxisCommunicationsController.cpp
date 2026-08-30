
#include "AxisCommunicationsController.hpp"
#include "AxisCommunicationsModel.hpp"
#include "AxisCommunicationsModel_F44.hpp"

#include <cassert>


cAxisCommunicationsController::cAxisCommunicationsController(QObject* parent)
    :
    cSensorController(parent), cAxisControllerNetEncoder(512)
{
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

/******************************************************************************
 * Axis Communications Controller F44
 ******************************************************************************/

cAxisCommunicationsController_F44::cAxisCommunicationsController_F44(QObject* parent)
    :
    cSensorController(parent), cAxisControllerNetEncoder(512)
{}

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

/**
 * Slot Handlers
 */
void cAxisCommunicationsController_F44::photoTaken()
{
    sendTakePhotoReplyMessage();
}

void cAxisCommunicationsController_F44::modeChanged(int mode)
{
    sendCameraModeMessage(static_cast<uint8_t>(mode));
}

void cAxisCommunicationsController_F44::cameraIdChanged(int id)
{
    sendActiveCameraIdMessage(id);
}

void cAxisCommunicationsController_F44::lapseIntervalChanged(int interval_ms)
{
    sendLapseIntervalMessage(interval_ms);
}

void cAxisCommunicationsController_F44::frameRateChanged(int rate_fps)
{
    sendFrameRateMessage(rate_fps);
}

void cAxisCommunicationsController_F44::imageSizeChanged(int width, int height)
{
    sendImageSizeMessage(width, height);
}

void cAxisCommunicationsController_F44::stateUpdated(int mode, int id, int width, int height, double rate_fps, int interval_ms, int min_id, int max_id, double min_fps, double max_fps)
{
    bool valid = (id >= 0) && (rate_fps > 0);

    std::optional<double> minFPS;
    std::optional<double> maxFPS;

    if (min_fps > 0.0)
        minFPS = min_fps;

    if (max_fps > 0.0)
        maxFPS = max_fps;

    sendCurrentStateMessage(valid, static_cast<uint8_t>(mode), static_cast<uint8_t>(id), 
        static_cast<uint16_t>(width), static_cast<uint16_t>(height), rate_fps, interval_ms, 
        static_cast<uint8_t>(min_id), static_cast<uint8_t>(max_id), minFPS, maxFPS);
}


/**
 * Message Handlers
 */

void cAxisCommunicationsController_F44::onQueryModeMessage()
{
    emit queryMode();
}

void cAxisCommunicationsController_F44::onQueryStateMessage()
{
    emit queryState();
}

void cAxisCommunicationsController_F44::onQueryCameraIdMessage()
{
    emit queryCameraId();
}

void cAxisCommunicationsController_F44::onQueryImageSizeMessage()
{
    emit queryImageSize();
}

void cAxisCommunicationsController_F44::onQueryFrameRateMessage()
{
    emit queryFrameRate();
}

void cAxisCommunicationsController_F44::onQueryLapseIntervalMessage()
{
    emit queryLapseInterval();
}

void cAxisCommunicationsController_F44::onGrabImageMessage()
{
    emit requestImage();
}

void cAxisCommunicationsController_F44::onTakePhotoMessage(bool updateView)
{
    emit requestPhoto(updateView);
}

void cAxisCommunicationsController_F44::onTakePhotoMessage(bool updateView, bool autoSave)
{
    emit requestPhoto(updateView, autoSave);
}

void cAxisCommunicationsController_F44::setModeMessage(uint8_t mode)
{
    emit requestMode(mode);
}

void cAxisCommunicationsController_F44::setCameraIdMessage(uint8_t id)
{
    emit requestCameraID(id);
}

void cAxisCommunicationsController_F44::setImageSizeMessage(uint16_t width, uint16_t height)
{
    emit requestImageSize(width, height);
}

void cAxisCommunicationsController_F44::setFrameRateMessage(uint8_t fps)
{
    emit requestFrameRate_Hz(fps);
}
void cAxisCommunicationsController_F44::setLapseIntervalMessage(uint32_t interval_ms)
{
    emit requestLapseInterval_ms(interval_ms);
}

void cAxisCommunicationsController_F44::onSaveStateMessage()
{
    emit requestSaveState();
}

void cAxisCommunicationsController_F44::onRestoreStateMessage()
{
    emit requestRestoreState();
}


