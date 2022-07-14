
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsFactory.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>

const std::size_t MAX_CAMERAS = 4;

namespace
{
    bool operator>(const axis::sImageSize_t& lhs, const axis::sImageSize_t& rhs)
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

bool cAxisCommunicationsModel_F44::configure(const nlohmann::json& jsonCfg)
{
    axis::sImageSize_t max_image_size;

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
            emit errorMessage("Configuration Error", str);

            setStatus(sensor::eStatus::FAILED);
            return false;
        }

        for (int i = 0; i < cameras.size(); ++i)
        {
            auto camera = cameras[i];
            int id = camera["id"];
            mCameras[i] = new cAxisCamera(id, this);

            axis::sImageSize_t image_size = axis::to_image_size(camera["resolution"]);
            if (image_size > max_image_size)
                max_image_size = image_size;

            auto it = std::find(mSupportedImageSizes.begin(), mSupportedImageSizes.end(), image_size);
            if (it == mSupportedImageSizes.end())
            {
                QString str = "Error in the \"axis_communications\" configuration:\n";
                str.append("The F44 controller only supports a maximum of four cameras.");
                emit errorMessage("Configuration Error", str);

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
        return false;
    }

    size_t buffer_size = max_image_size.height * max_image_size.width * 32;

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
    mSerializer.write(mpActiveCamera->getImageSize());
    mSerializer.writeFramesPerSecond(mpActiveCamera->getFramesPerSeconds());

}

bool cAxisCommunicationsModel_F44::startCommunications()
{
    mpActiveCamera->startGrabbing();

    setStatus(sensor::eStatus::CONNECTING);

	return true;
}

void cAxisCommunicationsModel_F44::stopCommunications()
{
    mpActiveCamera->stopGrabbing();
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
        mSerializer.write(mpActiveCamera->getImageSize());
    }
}

void cAxisCommunicationsModel_F44::frameGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;
    emit onNewImage(mCurrentImage);

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.writeMpegFrame(mCurrentImage);
    }
}

void cAxisCommunicationsModel_F44::imageGrabbed(int id, QImage* img)
{
    mCurrentImage = *img;
    emit onNewImage(mCurrentImage);

    if (mIsRecording && static_cast<bool>(mSerializer))
    {
        mSerializer.writeJPEG(mCurrentImage);
    }
}

void cAxisCommunicationsModel_F44::errorHappend(int id, QString msg)
{
    QString title = "Camera ";
    title += QString::number(id);
    title += " Error";
    emit errorMessage(title, msg);

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


