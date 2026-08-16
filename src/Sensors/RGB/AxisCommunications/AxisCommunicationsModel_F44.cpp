
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsFactory.hpp"
#include "AxisCommunicationsUtils.hpp"

#include "TimestampProvider.hpp"

#include <cbdf/BlockDataFile.hpp>

#include <QDebug>
#include <QNetworkAccessManager>
#include <QMessageBox>

#include <filesystem>
#include <algorithm>


const std::size_t MAX_CAMERAS = 4;

#define USE_LOG_MESSAGE


namespace
{
    static uint8_t axis_comm_F44_device_id = 0;
}


namespace
{
    bool operator>(const rgb::sImageSize_t& lhs, const rgb::sImageSize_t& rhs)
    {
        return ((lhs.height * lhs.width) > (rhs.height * rhs.width));
    }
}

cAxisCommunicationsModel_F44::cAxisCommunicationsModel_F44(QObject* parent)
:
    cAxisCommunicationsModel("Axis F44 Webcam", parent),
    mImageData(),
    mImageBuffer(&mImageData),
    mpActiveCamera(nullptr), 
    mDeviceID(++axis_comm_F44_device_id)
{
    mModel = "AXIS F44 DUAL AUDO INPUT";

    mImageBuffer.open(QIODevice::ReadWrite);

    mMinFrameRate_fps = 1;
    mMaxFrameRate_fps = 50;
}

cAxisCommunicationsModel_F44::~cAxisCommunicationsModel_F44()
{
	stopCommunications();

    for (auto* camera : mCameras)
    {
        delete camera;
        camera = nullptr;
    }
}

uint8_t cAxisCommunicationsModel_F44::device_id() const
{
    return mDeviceID;
}

void cAxisCommunicationsModel_F44::updateViews()
{
    emit sensorStatusChanging(q_name(), q_instance(), getStatus());

    emit cameraIdChanged(getActiveCameraID());
    emit frameRateChanged(getActiveFramesRate_fps());

    auto s = getActiveImageSize();
    emit imageSizeChanged(s.width, s.height);

    cAxisCommunicationsModel::updateViews();
}

