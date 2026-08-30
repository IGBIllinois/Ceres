
#include "TeledyneFlirController.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirCameraModel_T1K.hpp"

#include <cassert>

/*
cTeledyneFlirController::cTeledyneFlirController(cTeledyneFlirCameraModel* model, QObject* parent)
    :
    cSensorController(parent), cTeledyneFlirControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}
*/

cTeledyneFlirController::cTeledyneFlirController(QObject* parent)
    :
    cSensorController(parent), cTeledyneFlirControllerNetEncoder(512)
{}

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

/*
 * Slots
 */
void cTeledyneFlirController::photoTaken()
{
    sendTakePhotoReplyMessage();
}

void cTeledyneFlirController::modeChanged(int mode)
{
    sendCameraModeMessage(mode);
}

void cTeledyneFlirController::lapseIntervalChanged(int interval_ms)
{
    sendLapseIntervalMessage(interval_ms);
}

void cTeledyneFlirController::frameRateChanged(double rate_fps)
{
    sendFrameRateMessage(rate_fps);
}

void cTeledyneFlirController::imageSizeChanged(int width, int height)
{
    sendImageSizeMessage(width, height);
}

/*
 * Message Handlers
 */
void cTeledyneFlirController::onQueryModeMessage()
{
    emit queryMode();
}

void cTeledyneFlirController::onQueryImageSizeMessage()
{
    emit queryImageSize();
}

void cTeledyneFlirController::onQueryFrameRateMessage()
{
    emit queryFrameRate_Hz();
}

void cTeledyneFlirController::onQueryLapseIntervalMessage()
{
    emit queryLapseInterval_ms();
}


void cTeledyneFlirController::onSetModeMessage(uint8_t mode)
{
    emit requestMode(mode);
}

void cTeledyneFlirController::onSetImageSizeMessage(uint16_t width, uint16_t height)
{
    emit requestImageSize(width, height);
}

void cTeledyneFlirController::onSetFrameRateMessage(double fps)
{
    emit requestFrameRate_Hz(fps);
}

void cTeledyneFlirController::onSetLapseIntervalMessage(uint32_t interval_ms)
{
    emit requestLapseInterval_ms(interval_ms);
}

void cTeledyneFlirController::onSaveStateMessage()
{
    emit requestSaveState();
}

void cTeledyneFlirController::onRestoreStateMessage()
{
    emit requestRestoreState();
}


/******************************************************************************
 * Teledyne FLIR Controller T1K
 ******************************************************************************/

cTeledyneFlirController_T1K::cTeledyneFlirController_T1K(QObject* parent)
    :
    cTeledyneFlirController(parent)
{}

/*
 * Slots
 */
void cTeledyneFlirController_T1K::stateUpdated(int mode, int width, int height, double fps, double min_fps, double max_fps,
    int interval_ms, float minValue_K, float maxValue_K)
{
    std::optional<double> minFrameRate_fps;
    std::optional<double> maxFrameRate_fps;
    std::optional<float> minThermalValue_K;
    std::optional<float> maxThermalValue_K;

    if (min_fps > 1.0)
        minFrameRate_fps = min_fps;

    if (max_fps > 1.0)
        maxFrameRate_fps = max_fps;

    if (minValue_K > 1.0)
        minThermalValue_K = minValue_K;

    if (maxValue_K > 1.0)
        maxThermalValue_K = maxValue_K;

    sendCurrentStateMessage(true, static_cast<uint8_t>(mode), width, height, fps, interval_ms,
        minFrameRate_fps, maxFrameRate_fps, minThermalValue_K, maxThermalValue_K);
}

void cTeledyneFlirController_T1K::thermalRangeUpdated(float minValue_K, float maxValue_K)
{
    sendThermalRangeMessage(minValue_K, maxValue_K);
}

void cTeledyneFlirController_T1K::onPhotoTaken()
{
    sendTakePhotoReplyMessage();
}

/*
 * Message Handlers
 */
void cTeledyneFlirController_T1K::onQueryStateMessage()
{
    emit queryState();
}

void cTeledyneFlirController_T1K::onQueryThermalRangeMessage()
{
    emit queryThermalRange();
}

void cTeledyneFlirController_T1K::onGrabImageMessage()
{
    emit grabImage();
}

void cTeledyneFlirController_T1K::onTakePhotoMessage(bool update_view)
{
    emit requestPhoto(update_view);
}

void cTeledyneFlirController_T1K::onTakePhotoMessage(bool updateView, bool autoSave)
{
    emit requestPhoto(updateView, autoSave);
}

void cTeledyneFlirController_T1K::onSetThermalRangeMessage(float min_value_K, float max_value_K)
{
    emit requestThermalRange(min_value_K, max_value_K);
}


