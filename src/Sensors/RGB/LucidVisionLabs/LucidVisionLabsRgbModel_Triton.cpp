
#include "LucidVisionLabsRgbModel_Triton.hpp"
#include "LucidVisionLabsRgbUtils.hpp"

#include <LucidVisionLabsConnect/LucidTritonCamera.hpp>
#include <LucidVisionLabsConnect/LucidVisionLabsData.hpp>

#include "TimestampProvider.hpp"
#include "StringUtils.hpp"

#include <Arena/ArenaAPI.h>

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

nLucidVisionLabsConnect::nTriton::eExposureAuto cLucidVisionLabsRgbModel_Triton::exposureAuto() const
{
    return mCamera->exposureAuto();
}

bool cLucidVisionLabsRgbModel_Triton::exposureAuto(nLucidVisionLabsConnect::nTriton::eExposureAuto mode)
{
    return mCamera->exposureAuto(mode);
}

double cLucidVisionLabsRgbModel_Triton::exposureTime_us() const
{
    return mCamera->exposureTime_us();
}

bool cLucidVisionLabsRgbModel_Triton::exposureTime_us(double time)
{
    return mCamera->exposureTime_us(time);
}

nLucidVisionLabsConnect::nTriton::eExposureTimeSelector cLucidVisionLabsRgbModel_Triton::exposureTimeSelector() const
{
    return mCamera->exposureTimeSelector();
}

bool cLucidVisionLabsRgbModel_Triton::exposureTimeSelector(nLucidVisionLabsConnect::nTriton::eExposureTimeSelector mode)
{
    return mCamera->exposureTimeSelector(mode);
}

nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto cLucidVisionLabsRgbModel_Triton::balanceWhiteAuto() const
{
    return mCamera->balanceWhiteAuto();
}

bool cLucidVisionLabsRgbModel_Triton::balanceWhiteAuto(nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto mode)
{
    return mCamera->balanceWhiteAuto(mode);
}

float cLucidVisionLabsRgbModel_Triton::gain_dB() const
{
    return mCamera->gain_dB();
}

bool cLucidVisionLabsRgbModel_Triton::gain_dB(float level_dB)
{
    return mCamera->gain_dB(level_dB);
}

nLucidVisionLabsConnect::nTriton::eGainAuto cLucidVisionLabsRgbModel_Triton::gainAuto() const
{
    return mCamera->gainAuto();
}

bool cLucidVisionLabsRgbModel_Triton::gainAuto(nLucidVisionLabsConnect::nTriton::eGainAuto mode)
{
    return mCamera->gainAuto(mode);
}

float cLucidVisionLabsRgbModel_Triton::gamma() const
{
    return mCamera->gamma();
}

bool cLucidVisionLabsRgbModel_Triton::gamma(float level)
{
    return mCamera->gamma(level);
}

bool cLucidVisionLabsRgbModel_Triton::gammaEnable() const
{
    return mCamera->gammaEnable();
}

bool cLucidVisionLabsRgbModel_Triton::gammaEnable(bool enable)
{
    return mCamera->gammaEnable(enable);
}

nLucidVisionLabsConnect::nTriton::ePixelFormat cLucidVisionLabsRgbModel_Triton::pixelFormat() const
{
    return mCamera->pixelFormat();
}

bool cLucidVisionLabsRgbModel_Triton::pixelFormat(nLucidVisionLabsConnect::nTriton::ePixelFormat mode)
{
    return mCamera->pixelFormat(mode);
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

    if (jsonCfg.contains("pixel format"))
    {
        std::string pixel_format = jsonCfg["pixel format"];
        mPixelFormat = lucid::to_pixel_format(pixel_format);
        mCamera->pixelFormat(mPixelFormat);
    }

    if (jsonCfg.contains("exposure time (us)"))
    {
        mExposureTime_us = jsonCfg["exposure time (us)"].get<double>();
        mCamera->exposureTime_us(mExposureTime_us);
    }

    if (jsonCfg.contains("exposure auto mode"))
    {
        std::string str = jsonCfg["exposure auto mode"];
        mExposureAuto = lucid::to_exposure_auto(str);
        mCamera->exposureAuto(mExposureAuto);
    }

    if (jsonCfg.contains("gain (dB)"))
    {
        mGain_dB = jsonCfg["gain (dB)"].get<double>();
        mCamera->gain_dB(mGain_dB);
    }

    if (jsonCfg.contains("gain auto mode"))
    {
        std::string str = jsonCfg["gain auto mode"];
        mGainAuto = lucid::to_gain_auto(str);
        mCamera->gainAuto(mGainAuto);
    }

    if (jsonCfg.contains("balance white auto mode"))
    {
        std::string str = jsonCfg["balance white auto mode"];
        mBalanceWhiteAuto = lucid::to_balance_white_auto(str);
        mCamera->balanceWhiteAuto(mBalanceWhiteAuto);
    }

    if (jsonCfg.contains("gamma enable"))
    {
        mGammaEnable = jsonCfg["gamma enable"].get<bool>();
        mCamera->gammaEnable(mGammaEnable);
    }

    if (jsonCfg.contains("gamma"))
    {
        mGamma = jsonCfg["gamma"].get<double>();
        mCamera->gamma(mGamma);
    }

    mPixelFormat = mCamera->pixelFormat();
    mExposureTime_us = mCamera->exposureTime_us();
    mExposureAuto = mCamera->exposureAuto();
    mGain_dB = mCamera->gain_dB();
    mGainAuto = mCamera->gainAuto();
    mBalanceWhiteAuto = mCamera->balanceWhiteAuto();
    mGammaEnable = mCamera->gammaEnable();
    mGamma = mCamera->gamma();

    mImageWidth  = mCamera->width();
    mImageHeight = mCamera->height();

    size_t buffer_size = mImageHeight * mImageWidth * sizeof(nLucidVisionLabsConnect::cRgbImage::value_type);

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
