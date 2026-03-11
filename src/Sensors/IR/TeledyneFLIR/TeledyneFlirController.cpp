
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

void cTeledyneFlirController_T1K::onQueryMode()
{
    sendCameraMode(static_cast<uint8_t>(mpModel->mode()));
}

void cTeledyneFlirController_T1K::onQueryImageSize()
{
    auto width = mpModel->imageWidth();
    auto height = mpModel->imageHeight();

    sendImageSize(width, height);
}

void cTeledyneFlirController_T1K::onQueryFrameRate()
{
    sendFrameRate_Hz(mpModel->frameRate_Hz());
}

void cTeledyneFlirController_T1K::onQueryFrameInterval()
{
    sendFrameInterval_ms(mpModel->frameInterval_ms());
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
        mpModel->takePhoto(true);
    else
        mpModel->requestImage();
}

void cTeledyneFlirController_T1K::onTakePhoto(bool update_view)
{
    mpModel->takePhoto(update_view);
}

void cTeledyneFlirController_T1K::setMode(uint8_t mode)
{
    mpModel->setMode(static_cast<cTeledyneFlirCameraModel::eMode>(mode));
    sendCameraMode(static_cast<uint8_t>(mpModel->mode()));
}

void cTeledyneFlirController_T1K::setImageSize(uint16_t width, uint16_t height)
{
//    rgb::sImageSize_t image_size = {width, height};

//    mpModel->setActiveImageSize(image_size);

    width = mpModel->imageWidth();
    height = mpModel->imageHeight();

    sendImageSize(width, height);
}

void cTeledyneFlirController_T1K::setFrameRate_Hz(double fps)
{
    mpModel->setFrameRate_Hz(fps);
    sendFrameRate_Hz(mpModel->frameRate_Hz());
}

void cTeledyneFlirController_T1K::setFrameInterval_ms(uint32_t interval_ms)
{
    mpModel->setFrameInterval_ms(interval_ms);
    sendFrameInterval_ms(mpModel->frameInterval_ms());
}

void cTeledyneFlirController_T1K::setThermalRange_K(float min_value_K, float max_value_K)
{
    //    mpModel->setActiveFrameInterval_s(fps);

    float minValue_K = mpModel->minThermalValue_K().value_or(-1.0f);
    float maxValue_K = mpModel->maxThermalValue_K().value_or(-1.0f);

    sendThermalRange_K(minValue_K, maxValue_K);
}