bool cAxisCommunicationsModel_F44::configure(const nlohmann::json& jsonCfg)
{
    rgb::sImageSize_t max_image_size;

    try
    {
        auto section = jsonCfg["F44"];

        if (!cAxisCommunicationsModel::configure(section))
        {
            setStatus(sensor::eStatus::FAILED);
            return false;
        }

        auto cameras = section["cameras"];

        if (MAX_CAMERAS <= cameras.size())
        {
            QString str = "Error in the \"axis_communications\" configuration:\n";
            str.append("The F44 controller only supports a maximum of four cameras.");
            logMessage(logERROR, str);

            setStatus(sensor::eStatus::FAILED);
            return false;
        }

        for (int i = 0; i < cameras.size(); ++i)
        {
            auto camera_info = cameras[i];
            int id = camera_info["id"];
            
            if (mMinCameraID == -1)
                mMinCameraID = id;
            else
                mMinCameraID = std::min(mMinCameraID, id);

            mMaxCameraID = std::max(mMaxCameraID, id);

            std::unique_ptr<cAxisCamera> pCamera = std::make_unique<cAxisCamera>(id, this);

            rgb::sImageSize_t image_size = axis::to_image_size(camera_info["resolution"]);
            if (image_size > max_image_size)
                max_image_size = image_size;

            auto it = std::find(mSupportedImageSizes.begin(), mSupportedImageSizes.end(), image_size);
            if (it == mSupportedImageSizes.end())
            {
                QString str = "Error in the \"axis_communications\" configuration:\n";
                str.append("The F44 controller only supports a maximum of four cameras.");
                logMessage(logERROR, str);

                setStatus(sensor::eStatus::FAILED);
                return false;
            }
            pCamera->setImageSize(image_size);

            int fps = camera_info["frames per second"];
            pCamera->setFramesPerSeconds(fps);

            pCamera->setSource(mUrl);

            connect(pCamera.get(), &cAxisCamera::frameGrabbed, this, &cAxisCommunicationsModel_F44::frameGrabbed);
            connect(pCamera.get(), &cAxisCamera::imageGrabbed, this, &cAxisCommunicationsModel_F44::imageGrabbed);
            connect(pCamera.get(), &cAxisCamera::errorHappend, this, &cAxisCommunicationsModel_F44::errorHappend);
            connect(pCamera.get(), &cAxisCamera::stateChanged, this, &cAxisCommunicationsModel_F44::stateChanged);

            mCameras.push_back(pCamera.release());

            emit enableCamera(id);
        }

        emit cameraRangeUpdated(mMinCameraID, mMaxCameraID);

        int default_id = section["default camera id"];
        setActiveCamera(default_id);

    }
    catch (const std::exception& e)
    {
        setStatus(sensor::eStatus::FAILED);
        qCritical() << "Axis Communications F44 failed configuration";
        logMessage(logERROR, "Axis Communications F44 failed configuration");
        return false;
    }

    size_t buffer_size = max_image_size.height * max_image_size.width;

    mSerializer.setBufferCapacity(buffer_size + 1024);

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

void cAxisCommunicationsModel_F44::enableDataRecording(cBlockDataFileWriter& file)
{
    std::filesystem::path filename = file.filename();
    std::filesystem::path path = filename.parent_path();

    QString qPath = QString::fromStdString(path.string());

    emit defaultDataPathChanged(qPath);

    mSerializer.attach(&file);
}

void cAxisCommunicationsModel_F44::disableDataRecording()
{
    cAxisCommunicationsModel::disableDataRecording();
    mSerializer.detach();
}

void cAxisCommunicationsModel_F44::writeDataHeader()
{
    if (!mpActiveCamera) return;

    mSerializer.writeMode(mDeviceID, static_cast<uint8_t>(mMode));

    if (mMode == eMode::TIME_LAPSE)
        mSerializer.writeLapseTime(mDeviceID, mLapseInterval_ms);

    mSerializer.writeActiveCameraId(mDeviceID, mpActiveCamera->cameraID());

    auto size = mpActiveCamera->getImageSize();
    mSerializer.writeImageSize(mDeviceID, size.width, size.height);

    mSerializer.writeFramesPerSecond(mDeviceID, mpActiveCamera->getFramesPerSeconds());
}

bool cAxisCommunicationsModel_F44::startCommunications()
{
    if (!mConnected) return false;

    mpActiveCamera->startGrabbing();

    setStatus(sensor::eStatus::CONNECTING);

	return true;
}

void cAxisCommunicationsModel_F44::stopCommunications()
{
    if (!mConnected) return;
    mpActiveCamera->stopGrabbing();
    setStatus(sensor::eStatus::STOPPED);
}

void cAxisCommunicationsModel_F44::requestReceived(QNetworkReply* pReply)
{
	pReply->deleteLater();

    if (pReply->error() == QNetworkReply::NoError) 
    {
        // Get the http status code
        int v = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            // Here we got the final reply 
            QString replyText = pReply->readAll();
            processReply(replyText.toStdString());
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Get the redirection url
            QUrl newUrl = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative, 
            // we have to use the previous one to resolve it 
            mUrl = pReply->url().resolved(newUrl);
        }
    }
    else
    {
        // Error
        pReply->errorString();
    }
}

void cAxisCommunicationsModel_F44::setMode(eMode mode)
{
    mMode = mode;

    if (mMode == eMode::TIME_LAPSE)
        mTimeLapseTimer.start();
    else
        mTimeLapseTimer.stop();

    if (static_cast<bool>(mSerializer))
    {
        mSerializer.writeMode(mDeviceID, static_cast<uint8_t>(mMode));

        if (mMode == eMode::TIME_LAPSE)
            mSerializer.writeLapseTime(mDeviceID, mLapseInterval_ms);
    }

    emit modeChanged(static_cast<int>(mMode));
}

void cAxisCommunicationsModel_F44::setFrameRate_Hz(double frame_rate_hz)
{
    if (mMinFrameRate_fps.has_value())
    {
        if (frame_rate_hz < mMinFrameRate_fps.value())
            frame_rate_hz = mMinFrameRate_fps.value();
    }
    else if (frame_rate_hz < 0)
        frame_rate_hz = 0;

    if (mMaxFrameRate_fps.has_value())
    {
        if (frame_rate_hz > mMaxFrameRate_fps.value())
            frame_rate_hz = mMaxFrameRate_fps.value();
    }

    bool changing = frame_rate_hz != mFrameRate_fps;

    if (updateFrameRate(frame_rate_hz))
    {
        mFrameRate_fps = frame_rate_hz;

        if (static_cast<bool>(mSerializer))
        {
            mSerializer.writeFramesPerSecond(mDeviceID, mFrameRate_fps);
        }
    }

    emit frameRateChanged(mFrameRate_fps);
}

void cAxisCommunicationsModel_F44::setLapseInterval_ms(uint32_t interval_ms)
{
    if (interval_ms < 100)
        interval_ms = 100;

    bool changing = interval_ms != mLapseInterval_ms;

    if (updateLapseInterval(interval_ms))
    {
        mLapseInterval_ms = interval_ms;
        mTimeLapseTimer.time_ms(mLapseInterval_ms);

        if (static_cast<bool>(mSerializer))
        {
            mSerializer.writeLapseTime(mDeviceID, mLapseInterval_ms);
        }
    }

    emit lapseIntervalChanged(mLapseInterval_ms);
}

