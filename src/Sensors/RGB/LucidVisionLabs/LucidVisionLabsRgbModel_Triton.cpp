
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
    if (interval_ms < 500) interval_ms = 500;

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
    if (mCamera->isConnected())
        return mCamera->exposureAuto(mode);

    return false;
}

double cLucidVisionLabsRgbModel_Triton::exposureTime_us() const
{
    return mCamera->exposureTime_us();
}

bool cLucidVisionLabsRgbModel_Triton::exposureTime_us(double time)
{
    if (mCamera->isConnected())
        return mCamera->exposureTime_us(time);

    return false;
}

nLucidVisionLabsConnect::nTriton::eExposureTimeSelector cLucidVisionLabsRgbModel_Triton::exposureTimeSelector() const
{
    return mCamera->exposureTimeSelector();
}

bool cLucidVisionLabsRgbModel_Triton::exposureTimeSelector(nLucidVisionLabsConnect::nTriton::eExposureTimeSelector mode)
{
    if (mCamera->isConnected())
        return mCamera->exposureTimeSelector(mode);

    return false;
}

nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto cLucidVisionLabsRgbModel_Triton::balanceWhiteAuto() const
{
    return mCamera->balanceWhiteAuto();
}

bool cLucidVisionLabsRgbModel_Triton::balanceWhiteAuto(nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto mode)
{
    if (mCamera->isConnected())
        return mCamera->balanceWhiteAuto(mode);

    return false;
}

float cLucidVisionLabsRgbModel_Triton::gain_dB() const
{
    return mCamera->gain_dB();
}

bool cLucidVisionLabsRgbModel_Triton::gain_dB(float level_dB)
{
    if (mCamera->isConnected())
        return mCamera->gain_dB(level_dB);

    return false;
}

nLucidVisionLabsConnect::nTriton::eGainAuto cLucidVisionLabsRgbModel_Triton::gainAuto() const
{
    return mCamera->gainAuto();
}

bool cLucidVisionLabsRgbModel_Triton::gainAuto(nLucidVisionLabsConnect::nTriton::eGainAuto mode)
{
    if (mCamera->isConnected())
        return mCamera->gainAuto(mode);

    return false;
}

float cLucidVisionLabsRgbModel_Triton::gamma() const
{
    return mCamera->gamma();
}

bool cLucidVisionLabsRgbModel_Triton::gamma(float level)
{
    if (mCamera->isConnected())
        return mCamera->gamma(level);

    return false;
}

bool cLucidVisionLabsRgbModel_Triton::gammaEnable() const
{
    return mCamera->gammaEnable();
}

bool cLucidVisionLabsRgbModel_Triton::gammaEnable(bool enable)
{
    if (mCamera->isConnected())
        return mCamera->gammaEnable(enable);

    return false;
}

nLucidVisionLabsConnect::nTriton::ePixelFormat cLucidVisionLabsRgbModel_Triton::pixelFormat() const
{
    return mCamera->pixelFormat();
}

bool cLucidVisionLabsRgbModel_Triton::pixelFormat(nLucidVisionLabsConnect::nTriton::ePixelFormat mode)
{
    if (mCamera->isConnected())
        return mCamera->pixelFormat(mode);

    return false;
}

void cLucidVisionLabsRgbModel_Triton::updateViews()
{
    cLucidVisionLabsRgbModel::updateViews();

    if (mPixelFormat.has_value())
        emit pixelFormatChanged(static_cast<int>(mPixelFormat.value()));

    if (mExposureAuto.has_value() && mExposureTime_us.has_value())
        emit exposureChanged(static_cast<int>(mExposureAuto.value()), mExposureTime_us.value());

    if (mGainAuto.has_value() && mGain_dB.has_value())
        emit gainChanged(static_cast<int>(mGainAuto.value()), mGain_dB.value());

    if (mBalanceWhiteAuto.has_value())
        emit balanceWhiteAutoChanged(static_cast<int>(mBalanceWhiteAuto.value()));

    if (mGammaEnable.has_value() && mGamma.has_value())
        emit gammaChanged(mGammaEnable.value(), mGamma.value());
}

