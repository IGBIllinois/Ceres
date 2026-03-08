
#include "TeledyneFlirCameraModel_T1K.hpp"

#include "TimestampProvider.hpp"

#include <TeledyneAtlasConnect/TeledyneFlirCamera.hpp>

#define USE_LOG_MESSAGE


namespace
{
    static uint8_t teledyne_flir_t1k_instance_id = 0;
}


cTeledyneFlirCameraModel_T1K::cTeledyneFlirCameraModel_T1K(std::unique_ptr<cTeledyneFlirCamera> camera, QObject* parent)
:
    cTeledyneFlirCameraModel("T1K FLIR Camera", parent),
    mInstanceID(++teledyne_flir_t1k_instance_id)
{
    mCamera = std::move(camera);

    mModel = mCamera->modelName();
    mSerialNumber = mCamera->serialNumber();

    mFrameRate_fps = mCamera->getFrameRate_Hz();
    mMinFrameRate_fps = mCamera->getMinFrameRate_Hz();
    mMaxFrameRate_fps = mCamera->getMaxFrameRate_Hz();

    mMinThermalRange_K = mCamera->getThermalRangeMin_K();
    mMaxThermalRange_K = mCamera->getThermalRangeMax_K();

    if (mMinThermalRange_K.has_value() && mMaxThermalRange_K.has_value())
        mColorTable.setRange(mMinThermalRange_K.value(), mMaxThermalRange_K.value());
    else
        mColorTable.setRange(250, 400);

    auto width = mCamera->width();
    if (width > 0)
        mImageWidth = static_cast<uint16_t>(width);

    auto height = mCamera->height();
    if (height > 0)
        mImageHeight = static_cast<uint16_t>(height);
}

cTeledyneFlirCameraModel_T1K::~cTeledyneFlirCameraModel_T1K()
{
	stopCommunications();
}

uint8_t cTeledyneFlirCameraModel_T1K::device_id() const
{
    return mInstanceID;
}

void cTeledyneFlirCameraModel_T1K::updateViews()
{
}

bool cTeledyneFlirCameraModel_T1K::configure(const nlohmann::json& jsonCfg)
{
    bool result = cTeledyneFlirCameraModel::configure(jsonCfg);

    size_t buffer_size = mImageHeight * mImageWidth * sizeof(double);

    mSerializer.setBufferCapacity(buffer_size + 1024);

    if (result)
        setStatus(sensor::eStatus::CONFIGURED);
    else
        setStatus(sensor::eStatus::FAILED);

    return result;
}

void cTeledyneFlirCameraModel_T1K::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cTeledyneFlirCameraModel_T1K::disableDataRecording()
{
    cTeledyneFlirCameraModel::disableDataRecording();
    mSerializer.detach();
}

void cTeledyneFlirCameraModel_T1K::writeDataHeader()
{
    auto dev_id = device_id();

    auto str = mCamera->modelName();
    if (!str.empty())
        mSerializer.writeModelName(dev_id, str);

    str = mCamera->filter();
    if (!str.empty())
        mSerializer.writeFilter(dev_id, str);

    str = mCamera->lens();
    if (!str.empty())
        mSerializer.writeLens(dev_id, str);

    str = mCamera->serialNumber();
    if (!str.empty())
        mSerializer.writeSerialNumber(dev_id, str);

    str = mCamera->programVersion();
    if (!str.empty())
        mSerializer.writeProgramVersion(dev_id, str);

    str = mCamera->articleNumber();
    if (!str.empty())
        mSerializer.writeArticleNumber(dev_id, str);

    str = mCamera->calibrationTitle();
    if (!str.empty())
        mSerializer.writeCalibrationTitle(dev_id, str);

    str = mCamera->lensSerialNumber();
    if (!str.empty())
        mSerializer.writeLensSerialNumber(dev_id, str);

    str = mCamera->arcFileVersion();
    if (!str.empty())
        mSerializer.writeArcFileVersion(dev_id, str);

    str = mCamera->arcDateTime();
    if (!str.empty())
        mSerializer.writeArcDateTime(dev_id, str);

    str = mCamera->arcSignature();
    if (!str.empty())
        mSerializer.writeArcSignature(dev_id, str);

    str = mCamera->countryCode();
    if (!str.empty())
        mSerializer.writeCountryCode(dev_id, str);

    mSerializer.writeFrameRate_Hz(dev_id, mFrameRate_fps);

    if (mMinFrameRate_fps.has_value() && mMaxFrameRate_fps.has_value())
        mSerializer.writeFrameRateRange_Hz(dev_id, mMinFrameRate_fps.value(), mMaxFrameRate_fps.value());

    if (mMinThermalRange_K.has_value() && mMaxThermalRange_K.has_value())
        mSerializer.writeThermalRange_K(dev_id, mMinThermalRange_K.value(), mMaxThermalRange_K.value());

    auto fov = mCamera->getHorizonalFoV_deg();
    if (fov.has_value())
        mSerializer.writeHorizonalFoV_deg(dev_id, fov.value());
    
    auto fl = mCamera->getFocalLength();
    if (fl.has_value())
        mSerializer.writeFocalLength(dev_id, fl.value());
}

