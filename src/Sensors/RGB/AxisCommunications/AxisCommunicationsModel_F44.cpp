
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsFactory.hpp"
#include "AxisCommunicationsUtils.hpp"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QMessageBox>

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

//    mCameras = {nullptr, nullptr, nullptr, nullptr};
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

    if (mIsRecording && static_cast<bool>(mSerializer))
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

    if (mIsRecording && static_cast<bool>(mSerializer))
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

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.writeFramesPerSecond(mDeviceID, mpActiveCamera->getFramesPerSeconds());
    }

    emit frameRateChanged(fps);
}

void cAxisCommunicationsModel_F44::frameGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;
    if (mAutoEmitImages)
    {
        emit onNewImage(mCurrentImage);
    }

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        try
        {
            axis::to_buffer(mCurrentImage, mMpegFrameBuffer);
            mSerializer.write(mDeviceID, mMpegFrameBuffer);
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

void cAxisCommunicationsModel_F44::imageGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;

    if (mAutoEmitImages)
    {
        emit onNewImage(mCurrentImage);
    }

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        try
        {
            axis::to_buffer(mCurrentImage, mJpegBuffer);
            mSerializer.write(mDeviceID, mJpegBuffer);
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


