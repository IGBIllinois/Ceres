
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
{}

cLucidVisionLabsRgbModel_Triton::~cLucidVisionLabsRgbModel_Triton()
{
	stopCommunications();

    if (mCamera->isConnected())
        mCamera->disconnect();
}

uint8_t cLucidVisionLabsRgbModel_Triton::device_id() const
{
    return mInstanceID;
}

void cLucidVisionLabsRgbModel_Triton::setMode(eMode mode)
{
    bool changing = mode != mMode;

    if (mCamera->isConnected())
    {
        bool is_streaming = mCamera->isStreaming();

        if (is_streaming)
        {
            mCamera->stopStream();
        }

        switch (mode)
        {
        case cRgbCameraModel::eMode::SINGLE:
            mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);
            break;
        case cRgbCameraModel::eMode::TIME_LAPSE:
            mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);

            mTimeLapseTimer.time_ms(mLapseInterval_ms);
            mTimeLapseTimer.start();

            break;
        case cRgbCameraModel::eMode::CONTINUOUS:
            mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::CONTINUOUS);

            if (is_streaming)
                mCamera->startStream();
            break;
        }

        if (is_streaming)
            mCamera->startStream();
    }

    mMode = mode;

    if (changing)
        emit modeChanged(static_cast<int>(mMode));
}

void cLucidVisionLabsRgbModel_Triton::setFrameRate_Hz(double frame_rate_hz)
{
    if (frame_rate_hz == mFrameRate_fps)
        return;

    if (mCamera->isConnected())
    {
        if ((mMode == cRgbCameraModel::eMode::CONTINUOUS) && mCamera->isStreaming())
        {
            if (mCamera->isStreaming())
            {
                mCamera->stopStream();
            }

            mCamera->acquisitionFrameRateEnable(true);
            mCamera->acquisitionFrameRate_Hz(frame_rate_hz);
            mFrameRate_fps = mCamera->acquisitionFrameRate_Hz();

            mCamera->startStream();
        }
        else
        {
            mCamera->acquisitionFrameRateEnable(false);
            mFrameRate_fps = frame_rate_hz;
        }
    }
    else
        mFrameRate_fps = frame_rate_hz;

    emit frameRateChanged(mFrameRate_fps);
}

void cLucidVisionLabsRgbModel_Triton::setLapseInterval_ms(uint32_t interval_ms)
{
    if (interval_ms == mLapseInterval_ms)
        return;

    if (mCamera->isConnected())
    {
        if ((mMode == cRgbCameraModel::eMode::TIME_LAPSE) && mCamera->isStreaming())
        {
            mTimeLapseTimer.time_ms(interval_ms);
            mTimeLapseTimer.start();
        }
    }

    mLapseInterval_ms = interval_ms;

    emit lapseIntervalChanged(mLapseInterval_ms);
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
    cLucidVisionLabsRgbModel::updateViews();

    emit pixelFormatChanged(static_cast<int>(mPixelFormat));
    emit exposureChanged(static_cast<int>(mExposureAuto), mExposureTime_us);
    emit gainChanged(static_cast<int>(mGainAuto), mGain_dB);
    emit balanceWhiteAutoChanged(static_cast<int>(mBalanceWhiteAuto));
    emit gammaChanged(mGammaEnable, mGamma);
}