bool cTeledyneFlirCameraModel_T1K::startCommunications()
{
    mCamera->start();

    setStatus(sensor::eStatus::CONNECTING);

    mIsRunning = mCamera->isRunning();

    setStatus(sensor::eStatus::RUNNING);

    mFrameTimer.start();

	return true;
}

void cTeledyneFlirCameraModel_T1K::stopCommunications()
{
    mFrameTimer.stop();

    mCamera->stop();

    setStatus(sensor::eStatus::STOPPED);

    mIsRunning = false;
}

bool cTeledyneFlirCameraModel_T1K::updateFrameInterval(uint32_t frame_interval_ms)
{
    mFrameTimer.time_ms(frame_interval_ms);

    return true;
}

bool cTeledyneFlirCameraModel_T1K::updateFrameRate(double frame_rate_fps)
{
    return true;
}

void cTeledyneFlirCameraModel_T1K::update()
{
    if (!mIsRunning) return;

    if (!mCamera->isRunning())
    {
        setStatus(sensor::eStatus::FAILED);

        mIsRunning = false;
        return;
    }

    bool newData = false;

    switch (mMode)
    {
    case eMode::SINGLE:
        if (mPhotoRequested)
        {
            if (mCamera->isNewImageReady())
            {
                mCurrentImage = mCamera->getImage();
                mCurrentImage.setTimestamp_ns(cTimestampProvider::timestamp_ns());
                newData = true;
                mPhotoRequested = false;
            }
        }
        break;
    case eMode::TIME_LAPSE:
        if (mFrameTimer.elapsed() && mCamera->isNewImageReady())
        {
            mCurrentImage = mCamera->getImage();
            mCurrentImage.setTimestamp_ns(cTimestampProvider::timestamp_ns());
            newData = true;
            mFrameTimer.start();
        }
        break;
    case eMode::CONTINUOUS:
        if (mCamera->isNewImageReady())
        {
            mCurrentImage = mCamera->getImage();
            mCurrentImage.setTimestamp_ns(cTimestampProvider::timestamp_ns());
            newData = true;
        }
        break;
    }

    if (newData)
    {
        if (mSerializer)
        {
            mSerializer.writeThermalImage(mInstanceID, mCurrentImage);
        }

        if (mImageRequested)
        {
            for (std::size_t i = 0; i < mCurrentImage.size(); ++i)
            {
                auto color = mColorTable.getColor(i);

                qRgb(color.red, color.green, color.blue);

//                mColorizedImage.

                //                mColorizedImage.setColor(i, )
                //                QImage mColorizedImage;
            }

        }
    }
}

void cTeledyneFlirCameraModel_T1K::errorHappend(int id, QString msg)
{
    QString full_msg = "Camera ";
    full_msg += QString::number(id);
    full_msg += ": " + msg;
    logMessage(logERROR, full_msg);

    setStatus(sensor::eStatus::FAILED);
}

