
#include "TritonCamera.hpp"
#include "LucidVisionLabsRgbUtils.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QEventLoop>
#include <QByteArray>
#include <QUrlQuery>
#include <QImageReader>

#include <nlohmann/json.hpp>


cTritonCamera::cTritonCamera(int id, QObject* parent)
:
    QObject(parent), mCameraID(id),
    mpImageBuffer(nullptr), mpImageReader(nullptr), mpCurrentImage(nullptr),
    mpDownloadManager(nullptr), mpRequest(nullptr), mpReply(nullptr)

{
    mpImageReader = new QImageReader();
    mpImageReader->setAutoDetectImageFormat(true);

    mpCurrentImage = new QImage(640, 480, QImage::Format_RGB888);
    mpImageBuffer = new QBuffer(this);
    mpImageBuffer->open(QBuffer::ReadWrite);
    mpImageReader->setDevice(mpImageBuffer);

    mCurrentState = GrabbingState::Off;
    mCurrentImageSize = 0;

    mpDownloadManager = new QNetworkAccessManager(this);
    connect(mpDownloadManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    mpRequest = new QNetworkRequest();

    mFramesPerSeconds = 0;
}

cTritonCamera::~cTritonCamera()
{
}


QString cTritonCamera::errorMessage() const
{
    return mErrorStr;
}

void cTritonCamera::setSource(QUrl url)
{
    QUrl checkUrl(url);
    if (checkUrl.isValid())
    {
        mCurrentUrl = checkUrl;
    }
    else
    {
        mErrorStr = tr("Invalid URL");
        emit errorHappend(mCameraID, mErrorStr);
    }
}

uint8_t cTritonCamera::getFramesPerSeconds() const
{
    return mFramesPerSeconds;
}

void cTritonCamera::setFramesPerSeconds(uint8_t fps)
{
    mFramesPerSeconds = fps;
}

rgb::sImageSize_t cTritonCamera::getImageSize() const
{
    return mImageSize;
}

void cTritonCamera::setImageSize(rgb::sImageSize_t image_size)
{
    mImageSize = image_size;
    if ((mpCurrentImage->width() != mImageSize.width) ||
        (mpCurrentImage->height() != mImageSize.height))
    {
        delete mpCurrentImage;
        mpCurrentImage = new QImage(mImageSize.width, mImageSize.height, QImage::Format_RGB888);
    }
}

QImage* cTritonCamera::currentImage() const
{
    return mpCurrentImage;
}

bool cTritonCamera::startGrabbing()
{
    mCurrentUrl.setPath("/axis-cgi/mjpg/video.cgi");

    QUrlQuery query;
    query.addQueryItem("camera", QString::number(mCameraID));

    if (mFramesPerSeconds > 0)
        query.addQueryItem("fps", QString::number(mFramesPerSeconds));

    if (mImageSize != rgb::sImageSize_t())
        query.addQueryItem("resolution", QString::fromStdString(lucid::to_string(mImageSize)));

    mCurrentUrl.setQuery(query);

    mCurrentState = GrabbingState::TurnOn;
    emit stateChanged(mCameraID, mCurrentState);

    return sendRequest();
}

void cTritonCamera::stopGrabbing()
{
    mCurrentState = GrabbingState::TurnOff;
    emit stateChanged(mCameraID, mCurrentState);
    if (mpReply)
    {
        mpReply->abort();
    }
}

bool cTritonCamera::sendRequest()
{
    if (!mCurrentUrl.isValid())
        return false;

    mpRequest->setUrl(mCurrentUrl);

    mpReply = mpDownloadManager->get(*mpRequest);
    connect(mpReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    connect(mpReply, SIGNAL(readyRead()), this, SLOT(replyDataAvailable()));

    mCurrentImageSize = 0;
    mpImageBuffer->seek(0);

    emit stateChanged(mCameraID, GrabbingState::TurnOn);
    return true;
}


/** Private Slot **/
void cTritonCamera::downloadFinished(QNetworkReply* reply)
{
    mpImageReader->setDevice(reply);
    qWarning() << "start reading image";
    mpImageReader->read(mpCurrentImage);
    qWarning() << "reading image done";
    emit imageGrabbed(mCameraID, mpCurrentImage);

    mCurrentState = GrabbingState::Off;
    emit stateChanged(mCameraID, mCurrentState);
}

void cTritonCamera::downloadError(QNetworkReply::NetworkError)
{
    if (mpReply && (mCurrentState == GrabbingState::On))
    {
        mErrorStr = mpReply->errorString();
        emit errorHappend(mCameraID, mErrorStr);
    }
}

void cTritonCamera::replyDataAvailable()
{
    if (mCurrentState != GrabbingState::On)
    {
        mCurrentState = GrabbingState::On;
        emit stateChanged(mCameraID, mCurrentState);
    }

    auto bytesRemaining = mpReply->bytesAvailable();

    if (bytesRemaining <= 0)
        return;

    mNetworkData = mpReply->readAll();

    auto boundaryPos = mNetworkData.indexOf("--myboundary");
    if (boundaryPos < 0)
    {
        mpImageBuffer->write(mNetworkData);
        mNetworkData.clear();
    }
    else if (boundaryPos > 0)
    {
        mpImageBuffer->write(mNetworkData.left(boundaryPos));
        mNetworkData = mNetworkData.chopped(boundaryPos);
    }

    auto buffer_pos = mpImageBuffer->pos();
    if ((buffer_pos >= mCurrentImageSize) && (mCurrentImageSize>0))
    {
        bufferToImage();
    }

    if (mNetworkData.contains("--myboundary"))
    {
        int i = mNetworkData.indexOf("Content-Length:");
        if (i < 0)
        {
            qWarning() << QString("Packet did not contain \"Content-Length:\"");
            mNetworkData.clear();
            return;
        }

        i += 16; // Adding the number of characters in "Content-Length:"
        int n = mNetworkData.indexOf('\r', i);

        bool ok = false;
        mCurrentImageSize = mNetworkData.mid(i, n - i).toInt(&ok);
        if (!ok)
        {
            QString str = mNetworkData.mid(i, n - i);
            qWarning() << QString("Could not convert %1 to number").arg(str);
            mNetworkData.clear();
            return;
        }

        n += 4; // The HTTP reply header ends with "\r\n\r\n"

        auto len = mNetworkData.size() - n;
        mpImageBuffer->write(mNetworkData.right(len));
    }

    mNetworkData.clear();
}

void cTritonCamera::bufferToImage()
{

    bool ok = false;
    mpImageReader->setDevice(mpImageBuffer);
    mpImageBuffer->seek(0);
    ok = mpImageReader->read(mpCurrentImage);
    mpImageBuffer->seek(0);

    if (ok)
    {
        emit frameGrabbed(mCameraID, mpCurrentImage);
    }
    else
    {
        mErrorStr = "Image read fail: ";
        mErrorStr += mpImageReader->errorString();
        qWarning() << mErrorStr;
    }
}

void cTritonCamera::newImageTimeOut()
{
}

void cTritonCamera::requestReceived(QNetworkReply* pReply)
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
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Get the redirection url
            QUrl newUrl = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative, 
            // we have to use the previous one to resolve it 
            mCurrentUrl = pReply->url().resolved(newUrl);
        }
    }
    else
    {
        // Error
        qWarning() << pReply->errorString();
    }
}