bool cLucidVisionLabsRgbModel_Triton::configure(const nlohmann::json& jsonCfg)
{
    if (!mCamera->connect())
    {
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    mModel = mCamera->getModelName();
    mManufacturer = mCamera->getVendorName();
    mSerialNumber = mCamera->getSerialNumber();
    mFamilyName = mCamera->getFamilyName();
    mModelVersion = mCamera->getVersion();
    mFirmwareVersion = mCamera->getFirmwareVersion();
    mMacAddress = mCamera->getMacAddress();
    mIpAddress = mCamera->getIpAddress();

    updateName(mModel);

    // Load defaults from camera
    mPixelFormat = mCamera->pixelFormat();
    mExposureTime_us = mCamera->exposureTime_us();
    mExposureAuto = mCamera->exposureAuto();
    mGainAuto = mCamera->gainAuto();
    mGain_dB = mCamera->gain_dB();
    mBalanceWhiteAuto = mCamera->balanceWhiteAuto();
    mGammaEnable = mCamera->gammaEnable();
    mGamma = mCamera->gamma();
    mFrameRate_fps = mCamera->acquisitionFrameRate_Hz();

    auto result = cLucidVisionLabsRgbModel::configure(jsonCfg);

    if (jsonCfg.contains("pixel format"))
    {
        std::string str = jsonCfg["pixel format"];
        auto pixel_format = lucid::to_pixel_format(str);
        if (mPixelFormat != pixel_format)
        {
            mCamera->pixelFormat(pixel_format);
            mPixelFormat = mCamera->pixelFormat();
        }
    }

    if (jsonCfg.contains("exposure auto mode"))
    {
        std::string str = jsonCfg["exposure auto mode"];
        auto exposureAuto = lucid::to_exposure_auto(str);
        if (mExposureAuto != exposureAuto)
        {
            mCamera->exposureAuto(exposureAuto);
            mExposureAuto = mCamera->exposureAuto();
        }
    }

    if (jsonCfg.contains("exposure time (us)"))
    {
        auto exposureTime_us = jsonCfg["exposure time (us)"].get<double>();
        if (mExposureTime_us != exposureTime_us)
        {
            mCamera->exposureTime_us(exposureTime_us);
            mExposureTime_us = mCamera->exposureTime_us();
        }
    }

    if (jsonCfg.contains("gain auto mode"))
    {
        std::string str = jsonCfg["gain auto mode"];
        auto gainAuto = lucid::to_gain_auto(str);
        if (mGainAuto != gainAuto)
        {
            mCamera->gainAuto(gainAuto);
            mGainAuto = mCamera->gainAuto();
        }
    }

    if (jsonCfg.contains("gain (dB)"))
    {
        auto gain_dB = jsonCfg["gain (dB)"].get<double>();
        if (mGain_dB != gain_dB)
        {
            mCamera->gain_dB(gain_dB);
            mGain_dB = mCamera->gain_dB();
        }
    }

    if (jsonCfg.contains("balance white auto mode"))
    {
        std::string str = jsonCfg["balance white auto mode"];
        auto balanceWhiteAuto = lucid::to_balance_white_auto(str);
        if (mBalanceWhiteAuto != balanceWhiteAuto)
        {
            mCamera->balanceWhiteAuto(balanceWhiteAuto);
            mBalanceWhiteAuto = mCamera->balanceWhiteAuto();
        }
    }

    if (jsonCfg.contains("gamma enable"))
    {
        auto gammaEnable = jsonCfg["gamma enable"].get<bool>();
        if (mGammaEnable != gammaEnable)
        {
            mCamera->gammaEnable(gammaEnable);
            mGammaEnable = mCamera->gammaEnable();
        }
    }

    if (jsonCfg.contains("gamma"))
    {
        auto gamma = jsonCfg["gamma"].get<double>();
        if (mGamma != gamma)
        {
            mCamera->gamma(gamma);
            mGamma = mCamera->gamma();
        }
    }

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
    return cLucidVisionLabsRgbModel::initialize();
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
    if (!mCamera->isConnected())
    {
        setStatus(sensor::eStatus::FAILED);

        return false;
    }

    mCamera->streamBufferHandlingMode(nLucidVisionLabsConnect::eBufferHandlingMode::NEWEST_ONLY);
    mCamera->streamAutoNegotiatePacketSize(true);
    mCamera->enableStreamPacketResend(true);

    mCamera->acquisitionStartMode(nLucidVisionLabsConnect::eAcquisitionStartMode::NORMAL);
//    mCamera->triggerMode(nLucidVisionLabsConnect::eTriggerMode::OFF);
//    mCamera->triggerSource(nLucidVisionLabsConnect::eTriggerSource::LINE_0);
//    mCamera->triggerArmed(false);

    mCamera->registerImageCallback(this);

    switch (mMode)
    {
    case cRgbCameraModel::eMode::SINGLE:
        mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);
        mProcessImage = false;
        break;
    case cRgbCameraModel::eMode::TIME_LAPSE:
        mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);

        mTimeLapseTimer.time_ms(mLapseInterval_ms);
        mTimeLapseTimer.start();

        mProcessImage = false;
        break;
    case cRgbCameraModel::eMode::CONTINUOUS:
        mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::CONTINUOUS);

        mProcessImage = true;
        break;
    }

    if (mCamera->startStream())
    {
        setStatus(sensor::eStatus::RUNNING);
        mIsRunning = true;
    }
    else
    {
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    switch (mMode)
    {
    case cRgbCameraModel::eMode::SINGLE:
    case cRgbCameraModel::eMode::TIME_LAPSE:
//        mCamera->acquisitionStop();
        break;
    case cRgbCameraModel::eMode::CONTINUOUS:
        break;
    }

    return true;
}

