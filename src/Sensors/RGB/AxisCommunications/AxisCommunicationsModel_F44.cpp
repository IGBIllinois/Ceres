
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsFactory.hpp"
#include "AxisCommunicationsUtils.hpp"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QMessageBox>

const std::size_t MAX_CAMERAS = 4;

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
    mpActiveCamera(nullptr)
{
    mModel = "AXIS F44 DUAL AUDO INPUT";

    mImageBuffer.open(QIODevice::ReadWrite);

    mCameras = {nullptr, nullptr, nullptr, nullptr};
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

void cAxisCommunicationsModel_F44::updateViews()
{

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
            emit logMessage(logERROR, q_name(), str);

            setStatus(sensor::eStatus::FAILED);
            return false;
        }

        for (int i = 0; i < cameras.size(); ++i)
        {
            auto camera = cameras[i];
            int id = camera["id"];
            mCameras[i] = new cAxisCamera(id, this);

            rgb::sImageSize_t image_size = axis::to_image_size(camera["resolution"]);
            if (image_size > max_image_size)
                max_image_size = image_size;

            auto it = std::find(mSupportedImageSizes.begin(), mSupportedImageSizes.end(), image_size);
            if (it == mSupportedImageSizes.end())
            {
                QString str = "Error in the \"axis_communications\" configuration:\n";
                str.append("The F44 controller only supports a maximum of four cameras.");
                emit logMessage(logERROR, q_name(), str);

                setStatus(sensor::eStatus::FAILED);
                return false;
            }
            mCameras[i]->setImageSize(image_size);

            int fps = camera["frames per second"];
            mCameras[i]->setFramesPerSeconds(fps);

            mCameras[i]->setSource(mUrl);

            connect(mCameras[i], &cAxisCamera::frameGrabbed, this, &cAxisCommunicationsModel_F44::frameGrabbed);
            connect(mCameras[i], &cAxisCamera::imageGrabbed, this, &cAxisCommunicationsModel_F44::imageGrabbed);
            connect(mCameras[i], &cAxisCamera::errorHappend, this, &cAxisCommunicationsModel_F44::errorHappend);
            connect(mCameras[i], &cAxisCamera::stateChanged, this, &cAxisCommunicationsModel_F44::stateChanged);

            emit enableCamera(id);
        }

        int default_id = section["default camera id"];
        setActiveCamera(default_id);

    }
    catch (const std::exception& e)
    {
        setStatus(sensor::eStatus::FAILED);
        qCritical() << "Axis Communications F44 failed configuration";
        emit logMessage(logERROR, q_name(), "Axis Communications F44 failed configuration");
        return false;
    }

    size_t buffer_size = max_image_size.height * max_image_size.width;

    mSerializer.setBufferCapacity(buffer_size + 1024);

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

    mSerializer.writeActiveCameraId(mpActiveCamera->cameraID());
    auto size = mpActiveCamera->getImageSize();
    mSerializer.writeImageSize(size.width, size.height);
    mSerializer.writeFramesPerSecond(mpActiveCamera->getFramesPerSeconds());

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
        mSerializer.writeActiveCameraId(mpActiveCamera->cameraID());
        auto size = mpActiveCamera->getImageSize();
        mSerializer.writeImageSize(size.width, size.height);
    }
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

    emit statusMessage(msg);

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        auto size = mpActiveCamera->getImageSize();
        mSerializer.writeImageSize(size.width, size.height);
    }
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

    emit statusMessage(msg);

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.writeFramesPerSecond(mpActiveCamera->getFramesPerSeconds());
    }
}

void cAxisCommunicationsModel_F44::frameGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;
    emit onNewImage(mCurrentImage);

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        try
        {
            axis::to_buffer(mCurrentImage, mMpegFrameBuffer);
            mSerializer.write(mMpegFrameBuffer);
        }
        catch (const std::exception& e)
        {
            QString msg = "Error in writing MPEG frame: ";
            msg += e.what();
            emit logMessage(logERROR, q_name(), msg);
            qCritical() << msg;
        }
    }
}

void cAxisCommunicationsModel_F44::imageGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;
    emit onNewImage(mCurrentImage);

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        try
        {
            axis::to_buffer(mCurrentImage, mJpegBuffer);
            mSerializer.write(mJpegBuffer);
        }
        catch (const std::exception& e)
        {
            QString msg = "Error in writing JPEG: ";
            msg += e.what();
            emit logMessage(logERROR, q_name(), msg);
            qCritical() << msg;
        }
    }
}

void cAxisCommunicationsModel_F44::errorHappend(int id, QString msg)
{
    QString title = "Camera ";
    title += QString::number(id);
    emit logMessage(logERROR, title, msg);

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
    emit statusMessage(msg);
}


