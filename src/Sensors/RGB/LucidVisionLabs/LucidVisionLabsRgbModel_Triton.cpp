
#include "LucidVisionLabsRgbModel_Triton.hpp"

#include <LucidVisionLabsConnect/LucidTritonCamera.hpp>

#include <Arena/ArenaAPI.h>

#include "TimestampProvider.hpp"


#define USE_LOG_MESSAGE

namespace
{
    static uint8_t lucid_vision_labs_triton_instance_id = 0;
}


cLucidVisionLabsRgbModel_Triton::cLucidVisionLabsRgbModel_Triton(std::unique_ptr<cLucidTritonCamera> camera, QObject* parent)
:
    cLucidVisionLabsRgbModel("Triton Camera", parent),
    mInstanceID(++lucid_vision_labs_triton_instance_id), 
    mCamera(std::move(camera))
{
    mModel = mCamera->getModelName();
    mManufacturer = mCamera->getVendorName();
    mSerialNumber = mCamera->getSerialNumber();
    mFamilyName = mCamera->getFamilyName();
    mModelVersion = mCamera->getVersion();
    mFirmwareVersion = mCamera->getFirmwareVersion();
    mMacAddress = mCamera->getMacAddress();
    mIpAddress = mCamera->getIpAddress();

}

cLucidVisionLabsRgbModel_Triton::~cLucidVisionLabsRgbModel_Triton()
{
	stopCommunications();
}

uint8_t cLucidVisionLabsRgbModel_Triton::device_id() const
{
    return mInstanceID;
}

void cLucidVisionLabsRgbModel_Triton::setMode(eMode mode)
{
    bool changing = mode != mMode;

    mMode = mode;

    if (changing)
        emit modeChanged(static_cast<int>(mMode));
}

void cLucidVisionLabsRgbModel_Triton::setFrameRate_Hz(double frame_rate_hz)
{

}

void cLucidVisionLabsRgbModel_Triton::setLapseInterval_ms(uint32_t interval_ms)
{

}

void cLucidVisionLabsRgbModel_Triton::updateViews()
{
    emit modeChanged(static_cast<int>(mMode));
    emit lapseIntervalChanged(mLapseInterval_ms);
    emit frameRateChanged(mFrameRate_fps);
    emit imageSizeChanged(mImageWidth, mImageHeight);
}

bool cLucidVisionLabsRgbModel_Triton::configure(const nlohmann::json& jsonCfg)
{
    updateName(mModel);

    auto result = cLucidVisionLabsRgbModel::configure(jsonCfg);

    size_t buffer_size = mImageHeight * mImageWidth * sizeof(double);

    mSerializer.setBufferCapacity(buffer_size + 1024);

    if (result)
        setStatus(sensor::eStatus::CONFIGURED);
    else
        setStatus(sensor::eStatus::FAILED);

    return result;
}

bool cLucidVisionLabsRgbModel_Triton::initialize()
{
    return true;
}

void cLucidVisionLabsRgbModel_Triton::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cLucidVisionLabsRgbModel_Triton::disableDataRecording()
{
    cLucidVisionLabsRgbModel::disableDataRecording();
    mSerializer.detach();
}

void cLucidVisionLabsRgbModel_Triton::writeDataHeader()
{
//    mSerializer.writeActiveCameraId(mInstanceID, mpActiveCamera->cameraID());
//    auto size = mpActiveCamera->getImageSize();
//    mSerializer.writeImageSize(mInstanceID, size.width, size.height);
//    mSerializer.writeFramesPerSecond(mInstanceID, mpActiveCamera->getFramesPerSeconds());
}

bool cLucidVisionLabsRgbModel_Triton::startCommunications()
{
    if (mCamera->isConnected())
    {
        setStatus(sensor::eStatus::CONNECTED);

        return true;
    }

    setStatus(sensor::eStatus::CONNECTING);

	return false;
}

void cLucidVisionLabsRgbModel_Triton::stopCommunications()
{
    setStatus(sensor::eStatus::STOPPED);
}