int cAxisCommunicationsModel_F44::getActiveCameraID() const
{
    if (mpActiveCamera)
        return mpActiveCamera->cameraID();

    return -1;
}

void cAxisCommunicationsModel_F44::setActiveCamera(int id)
{
    cAxisCamera* pCamera = nullptr;

    if (mpActiveCamera && (id == mpActiveCamera->cameraID()))
    {
        return;
    }

    for (auto* camera : mCameras)
    {
        if (camera->cameraID() == id)
        {
            pCamera = camera;
            break;
        }
    }

    // Check to make sure the camera was found
    if (!pCamera)
        return;

    bool isGrabbing = false;

    if (mpActiveCamera)
    {
        isGrabbing = mpActiveCamera->isGrabbing();
        mpActiveCamera->stopGrabbing();
    }

    if (isGrabbing)
        pCamera->startGrabbing();

    mpActiveCamera = pCamera;

    if (static_cast<bool>(mSerializer))
    {
        mSerializer.writeActiveCameraId(mDeviceID, mpActiveCamera->cameraID());
        mSerializer.writeFramesPerSecond(mDeviceID, mpActiveCamera->getFramesPerSeconds());

        auto size = mpActiveCamera->getImageSize();
        mSerializer.writeImageSize(mDeviceID, size.width, size.height);
    }

    emit cameraIdChanged(id);
    emit frameRateChanged(mpActiveCamera->getFramesPerSeconds());

    auto s = mpActiveCamera->getImageSize();
    emit imageSizeChanged(s.width, s.height);
}

rgb::sImageSize_t cAxisCommunicationsModel_F44::getActiveImageSize() const
{
    if (mpActiveCamera)
        return mpActiveCamera->getImageSize();

    return rgb::sImageSize_t{0,0};
}

void cAxisCommunicationsModel_F44::setActiveImageSize(rgb::sImageSize_t image_size)
{
    if (!mpActiveCamera)
        return;

    if ((image_size.width > 1920) || (image_size.height > 1080))
        return;

    mpActiveCamera->setImageSize(image_size);

    QString msg = "Camera ";
    msg += QString::number(mpActiveCamera->cameraID());
    msg += " image size set to ";
    msg += QString::number(image_size.width);
    msg += "x";
    msg += QString::number(image_size.height);

#ifdef USE_LOG_MESSAGE
    logMessage(logSTATUS, msg);
#else
    emit statusMessage(msg);
#endif

    if (static_cast<bool>(mSerializer))
    {
        auto size = mpActiveCamera->getImageSize();
        mSerializer.writeImageSize(mDeviceID, size.width, size.height);
    }

    emit imageSizeChanged(image_size.width, image_size.height);
}

int cAxisCommunicationsModel_F44::getActiveFramesRate_fps() const
{
    if (mpActiveCamera)
        return mpActiveCamera->getFramesPerSeconds();

    return -1;
}

void cAxisCommunicationsModel_F44::setActiveFramesRate_fps(int fps)
{
    if (!mpActiveCamera)
        return;

    if ((fps < 1) || (fps > 250))
        return;

    mpActiveCamera->setFramesPerSeconds(fps);

    QString msg = "Camera ";
    msg += QString::number(mpActiveCamera->cameraID());
    msg += " frame per second set to ";
    msg += QString::number(fps);

#ifdef USE_LOG_MESSAGE
    logMessage(logSTATUS, msg);
#else
    emit statusMessage(msg);
#endif

    mFrameRate_fps = mpActiveCamera->getFramesPerSeconds();

    if (static_cast<bool>(mSerializer))
    {
        mSerializer.writeFramesPerSecond(mDeviceID, mpActiveCamera->getFramesPerSeconds());
    }

    emit frameRateChanged(fps);
}

void cAxisCommunicationsModel_F44::onSaveState()
{
    sState state;

    state.mode = mode();
    state.frame_rate_fps = getActiveFramesRate_fps();
    state.lapse_interval_ms = lapseInterval_ms();
    state.cameraId = getActiveCameraID();
    state.resolution = getActiveImageSize();

    mStateStack.push_back(state);
}

void cAxisCommunicationsModel_F44::onRestoreState()
{
    if (mStateStack.empty()) return;

    auto state = mStateStack.back();
    mStateStack.pop_back();

    setActiveCamera(state.cameraId);
    setActiveImageSize(state.resolution);
    setActiveFramesRate_fps(state.frame_rate_fps);

    requestMode(state.mode);
    requestLapseInterval_ms(state.lapse_interval_ms);
}


