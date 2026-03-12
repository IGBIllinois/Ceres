
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

void cTeledyneFlirController::photoTaken()
{
    sendTakePhotoReply();
}

void cTeledyneFlirController::modeChanged(int mode)
{
    sendCameraMode(mode);
}

void cTeledyneFlirController::frameIntervalChanged(int interval_ms)
{
    sendFrameInterval_ms(interval_ms);
}

void cTeledyneFlirController::frameRateChanged(double rate_fps)
{
    sendFrameRate_Hz(rate_fps);
}

void cTeledyneFlirController::imageSizeChanged(int width, int height)
{
    sendImageSize(width, height);
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

void cTeledyneFlirController::onQueryMode()
{
    sendCameraMode(static_cast<uint8_t>(mpModel->mode()));
}

void cTeledyneFlirController::onQueryImageSize()
{
    auto width = mpModel->imageWidth();
    auto height = mpModel->imageHeight();

    sendImageSize(width, height);
}

void cTeledyneFlirController::onQueryFrameRate()
{
    sendFrameRate_Hz(mpModel->frameRate_Hz());
}

void cTeledyneFlirController::onQueryFrameInterval()
{
    sendFrameInterval_ms(mpModel->frameInterval_ms());
}


void cTeledyneFlirController::setMode(uint8_t mode)
{
    emit requestMode(mode);
}

void cTeledyneFlirController::setImageSize(uint16_t width, uint16_t height)
{
    width = mpModel->imageWidth();
    height = mpModel->imageHeight();

    sendImageSize(width, height);
}

void cTeledyneFlirController::setFrameRate_Hz(double fps)
{
    emit requestFrameRate_Hz(fps);
}

void cTeledyneFlirController::setFrameInterval_ms(uint32_t interval_ms)
{
    emit requestFrameInterval_ms(interval_ms);
}


/******************************************************************************
 * Teledyne FLIR Controller T1K
 ******************************************************************************/

cTeledyneFlirController_T1K::cTeledyneFlirController_T1K(cTeledyneFlirCameraModel_T1K* model, QObject* parent)
    :
    cTeledyneFlirController(model, parent), mpModel(model)
{
    assert(mpModel);
}

void cTeledyneFlirController_T1K::onPhotoTaken()
{
    sendTakePhotoReply();
}


void cTeledyneFlirController_T1K::onQueryState()
{
    auto mode = mpModel->mode();

    auto width = mpModel->imageWidth();
    auto height = mpModel->imageHeight();

    double fps = mpModel->frameRate_Hz();
    uint32_t interval_ms = mpModel->frameInterval_ms();

    sendCurrentState(true, static_cast<uint8_t>(mode), width, height, fps, interval_ms,
        mpModel->minFrameRate_fps(), mpModel->maxFrameRate_fps(),
        mpModel->minThermalValue_K(), mpModel->maxThermalValue_K());
}

void cTeledyneFlirController_T1K::onQueryThermalRange()
{
    float minValue_K = mpModel->minThermalValue_K().value_or(-1.0f);
    float maxValue_K = mpModel->maxThermalValue_K().value_or(-1.0f);

    sendThermalRange_K(minValue_K, maxValue_K);
}

void cTeledyneFlirController_T1K::onGrabImage()
{
    if (mpModel->mode() == cTeledyneFlirCameraModel::SINGLE)
        emit requestPhoto(true);
    else
        emit requestImage();
}

void cTeledyneFlirController_T1K::onTakePhoto(bool update_view)
{
    emit requestPhoto(update_view);
}

void cTeledyneFlirController_T1K::setThermalRange_K(float min_value_K, float max_value_K)
{
    float minValue_K = mpModel->minThermalValue_K().value_or(-1.0f);
    float maxValue_K = mpModel->maxThermalValue_K().value_or(-1.0f);

    sendThermalRange_K(minValue_K, maxValue_K);
}


