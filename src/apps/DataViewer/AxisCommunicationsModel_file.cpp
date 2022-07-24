
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
{}

void cAxisCommunicationsModel_file::onFramesPerSecond(int frames_per_sec)
{}

void cAxisCommunicationsModel_file::onBitmap(const QBitmap& in)
{}

void cAxisCommunicationsModel_file::onJPEG(const QImage& in)
{}

void cAxisCommunicationsModel_file::onMpegFrame(const QImage& image)
{
    mImage = image;
    onNewImage(mImage);
    ++numImages;
}

void cAxisCommunicationsModel_file::onImageSize(const axis::sImageSize_t& in)
{}