bool cAxisCommunicationsModel_F44::updateLapseInterval(uint32_t interval_ms)
{
    mLapseInterval_ms = interval_ms;

    return true;
}

bool cAxisCommunicationsModel_F44::updateFrameRate(double frame_rate_fps)
{
    if (frame_rate_fps == mFrameRate_fps)
        return true;

    setActiveFramesRate_fps(static_cast<int>(frame_rate_fps));

    return true;
}

bool cAxisCommunicationsModel_F44::updateImageSize(int width, int height)
{
    rgb::sImageSize_t image_size = {static_cast<uint16_t>(width), static_cast<uint16_t>(height)};

    // Find element with minimum absolute difference from target
    auto closest_it = std::min_element(mSupportedImageSizes.begin(), mSupportedImageSizes.end(),
            [image_size](auto a, auto b) {
                return std::abs((a.width - image_size.width) + (a.height - image_size.height)) < 
                    std::abs((b.width - image_size.width) + (b.height - image_size.height));
            });

    setActiveImageSize(*closest_it);
    
    return true;
}

void cAxisCommunicationsModel_F44::frameGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;
    if (mAutoEmitImages || mImageRequested)
    {
        emit onNewImage(mCurrentImage);

        mImageRequested = false;
    }

    bool newData = false;

    switch (mMode)
    {
    case eMode::SINGLE:
        if (mPhotoRequested)
        {
            newData = true;
            mPhotoRequested = false;
            emit photoTaken();
        }
        break;
    case eMode::TIME_LAPSE:
        if (mTimeLapseTimer.elapsed())
        {
            newData = true;
            mTimeLapseTimer.start();
        }
        break;
    case eMode::CONTINUOUS:
        newData = true;
        break;
    }

    if (newData)
    {
        if (mIsRecording && static_cast<bool>(mSerializer))
        {
            try
            {
/*
#ifdef USE_LOG_MESSAGE
                logMessage(logSTATUS, "Saving MPEG Frame Buffer");
#else
                emit statusMessage("Saving MPEG Frame Buffer");
#endif
*/

                axis::to_buffer(mCurrentImage, mMpegFrameBuffer);
                mSerializer.write(mDeviceID, cTimestampProvider::timestamp_ns(), mMpegFrameBuffer);
            }
            catch (const std::exception& e)
            {
                QString msg = "Error in writing MPEG frame: ";
                msg += e.what();
                logMessage(logERROR, msg);
                qCritical() << msg;
            }
        }
    }
}

void cAxisCommunicationsModel_F44::imageGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;

    if (mAutoEmitImages || mImageRequested)
    {
        emit onNewImage(mCurrentImage);

        mImageRequested = false;
    }

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        try
        {
/*
#ifdef USE_LOG_MESSAGE
            logMessage(logSTATUS, "Saving JPEG Frame Buffer");
#else
            emit statusMessage("Saving JPEG Frame Buffer");
#endif
*/

            axis::to_buffer(mCurrentImage, mJpegBuffer);
            mSerializer.write(mDeviceID, cTimestampProvider::timestamp_ns(), mJpegBuffer);
        }
        catch (const std::exception& e)
        {
            QString msg = "Error in writing JPEG: ";
            msg += e.what();
            logMessage(logERROR, msg);
            qCritical() << msg;
        }
    }
}

void cAxisCommunicationsModel_F44::errorHappend(int id, QString msg)
{
    QString full_msg = "Camera ";
    full_msg += QString::number(id);
    full_msg += ": " + msg;
    logMessage(logERROR, full_msg);

    setStatus(sensor::eStatus::FAILED);
}

void cAxisCommunicationsModel_F44::stateChanged(int id, cAxisCamera::GrabbingState newState)
{
    QString msg = "Camera ";
    msg += QString::number(id);

    switch (newState)
    {
    case cAxisCamera::GrabbingState::Off:
        msg += " is now off.";
        break;
    case cAxisCamera::GrabbingState::TurnOff:
        msg += " is turning off.";
        break;
    case cAxisCamera::GrabbingState::TurnOn:
        msg += " is turning on.";
        break;
    case cAxisCamera::GrabbingState::On:
        msg += " is now on.";
        setStatus(sensor::eStatus::RUNNING);
        break;
    case cAxisCamera::GrabbingState::Error:
        msg += " has an error.";
        break;
    }

#ifdef USE_LOG_MESSAGE
    logMessage(logSTATUS, msg);
#else
    emit statusMessage(msg);
#endif
}


