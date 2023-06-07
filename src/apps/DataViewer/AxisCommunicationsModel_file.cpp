
#include "AxisCommunicationsModel_file.hpp"
#include "RGB/AxisCommunications/AxisCommunicationsUtils.hpp"

#include <QMessageBox>

cAxisCommunicationsModel_file::cAxisCommunicationsModel_file(QObject* parent)
:
    QObject(parent)
{
}

cAxisCommunicationsModel_file::~cAxisCommunicationsModel_file()
{}

void cAxisCommunicationsModel_file::onActiveCameraId(int id)
{
    mCameraId = id;
    emit updateActiveCameraId(mCameraId);

}

void cAxisCommunicationsModel_file::onFramesPerSecond(int frames_per_sec)
{
    mFramesPerSec = frames_per_sec;
    emit updateFramesRate(mFramesPerSec);
}

void cAxisCommunicationsModel_file::onBitmap(const cBitmapBuffer& buffer)
{}

void cAxisCommunicationsModel_file::onJPEG(const cJpegBuffer& buffer)
{}

void cAxisCommunicationsModel_file::onMpegFrame(const cMpegFrameBuffer& buffer)
{
    try
    {
        axis::to_image(buffer, mImage);
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
        return;
    }

    emit onNewImage(mImage);

    ++numImages;
}

void cAxisCommunicationsModel_file::onImageSize(int width, int height)
{
    mImageWidth = width;
    mImageHeight = height;
    emit updateImageSize(mImageWidth, mImageHeight);
}