bool cLucidVisionLabsRgbModel_Triton::configure(const nlohmann::json& jsonCfg)
{
    auto result = cLucidVisionLabsRgbModel::configure(jsonCfg);

    if (jsonCfg.contains("pixel format"))
    {
        std::string str = jsonCfg["pixel format"];
        mPixelFormat = lucid::to_pixel_format(str);
    }

    if (jsonCfg.contains("exposure auto mode"))
    {
        std::string str = jsonCfg["exposure auto mode"];
        mExposureAuto = lucid::to_exposure_auto(str);
    }

    if (jsonCfg.contains("exposure time (us)"))
    {
        mExposureTime_us = jsonCfg["exposure time (us)"].get<double>();
    }

    if (jsonCfg.contains("gain auto mode"))
    {
        std::string str = jsonCfg["gain auto mode"];
        mGainAuto = lucid::to_gain_auto(str);
    }

    if (jsonCfg.contains("gain (dB)"))
    {
        mGain_dB = jsonCfg["gain (dB)"].get<double>();
    }

    if (jsonCfg.contains("balance white auto mode"))
    {
        std::string str = jsonCfg["balance white auto mode"];
        mBalanceWhiteAuto = lucid::to_balance_white_auto(str);
    }

    if (jsonCfg.contains("gamma enable"))
    {
        mGammaEnable = jsonCfg["gamma enable"].get<bool>();
    }

    if (jsonCfg.contains("gamma"))
    {
        mGamma = jsonCfg["gamma"].get<double>();
    }

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

    if (mFrameRate_fps <= 0)
        mFrameRate_fps = mCamera->acquisitionFrameRate_Hz();
        
    if (mPixelFormat.has_value())
    {
        auto pixel_format = mCamera->pixelFormat();
        if (mPixelFormat.value() != pixel_format)
        {
            if (!mCamera->pixelFormat(mPixelFormat.value()))
                mPixelFormat = mCamera->pixelFormat();
        }
    }
    else
        mPixelFormat = mCamera->pixelFormat();

    if (mExposureTime_us.has_value())
    {
        auto exposureTime_us = mCamera->exposureTime_us();
        if (mExposureTime_us.value() != exposureTime_us)
        {
            if (!mCamera->exposureTime_us(mExposureTime_us.value()))
                mExposureTime_us = mCamera->exposureTime_us();
        }
    }
    else
        mExposureTime_us = mCamera->exposureTime_us();

    if (mExposureAuto.has_value())
    {
        auto exposureAuto = mCamera->exposureAuto();
        if (mExposureAuto.value() != exposureAuto)
        {
            if (!mCamera->exposureAuto(mExposureAuto.value()))
                mExposureAuto = mCamera->exposureAuto();
        }
    }
    else
        mExposureAuto = mCamera->exposureAuto();

    if (mGainAuto.has_value())
    {
        auto gainAuto = mCamera->gainAuto();
        if (mGainAuto.value() != gainAuto)
        {
            if (!mCamera->gainAuto(mGainAuto.value()))
                mGainAuto = mCamera->gainAuto();
        }
    }
    else
        mGainAuto = mCamera->gainAuto();

    if (mGain_dB.has_value())
    {
        auto gain_dB = mCamera->gain_dB();
        if (mGain_dB.value() != gain_dB)
        {
            if (!mCamera->gain_dB(mGain_dB.value()))
                mGain_dB = mCamera->gain_dB();
        }
    }
    else
        mGain_dB = mCamera->gain_dB();

    if (mBalanceWhiteAuto.has_value())
    {
        auto balanceWhiteAuto = mCamera->balanceWhiteAuto();
        if (mBalanceWhiteAuto.value() != balanceWhiteAuto)
        {
            if (!mCamera->balanceWhiteAuto(mBalanceWhiteAuto.value()))
                mBalanceWhiteAuto = mCamera->balanceWhiteAuto();
        }
    }
    else
        mBalanceWhiteAuto = mCamera->balanceWhiteAuto();

    if (mGammaEnable.has_value())
    {
        auto gammaEnable = mCamera->gammaEnable();
        if (mGammaEnable.value() != gammaEnable)
        {
            if (!mCamera->gammaEnable(mGammaEnable.value()))
                mGammaEnable = mCamera->gammaEnable();
        }
    }
    else
        mGammaEnable = mCamera->gammaEnable();

    if (mGamma.has_value())
    {
        auto gamma = mCamera->gamma();
        if (mGamma.value() != gamma)
        {
            if (!mCamera->gamma(mGamma.value()))
                mGamma = mCamera->gamma();
        }
    }
    else
       mGamma = mCamera->gamma();

    mCamera->acquisitionStartMode(nLucidVisionLabsConnect::eAcquisitionStartMode::NORMAL);
    mCamera->triggerMode(nLucidVisionLabsConnect::eTriggerMode::OFF);
    mCamera->triggerSource(nLucidVisionLabsConnect::eTriggerSource::LINE_0);
    mCamera->triggerArmed(false);

    mImageWidth = mCamera->width();
    mImageHeight = mCamera->height();

    updateViews();

    size_t buffer_size = mImageHeight * mImageWidth * sizeof(nLucidVisionLabsConnect::cRgbImage::value_type);

    mSerializer.setBufferCapacity(buffer_size + 1024);

    mCamera->registerImageCallback(this);
    mCamera->streamAutoNegotiatePacketSize(true);
    mCamera->enableStreamPacketResend(true);

    setMode(mMode);

    if (!mCamera->isStreaming())
    {
        switch (mMode)
        {
        case cRgbCameraModel::eMode::SINGLE:
            break;
        case cRgbCameraModel::eMode::TIME_LAPSE:
            if (!mCamera->startStream())
            {
                setStatus(sensor::eStatus::FAILED);
                return false;
            }
            break;
        case cRgbCameraModel::eMode::CONTINUOUS:
            if (!mCamera->startStream())
            {
                setStatus(sensor::eStatus::FAILED);
                return false;
            }
            break;
        }
    }

    setStatus(sensor::eStatus::CONNECTED);

    mIsRunning = true;

    return true;
}

