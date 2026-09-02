
#pragma once

#include "../RgbTypes.hpp"

#include <QNetworkReply>
#include <QUrl>
#include <QBitmap>
#include <QImage>
#include <QBuffer>

#include <vector>
#include <fstream>


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

    rgb::sImageSize_t getImageSize() const;
    void setImageSize(rgb::sImageSize_t image_size);

    const QImage& currentImage() const;

    bool startGrabbing();
    void stopGrabbing();
    bool isGrabbing() const { return (mCurrentState == GrabbingState::On); }

signals:
    void frameGrabbed(int id, const QImage& img);
    void imageGrabbed(int id, const QImage& img);
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

    QBuffer* mpImageBuffer = nullptr;
    QImageReader* mpImageReader = nullptr;
    QImage mCurrentImage;

    QNetworkAccessManager* mpDownloadManager = nullptr;
    QNetworkRequest* mpRequest = nullptr;
    QNetworkReply* mpReply = nullptr;
    QByteArray  mNetworkData;
    QUrl mCurrentUrl;
    GrabbingState mCurrentState;
    QString mErrorStr;

    int mCurrentImageSize;

    uint8_t mFramesPerSeconds;
    rgb::sImageSize_t mImageSize;
};