void cLucidVisionLabsRgbModel_Triton::requestMode(int mode)
{
    if ((mode < eMode::SINGLE) || (eMode::CONTINUOUS < mode))
        return;

    auto camera_mode = mCamera->acquisitionMode();

    if (mCamera->isStreaming())
    {
        mCamera->stopStream();
    }

    mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);

    if (!mCamera->startStream())
    {
        return;
    }

    if (!mCamera->isStreaming())
    {
        return;
    }
}

void cLucidVisionLabsRgbModel_Triton::requestFrameRate_Hz(double frame_rate_hz)
{

}

void cLucidVisionLabsRgbModel_Triton::requestLapseInterval_ms(uint32_t frame_interval_ms)
{

}

void cLucidVisionLabsRgbModel_Triton::requestImage()
{

}

void cLucidVisionLabsRgbModel_Triton::requestImages(bool update_view)
{

}

void cLucidVisionLabsRgbModel_Triton::takePhoto(bool update_view)
{

}

void cLucidVisionLabsRgbModel_Triton::update()
{
//    if (!mIsRunning) return;

    if (!mCamera->isConnected())
    {
        setStatus(sensor::eStatus::FAILED);

//        mIsRunning = false;
        return;
    }

    if (!mCamera->isStreaming())
    {
        return;
    }

    bool newData = false;

    switch (mMode)
    {
    case eMode::SINGLE:
        if (mPhotoRequested)
        {
            auto* pImage = mCamera->getImage(0);

            if (pImage)
            {
                updateCurrentImage(pImage);

                newData = true;
                mPhotoRequested = false;
                emit photoTaken();
            }
        }
        break;
    case eMode::TIME_LAPSE:
        if (mTimeLapseTimer.elapsed())
        {
            auto* pImage = mCamera->getImage(0);

            if (pImage)
            {
                updateCurrentImage(pImage);
                newData = true;
                mTimeLapseTimer.start();
            }
        }
        break;
    case eMode::CONTINUOUS:
        auto* pImage = mCamera->getImage(0);

        if (pImage)
        {
            updateCurrentImage(pImage);
            newData = true;
        }
        break;
    }

    if (newData)
    {
        if (mIsRecording && static_cast<bool>(mSerializer))
        {
            mSerializer.writeImage(mInstanceID, mCurrentImage);
        }

        if (mImageRequested || mAutoEmitImages)
        {
            constexpr double scale = 255.0 / 65535.0;

            mImageBuffer.resize(mCurrentImage.size());

            for (std::size_t i = 0; i < mCurrentImage.size(); ++i)
            {
                mImageBuffer[i] = static_cast<uint8_t>(mCurrentImage[i] * scale);
            }

            mImage = QImage(mImageBuffer.data(), mCurrentImage.width(), mCurrentImage.height(), QImage::Format_RGB888);

            emit onNewImage(mImage);
            mImageRequested = false;
        }
    }
}

void cLucidVisionLabsRgbModel_Triton::errorHappend(int id, QString msg)
{
    QString full_msg = "Camera ";
    full_msg += QString::number(id);
    full_msg += ": " + msg;
    logMessage(logERROR, full_msg);

    setStatus(sensor::eStatus::FAILED);
}

void cLucidVisionLabsRgbModel_Triton::updateCurrentImage(Arena::IImage* pImage)
{
    auto pConverted = Arena::ImageFactory::Convert(pImage, RGB16);

    auto height = pConverted->GetHeight();
    auto width = pConverted->GetWidth();
    auto bits = pConverted->GetBitsPerPixel();
    auto id = pConverted->GetFrameId();

    mCurrentImage.setData(pConverted->GetData(), width, height, bits);

    Arena::ImageFactory::Destroy(pConverted);
    mCamera->requeueBuffer(pImage);

    mCurrentImage.setFrameID(id);
    mCurrentImage.setTimestamp_ns(cTimestampProvider::timestamp_ns());
}
