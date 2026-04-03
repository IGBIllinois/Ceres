
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

    if (mCamera->isStreaming())
    {
        mCamera->stopStream();
    }


    switch (mode)
    {
    case cRgbCameraModel::eMode::SINGLE:
        mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);
        mCamera->triggerMode(nLucidVisionLabsConnect::eTriggerMode::ON);
        mCamera->triggerArmed(true);
        mCamera->acquisitionFrameRateEnable(true);
        mCamera->acquisitionFrameRate_Hz(10.0);
        mCamera->triggerSource(nLucidVisionLabsConnect::eTriggerSource::SOFTWARE);
        break;
    case cRgbCameraModel::eMode::TIME_LAPSE:
        mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::SINGLE_FRAME);
//        mCamera->acquisitionFrameRateEnable(true);
//        mCamera->acquisitionFrameRate_Hz(10.0);
        mCamera->triggerSource(nLucidVisionLabsConnect::eTriggerSource::SOFTWARE);
        mCamera->triggerMode(nLucidVisionLabsConnect::eTriggerMode::ON);
        mCamera->triggerArmed(true);

        mTimeLapseTimer.time_ms(mLapseInterval_ms);
        mTimeLapseTimer.start();

        mCamera->triggerSoftware();
        break;
    case cRgbCameraModel::eMode::CONTINUOUS:
        mCamera->acquisitionMode(nLucidVisionLabsConnect::eAcquisitionMode::CONTINUOUS);
        mCamera->triggerMode(nLucidVisionLabsConnect::eTriggerMode::OFF);
        mCamera->acquisitionFrameRate_Hz(mFrameRate_fps);
        mCamera->triggerSource(nLucidVisionLabsConnect::eTriggerSource::ENCODER_0);

        break;
    }

    mMode = mode;

    mCamera->startStream();

    if (changing)
        emit modeChanged(static_cast<int>(mMode));
}

void cLucidVisionLabsRgbModel_Triton::setFrameRate_Hz(double frame_rate_hz)
{
    if (frame_rate_hz == mFrameRate_fps)
        return;

//        enum eMode { SINGLE = 0, TIME_LAPSE = 1, CONTINUOUS = 2 }
    if ((mMode == cRgbCameraModel::eMode::CONTINUOUS) && mCamera->isStreaming())
    {
        if (mCamera->isStreaming())
        {
            mCamera->stopStream();
        }

        mCamera->acquisitionFrameRate_Hz(frame_rate_hz);
        mFrameRate_fps = mCamera->acquisitionFrameRate_Hz();

        mCamera->startStream();
    }
    else
    {
        mFrameRate_fps = frame_rate_hz;
    }

    emit frameRateChanged(mFrameRate_fps);
}

void cLucidVisionLabsRgbModel_Triton::setLapseInterval_ms(uint32_t interval_ms)
{
    if (interval_ms == mLapseInterval_ms)
        return;

    if ((mMode == cRgbCameraModel::eMode::TIME_LAPSE) && mCamera->isStreaming())
    {
        mTimeLapseTimer.time_ms(interval_ms);
        mTimeLapseTimer.start();
    }
    else
    {
        mLapseInterval_ms = interval_ms;
    }

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
    updateName(mModel);

    // We should not really hit this case unless the program crashed
    if (mCamera->isStreaming())
    {
        mCamera->stopStream();
    }

    mPixelFormat = mCamera->pixelFormat();
    mExposureTime_us = mCamera->exposureTime_us();
    mExposureAuto = mCamera->exposureAuto();
    mGain_dB = mCamera->gain_dB();
    mGainAuto = mCamera->gainAuto();
    mBalanceWhiteAuto = mCamera->balanceWhiteAuto();
    mGammaEnable = mCamera->gammaEnable();
    mGamma = mCamera->gamma();


    auto result = cLucidVisionLabsRgbModel::configure(jsonCfg);

    if (jsonCfg.contains("pixel format"))
    {
        std::string str = jsonCfg["pixel format"];
        auto pixel_format = lucid::to_pixel_format(str);

        if (pixel_format != mPixelFormat)
        {
            result &= mCamera->pixelFormat(pixel_format);
            mPixelFormat = mCamera->pixelFormat();
        }
    }

    if (jsonCfg.contains("exposure auto mode"))
    {
        std::string str = jsonCfg["exposure auto mode"];
        auto exposure_auto = lucid::to_exposure_auto(str);

        if (exposure_auto != mExposureAuto)
        {
            result &= mCamera->exposureAuto(exposure_auto);
            mExposureAuto = mCamera->exposureAuto();
        }
    }

    if (jsonCfg.contains("exposure time (us)"))
    {
        mExposureTime_us = jsonCfg["exposure time (us)"].get<double>();

        if (mExposureAuto == nLucidVisionLabsConnect::nTriton::eExposureAuto::OFF)
            result &= mCamera->exposureTime_us(mExposureTime_us);
    }

    if (jsonCfg.contains("gain auto mode"))
    {
        std::string str = jsonCfg["gain auto mode"];
        auto gain_auto = lucid::to_gain_auto(str);
        if (gain_auto != mGainAuto)
        {
            result &= mCamera->gainAuto(gain_auto);
            mGainAuto = mCamera->gainAuto();
        }
    }

    if (jsonCfg.contains("gain (dB)"))
    {
        mGain_dB = jsonCfg["gain (dB)"].get<double>();
        if (mGainAuto == nLucidVisionLabsConnect::nTriton::eGainAuto::OFF)
            result &= mCamera->gain_dB(mGain_dB);
    }

    if (jsonCfg.contains("balance white auto mode"))
    {
        std::string str = jsonCfg["balance white auto mode"];
        auto balance_white_auto = lucid::to_balance_white_auto(str);

        if (balance_white_auto != mBalanceWhiteAuto)
        {
            result &= mCamera->balanceWhiteAuto(mBalanceWhiteAuto);
            mBalanceWhiteAuto = mCamera->balanceWhiteAuto();
        }
    }

    if (jsonCfg.contains("gamma enable"))
    {
        bool gamma_enable = jsonCfg["gamma enable"].get<bool>();
        if (gamma_enable != mGammaEnable)
        {
            result &= mCamera->gammaEnable(mGammaEnable);
            mGammaEnable = mCamera->gammaEnable();
        }
    }

    if (jsonCfg.contains("gamma"))
    {
        mGamma = jsonCfg["gamma"].get<double>();
        if (mGammaEnable)
            result &= mCamera->gamma(mGamma);
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
        mCamera->streamAutoNegotiatePacketSize(true);
        mCamera->enableStreamPacketResend(true);

        setMode(mMode);

        if (!mCamera->isStreaming())
        {
            if (!mCamera->startStream())
            {
                setStatus(sensor::eStatus::FAILED);
                return false;
            }
        }

        setStatus(sensor::eStatus::CONNECTED);

        return true;
    }

    setStatus(sensor::eStatus::CONNECTING);

	return false;
}

