
#include "AxisCamera.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QEventLoop>
#include <QByteArray>
#include <QUrlQuery>
#include <QImageReader>

#include <nlohmann/json.hpp>


cAxisCamera::cAxisCamera(int id, QObject* parent)
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

    mTimestampInMs = 0;
    mTimestampRegexp.setPattern("X-Timestamp: (\\d+).(\\d+)\\r\\n");

    mFramesPerSeconds = 0;
}

cAxisCamera::~cAxisCamera()
{
}


QString cAxisCamera::errorMessage() const
{
    return mErrorStr;
}

void cAxisCamera::setSource(QUrl url)
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

uint8_t cAxisCamera::getFramesPerSeconds() const
{
    return mFramesPerSeconds;
}

void cAxisCamera::setFramesPerSeconds(uint8_t fps)
{
    mFramesPerSeconds = fps;
}

axis::sImageSize_t cAxisCamera::getImageSize() const
{
    return mImageSize;
}

void cAxisCamera::setImageSize(axis::sImageSize_t image_size)
{
    mImageSize = image_size;
    if ((mpCurrentImage->width() != mImageSize.width) ||
        (mpCurrentImage->height() != mImageSize.height))
    {
        delete mpCurrentImage;
        mpCurrentImage = new QImage(mImageSize.width, mImageSize.height, QImage::Format_RGB888);
    }
}

QImage* cAxisCamera::currentImage() const
{
    return mpCurrentImage;
}

bool cAxisCamera::startGrabbing()
{
    mCurrentUrl.setPath("/axis-cgi/mjpg/video.cgi");

    QUrlQuery query;
    query.addQueryItem("camera", QString::number(mCameraID));

    if (mFramesPerSeconds > 0)
        query.addQueryItem("fps", QString::number(mFramesPerSeconds));

    if (mImageSize != axis::sImageSize_t())
        query.addQueryItem("resolution", QString::fromStdString(axis::to_string(mImageSize)));

    mCurrentUrl.setQuery(query);

    mCurrentState = GrabbingState::TurnOn;
    emit stateChanged(mCameraID, mCurrentState);

    return sendRequest();
}

void cAxisCamera::stopGrabbing()
{
    mCurrentState = GrabbingState::TurnOff;
    emit stateChanged(mCameraID, mCurrentState);
    if (mpReply)
    {
        mpReply->abort();
    }
}

bool cAxisCamera::sendRequest()
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
void cAxisCamera::downloadFinished(QNetworkReply* reply)
{
    mpImageReader->setDevice(reply);
    qWarning() << "start reading image";
    mpImageReader->read(mpCurrentImage);
    qWarning() << "reading image done";
    emit imageGrabbed(mCameraID, mpCurrentImage);

    mCurrentState = GrabbingState::Off;
    emit stateChanged(mCameraID, mCurrentState);
}

void cAxisCamera::downloadError(QNetworkReply::NetworkError)
{
    if (mpReply && (mCurrentState == GrabbingState::On))
    {
        mErrorStr = mpReply->errorString();
        emit errorHappend(mCameraID, mErrorStr);
    }
}

void cAxisCamera::replyDataAvailable()
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
/*
        mpImageBuffer->seek(mCurrentImageSize);
        auto remainder = mpImageBuffer->readAll();
        auto b1 = remainder.at(0);
        auto b2 = remainder.at(1);
*/
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

void cAxisCamera::bufferToImage()
{

    bool ok = false;
    mpImageReader->setDevice(mpImageBuffer);
    mpImageBuffer->seek(0);
    ok = mpImageReader->read(mpCurrentImage);
    auto buffer_pos = mpImageBuffer->pos();
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

void cAxisCamera::newImageTimeOut()
{
}

void cAxisCamera::requestReceived(QNetworkReply* pReply)
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