void cLucidVisionLabsRgbModel_Triton::stopCommunications()
{
    int n = 0;
    while (mCamera->isStreaming() && (n<5))
    {
        mCamera->stopStream();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++n;
    }

    if (mCamera)
        mCamera->deregisterImageCallback(this);

    mIsRunning = false;

    setStatus(sensor::eStatus::STOPPED);
}

void cLucidVisionLabsRgbModel_Triton::pushStreamState()
{
    mStreamStateStack = mCamera->isStreaming();

    if (mStreamStateStack.value())
        mCamera->stopStream();
}

void cLucidVisionLabsRgbModel_Triton::popStreamState()
{
    if (mStreamStateStack.has_value())
    {
        if (mStreamStateStack.value())
            mCamera->startStream();
    }

    mStreamStateStack.reset();
}

void cLucidVisionLabsRgbModel_Triton::requestPixelFormat(int mode)
{
    auto pixel_format = static_cast<nLucidVisionLabsConnect::nTriton::ePixelFormat>(mode);

    if (pixel_format != mPixelFormat)
    {
        mCamera->pixelFormat(pixel_format);
        mPixelFormat = mCamera->pixelFormat();
        if (pixel_format == mPixelFormat)
            emit pixelFormatChanged(static_cast<int>(mPixelFormat));
    }
}

void cLucidVisionLabsRgbModel_Triton::requestExposure(int mode, double exposureTime_us)
{
    bool changed = false;
    auto exposure_auto = static_cast<nLucidVisionLabsConnect::nTriton::eExposureAuto>(mode);

    if (exposure_auto != mExposureAuto)
    {
        mCamera->exposureAuto(exposure_auto);
        mExposureAuto = mCamera->exposureAuto();

        changed |= (exposure_auto == mExposureAuto);
    }

    if (exposureTime_us != mExposureTime_us)
    {
        if (mExposureAuto == nLucidVisionLabsConnect::nTriton::eExposureAuto::OFF)
        {
            mCamera->exposureTime_us(exposureTime_us);
            mExposureTime_us = mCamera->exposureTime_us();
            changed |= (exposureTime_us == mExposureTime_us);
        }
        else
        {
            mExposureTime_us = exposureTime_us;
            changed = true;
        }
    }

    if (changed)
        emit exposureChanged(static_cast<int>(mExposureAuto), mExposureTime_us);
}

void cLucidVisionLabsRgbModel_Triton::requestGain(int mode, double gain_dB)
{
    bool changed = false;
    auto gain_auto = static_cast<nLucidVisionLabsConnect::nTriton::eGainAuto>(mode);

    if (gain_auto != mGainAuto)
    {
        mCamera->gainAuto(gain_auto);
        mGainAuto = mCamera->gainAuto();

        changed |= (gain_auto == mGainAuto);
    }

    if (gain_dB != mGain_dB)
    {
        if (mGainAuto == nLucidVisionLabsConnect::nTriton::eGainAuto::OFF)
        {
            mCamera->gain_dB(gain_dB);
            mGain_dB = mCamera->gain_dB();
            changed |= (gain_dB == mGain_dB);
        }
        else
        {
            mGain_dB = gain_dB;
            changed = true;
        }
    }

    if (changed)
        emit gainChanged(static_cast<int>(mGainAuto), mGain_dB);
}

void cLucidVisionLabsRgbModel_Triton::requestBalanceWhiteAuto(int mode)
{
    auto balance_white_auto = static_cast<nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto>(mode);

    if (balance_white_auto != mBalanceWhiteAuto)
    {
        mCamera->balanceWhiteAuto(balance_white_auto);
        mBalanceWhiteAuto = mCamera->balanceWhiteAuto();

        if (balance_white_auto == mBalanceWhiteAuto)
            emit balanceWhiteAutoChanged(static_cast<int>(mBalanceWhiteAuto));
    }
}