void cLucidVisionLabsRgbModel_Triton::stopCommunications()
{
    if (mCamera->isStreaming())
    {
        mCamera->stopStream();
    }

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

        changed = (exposure_auto == mExposureAuto);
    }

    if (exposureTime_us != mExposureTime_us)
    {
        if (mExposureAuto == nLucidVisionLabsConnect::nTriton::eExposureAuto::OFF)
        {
            mCamera->exposureTime_us(exposureTime_us);
            mExposureTime_us = mCamera->exposureTime_us();
            changed = (exposureTime_us == mExposureTime_us);
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

        changed = (gain_auto == mGainAuto);
    }

    if (gain_dB != mGain_dB)
    {
        if (mGainAuto == nLucidVisionLabsConnect::nTriton::eGainAuto::OFF)
        {
            mCamera->gain_dB(gain_dB);
            mGain_dB = mCamera->gain_dB();
            changed = (gain_dB == mGain_dB);
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
        mCamera->balanceWhiteAuto(mBalanceWhiteAuto);
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

        changed = (enable == mGammaEnable);
    }

    if (gamma != mGamma)
    {
        if (mGammaEnable)
        {
            mCamera->gamma(gamma);
            mGamma = mCamera->gamma();
            changed = (gamma == mGamma);
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

    if (!mCamera->isStreaming())
    {
        mMode = static_cast<cRgbCameraModel::eMode>(mode);

        if (changing)
            emit modeChanged(static_cast<int>(mMode));

        return;
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
    mImageRequested = true;
    mAutoEmitImages = false;
}

void cLucidVisionLabsRgbModel_Triton::requestImages(bool update_view)
{

}

void cLucidVisionLabsRgbModel_Triton::takePhoto(bool update_view)
{
    if (mMode == eMode::SINGLE)
    {
        mPhotoRequested = true;
        mCamera->triggerSoftware();
    }
    else
    {
        mImageRequested = true;
    }
}

void cLucidVisionLabsRgbModel_Triton::update()
{
//    if (!mIsRunning) return;

    if (!mCamera->isConnected())
    {
        setStatus(sensor::eStatus::FAILED);
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
            auto* pImage = mCamera->getImage(1000);

            if (pImage)
            {
                updateCurrentImage(pImage);

                mCamera->triggerSoftware();

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

//-----------------------------------------------------------------------------
void cLucidVisionLabsRgbModel_Triton::OnImage(Arena::IImage* pImage)
{
    if (pImage->IsIncomplete())
    {

    }
    else if (pImage)
    {
        auto pConverted = Arena::ImageFactory::Convert(pImage, RGB16);

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

        constexpr double scale = 255.0 / 65535.0;

/*
        mImageBuffer.resize(rgb_image.size());

        for (std::size_t i = 0; i < rgb_image.size(); ++i)
        {
            mImageBuffer[i] = static_cast<uint8_t>(rgb_image[i] * scale);
        }

        QImage* image = new QImage(mImageBuffer.data(), width, height, QImage::Format_RGB888);

        if (image)
            mpImage->setImage(*image);
*/

//        delete image;
    }

    mCamera->requeueBuffer(pImage);

    mCamera->stopStream();

    update();
}

