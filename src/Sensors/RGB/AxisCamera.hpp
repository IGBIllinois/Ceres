
#pragma once

#include "AxisCommunicationsUtils.hpp"

#include <QNetworkReply>
#include <QUrl>
#include <QBitmap>
#include <QImage>
#include <QBuffer>

#include <vector>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE


class cAxisCamera : public QObject
{
    Q_OBJECT

public:
    enum class GrabbingState
    {
        Off = 0,
        TurnOn,
        On,
        TurnOff,
        Error
    };

public:
    cAxisCamera(int id, QObject* parent = nullptr);
    virtual ~cAxisCamera();

    int cameraID() const { return mCameraID;  }

    QString errorMessage() const;

    void setSource(QUrl url);
    QString currentSource() { return mCurrentUrl.toString(); }

    uint8_t getFramesPerSeconds() const;
    void setFramesPerSeconds(uint8_t fps);

    axis::sImageSize_t getImageSize() const;
    void setImageSize(axis::sImageSize_t image_size);

    QImage* currentImage() const;

    bool startGrabbing();
    void stopGrabbing();
    bool isGrabbing() const { return (mCurrentState == GrabbingState::On); }

signals:
    void imageGrabbed(int id, QImage* img);
    void errorHappend(int id, QString msg);
    void stateChanged(int id, GrabbingState newState);

protected slots:
    void requestReceived(QNetworkReply* pReply);

private slots:
    void downloadError(QNetworkReply::NetworkError);
    void downloadFinished(QNetworkReply* reply);
    void replyDataAvailable();
    void newImageTimeOut();

private:
    bool sendRequest();
    void bufferToImage();

private:
    const int mCameraID;

    QBuffer* mpImageBuffer;
    QImageReader* mpImageReader;
    QImage* mpCurrentImage;

    QNetworkAccessManager* mpDownloadManager;
    QNetworkRequest* mpRequest;
    QNetworkReply* mpReply;
    QByteArray  mNetworkData;
    int mLastState;
    QUrl mCurrentUrl;
    GrabbingState mCurrentState;
    QString mErrorStr;

    int mCurrentImageSize;

    quint64 mTimestampInMs;
    QRegExp mTimestampRegexp;

    uint8_t mFramesPerSeconds;
    axis::sImageSize_t mImageSize;
};