void cLucidVisionLabsRgbModel_Triton::requestGamma(bool enable, double gamma)
{
    bool changed = false;

    if (enable != mGammaEnable)
    {
        mCamera->gammaEnable(enable);
        mGammaEnable = mCamera->gammaEnable();

        changed |= (enable == mGammaEnable);
    }

    if (gamma != mGamma)
    {
        if (mGammaEnable)
        {
            mCamera->gamma(gamma);
            mGamma = mCamera->gamma();
            changed |= (gamma == mGamma);
        }
        else
        {
            mGamma = gamma;
            changed = true;
        }
    }

    if (changed)
        emit gammaChanged(mGammaEnable, mGamma);
}


void cLucidVisionLabsRgbModel_Triton::requestMode(int mode)
{
    if ((mode < eMode::SINGLE) || (eMode::CONTINUOUS < mode))
        return;

    bool changing = mode != mMode;

    if (changing)
        setMode(static_cast<eMode>(mode));
}

void cLucidVisionLabsRgbModel_Triton::requestFrameRate_Hz(double frame_rate_hz)
{
    if (frame_rate_hz == mFrameRate_fps)
        return;

    setFrameRate_Hz(frame_rate_hz);
}

void cLucidVisionLabsRgbModel_Triton::requestLapseInterval_ms(uint32_t interval_ms)
{
    if (interval_ms == mLapseInterval_ms)
        return;

    setLapseInterval_ms(interval_ms);
}

void cLucidVisionLabsRgbModel_Triton::requestImage()
{
    if (mIsRunning)
    {
        mImageRequested = true;
        mAutoEmitImages = false;
    }
}

void cLucidVisionLabsRgbModel_Triton::requestImages(bool update_view)
{
    if (mIsRunning)
        mAutoEmitImages = update_view;
}

void cLucidVisionLabsRgbModel_Triton::takePhoto(bool update_view)
{
    if (mIsRunning)
    {
        mPhotoRequested = true;
        mImageRequested = update_view;
    }
}

void cLucidVisionLabsRgbModel_Triton::update()
{
    if (!mIsRunning) return;

    if (updateImage())
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

        switch (mMode)
        {
        case eMode::SINGLE:
            mCamera->acquisitionStop();
            emit photoTaken();
            break;
        case eMode::TIME_LAPSE:
            mCamera->acquisitionStop();
            if (mPhotoRequested)
            {
                emit photoTaken();
                mPhotoRequested = false;
            }
            break;
        case eMode::CONTINUOUS:
            break;
        }
    }

    switch (mMode)
    {
    case eMode::SINGLE:
        if (mPhotoRequested)
        {
            mCamera->acquisitionStart();

            mProcessImage = true;
            mPhotoRequested = false;
        }
        break;
    case eMode::TIME_LAPSE:
        if (mTimeLapseTimer.elapsed())
        {
            mCamera->acquisitionStart();

            mProcessImage = true;
            mTimeLapseTimer.start();
        }
        break;
    case eMode::CONTINUOUS:
        break;
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

bool cLucidVisionLabsRgbModel_Triton::updateImage()
{
    Arena::IImage* pConverted = nullptr;
    auto image_width = mImageWidth;
    auto image_height = mImageHeight;

    {
        std::lock_guard<std::mutex> image_guard(mImageMutex);

        if (!static_cast<bool>(mpTemporyImage)) return false;

        //        image_width = mpTemporyImage->GetWidth();
        //        image_height = mpTemporyImage->GetHeight();

        auto pConverted = Arena::ImageFactory::Convert(mpTemporyImage, RGB16);

        Arena::ImageFactory::Destroy(mpTemporyImage);
        mpTemporyImage = nullptr;
    }

    return false;
}

//-----------------------------------------------------------------------------
// The Arena callback methods
void cLucidVisionLabsRgbModel_Triton::OnImage(Arena::IImage* pImage)
{
    if (pImage->IsIncomplete() || !mProcessImage)
    {
        std::lock_guard<std::mutex> camera_guard(mCameraMutex);
        mCamera->requeueBuffer(pImage);
        return;
    }

    std::lock_guard<std::mutex> image_guard(mImageMutex);

    if (mpTemporyImage)
    {
        Arena::ImageFactory::Destroy(mpTemporyImage);
        mpTemporyImage = nullptr;
    }

    mpTemporyImage = Arena::ImageFactory::Copy(pImage);

    {
        std::lock_guard<std::mutex> camera_guard(mCameraMutex);

        mCamera->requeueBuffer(pImage);
    }
}