void cLucidVisionLabsRgbModel_Triton::stopCommunications()
{
    if (mCamera->isStreaming())
    {
        mCamera->stopStream();
    }

    if (mCamera)
        mCamera->deregisterImageCallback(this);

    mCamera->disconnect();

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

    if (!mCamera->isConnected())
    {
        mPixelFormat = pixel_format;
        emit pixelFormatChanged(static_cast<int>(pixel_format));
        return;
    }

    if (!mPixelFormat.has_value())
        mPixelFormat = mCamera->pixelFormat();

    if (pixel_format != mPixelFormat)
    {
        mCamera->pixelFormat(pixel_format);
        mPixelFormat = mCamera->pixelFormat();
        if (pixel_format == mPixelFormat)
            emit pixelFormatChanged(static_cast<int>(mPixelFormat.value()));
    }
}

void cLucidVisionLabsRgbModel_Triton::requestExposure(int mode, double exposureTime_us)
{
    bool changed = false;
    auto exposure_auto = static_cast<nLucidVisionLabsConnect::nTriton::eExposureAuto>(mode);

    if (!mCamera->isConnected())
    {
        mExposureAuto = exposure_auto;
        mExposureTime_us = exposureTime_us;
        emit exposureChanged(static_cast<int>(exposure_auto), exposureTime_us);
        return;
    }

    if (!mExposureAuto.has_value())
        mExposureAuto = mCamera->exposureAuto();

    if (!mExposureTime_us.has_value())
        mExposureTime_us = mCamera->exposureTime_us();

    if (exposure_auto != mExposureAuto.value())
    {
        mCamera->exposureAuto(exposure_auto);
        mExposureAuto = mCamera->exposureAuto();

        changed |= (exposure_auto == mExposureAuto.value());
    }

    if (exposureTime_us != mExposureTime_us.value())
    {
        if (mExposureAuto == nLucidVisionLabsConnect::nTriton::eExposureAuto::OFF)
        {
            mCamera->exposureTime_us(exposureTime_us);
            mExposureTime_us = mCamera->exposureTime_us();
            changed |= (exposureTime_us == mExposureTime_us.value());
        }
        else
        {
            mExposureTime_us = exposureTime_us;
            changed = true;
        }
    }

    if (changed && mExposureAuto.has_value() && mExposureTime_us.has_value())
        emit exposureChanged(static_cast<int>(mExposureAuto.value()), mExposureTime_us.value());
}

void cLucidVisionLabsRgbModel_Triton::requestGain(int mode, double gain_dB)
{
    bool changed = false;
    auto gain_auto = static_cast<nLucidVisionLabsConnect::nTriton::eGainAuto>(mode);

    if (!mCamera->isConnected())
    {
        mGainAuto = gain_auto;
        mGain_dB  = gain_dB;
        emit gainChanged(static_cast<int>(gain_auto), gain_dB);
        return;
    }

    if (!mGainAuto.has_value())
        mGainAuto = mCamera->gainAuto();

    if (!mGain_dB.has_value())
        mGain_dB = mCamera->gain_dB();

    if (gain_auto != mGainAuto.value())
    {
        mCamera->gainAuto(gain_auto);
        mGainAuto = mCamera->gainAuto();

        changed |= (gain_auto == mGainAuto);
    }

    if (gain_dB != mGain_dB.value())
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
        emit gainChanged(static_cast<int>(mGainAuto.value()), mGain_dB.value());
}

