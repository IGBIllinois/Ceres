
#include "AxisCommunicationsModel_file.hpp"

#include <iostream>

cAxisCommunicationsModel_file::cAxisCommunicationsModel_file(QObject* parent)
:
    QObject(parent)
{
}

cAxisCommunicationsModel_file::~cAxisCommunicationsModel_file()
{
    std::cerr << numImages;
}

void cAxisCommunicationsModel_file::onActiveCameraId(int id)
{
    mCameraId = id;
}

void cAxisCommunicationsModel_file::onFramesPerSecond(int frames_per_sec)
{
    mFramesPerSec = frames_per_sec;
}

void cAxisCommunicationsModel_file::onBitmap(const QBitmap& in)
{}

void cAxisCommunicationsModel_file::onJPEG(const QImage& in)
{}

void cAxisCommunicationsModel_file::onMpegFrame(const QImage& image)
{
    mImage = image;
    emit onNewImage(mImage);
    ++numImages;
}

void cAxisCommunicationsModel_file::onImageSize(int width, int height)
{
    mImageWidth = width;
    mImageHeight = height;
}