void cLucidVisionLabsRgbModel_Triton::requestBalanceWhiteAuto(int mode)
{
    auto balance_white_auto = static_cast<nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto>(mode);

    if (!mCamera->isConnected())
    {
        mBalanceWhiteAuto = balance_white_auto;
        emit balanceWhiteAutoChanged(static_cast<int>(balance_white_auto));
        return;
    }

    if (!mBalanceWhiteAuto.has_value())
        mBalanceWhiteAuto = mCamera->balanceWhiteAuto();

    if (balance_white_auto != mBalanceWhiteAuto.value())
    {
        mCamera->balanceWhiteAuto(balance_white_auto);
        mBalanceWhiteAuto = mCamera->balanceWhiteAuto();

        if (balance_white_auto == mBalanceWhiteAuto.value())
            emit balanceWhiteAutoChanged(static_cast<int>(mBalanceWhiteAuto.value()));
    }
}

void cLucidVisionLabsRgbModel_Triton::requestGamma(bool enable, double gamma)
{
    bool changed = false;

    if (!mCamera->isConnected())
    {
        mGammaEnable = enable;
        mGamma = gamma;
        emit gammaChanged(enable, gamma);
        return;
    }

    if (!mGammaEnable.has_value())
        mGammaEnable = mCamera->gammaEnable();

    if (!mGamma.has_value())
        mGamma = mCamera->gamma();

    if (enable != mGammaEnable.value())
    {
        mCamera->gammaEnable(enable);
        mGammaEnable = mCamera->gammaEnable();

        changed |= (enable == mGammaEnable);
    }

    if (gamma != mGamma.value())
    {
        if (mGammaEnable.value())
        {
            mCamera->gamma(gamma);
            mGamma = mCamera->gamma();
            changed |= (gamma == mGamma.value());
        }
        else
        {
            mGamma = gamma;
            changed = true;
        }
    }

    if (changed)
        emit gammaChanged(mGammaEnable.value(), mGamma.value());
}


void cLucidVisionLabsRgbModel_Triton::requestMode(int mode)
{
    if ((mode < eMode::SINGLE) || (eMode::CONTINUOUS < mode))
        return;

    bool changing = mode != mMode;

    if (mCamera->isConnected())
    {
        if (!mCamera->isStreaming())
        {
            mMode = static_cast<cRgbCameraModel::eMode>(mode);

            if (changing)
                emit modeChanged(static_cast<int>(mMode));

            return;
        }
    }

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
            emit photoTaken();
            break;
        case eMode::TIME_LAPSE:
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
            std::lock_guard<std::mutex> camera_guard(mCameraMutex);

            if (mCamera->isStreaming())
            {
                mCamera->stopStream();
            }

            mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);

            mCamera->startStream();

            mPhotoRequested = false;
        }
        break;
    case eMode::TIME_LAPSE:
        if (mTimeLapseTimer.elapsed())
        {
            std::lock_guard<std::mutex> camera_guard(mCameraMutex);

            if (mCamera->isStreaming())
            {
                mCamera->stopStream();
            }

            mTimeLapseTimer.start();

            mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);
            mCamera->startStream();
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
    auto image_width  = mImageWidth;
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

    if ((mImageWidth != image_width) || (mImageHeight != image_height))
    {
        mImageWidth = image_width;
        mImageHeight = image_height;

        emit imageSizeChanged(mImageWidth, mImageHeight);
    }

    auto height = pConverted->GetHeight();
    auto width = pConverted->GetWidth();
    auto bits = pConverted->GetBitsPerPixel();
    auto n = pConverted->GetPayloadSize();
    auto id = pConverted->GetFrameId();
    auto timestamp_ns = pConverted->GetTimestampNs();

    mCurrentImage.setData(pConverted->GetData(), width, height, bits);
    mCurrentImage.setFrameID(id);
    mCurrentImage.setTimestamp_ns(timestamp_ns);

    Arena::ImageFactory::Destroy(pConverted);

    if (mMode != eMode::CONTINUOUS)
    {
        std::lock_guard<std::mutex> camera_guard(mCameraMutex);
        mCamera->stopStream();
    }

    return true;
}

//-----------------------------------------------------------------------------
// The Arena callback methods
void cLucidVisionLabsRgbModel_Triton::OnImage(Arena::IImage* pImage)
{
    if (pImage->IsIncomplete())
    {
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

