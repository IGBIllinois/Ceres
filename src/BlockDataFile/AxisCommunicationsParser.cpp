
#include "AxisCommunicationsParser.hpp"
#include "AxisDataIdentifiers.hpp"
#include "BlockDataFile.hpp"
#include "../Sensors/RGB/AxisCommunications/AxisCommunicationsUtils.hpp"

#include <QBitmap>
#include <QImage>

#include <cassert>

using namespace axis;

cAxisCommunicationsParser::cAxisCommunicationsParser()
:
    cBlockParser()
{}

cBlockID& cAxisCommunicationsParser::blockID()
{
	return mBlockID;
}

void cAxisCommunicationsParser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    mBlockID.setVersion(major_version, minor_version);
    mBlockID.dataID(static_cast<axis::DataID>(data_id));

    switch (static_cast<axis::DataID>(data_id))
    {
    case DataID::CAMERA_ID:
        processActiveCameraId(buffer);
        break;
    case DataID::FRAMES_PER_SECOND:
        processFramesPerSecond(buffer);
        break;
    case DataID::BITMAP:
        processBitmap(buffer);
        break;
    case DataID::JPEG:
        processJPEG(buffer);
        break;
    case DataID::MPEG_FRAME:
        processMpegFrame(buffer);
        break;
    case DataID::RESOLUTION:
        processImageSize(buffer);
        break;
    }
}


void cAxisCommunicationsParser::processActiveCameraId(cDataBuffer& buffer)
{
    int id = buffer.get<int32_t>();

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processActiveCameraId.");

    onActiveCameraId(id);
}

void cAxisCommunicationsParser::processFramesPerSecond(cDataBuffer& buffer)
{
    int frames_per_sec = buffer.get<int32_t>();

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processFramesPerSecond.");

    onFramesPerSecond(frames_per_sec);
}

void cAxisCommunicationsParser::processBitmap(cDataBuffer& buffer)
{
    mBitmapBuffer.clear();

    auto n = buffer.get<uint64_t>();
    mBitmapBuffer.resize(n);

    buffer.read(mBitmapBuffer.data(), n);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processBitmap.");

    onBitmap(mBitmapBuffer);
}

void cAxisCommunicationsParser::processJPEG(cDataBuffer& buffer)
{
    mJpegBuffer.clear();

    auto n = buffer.get<uint64_t>();
    mJpegBuffer.resize(n);

    buffer.read(mJpegBuffer.data(), n);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processJPEG.");

    onJPEG(mJpegBuffer);
}

void cAxisCommunicationsParser::processMpegFrame(cDataBuffer& buffer)
{
    mMpegFrameBuffer.clear();

    auto n = buffer.get<uint64_t>();
    mMpegFrameBuffer.resize(n);

    buffer.read(mMpegFrameBuffer.data(), n);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processMpegFrame.");

    onMpegFrame(mMpegFrameBuffer);
}

void cAxisCommunicationsParser::processImageSize(cDataBuffer& buffer)
{
    rgb::sImageSize_t image_size;

    image_size.width  = buffer.get<uint16_t>();
    image_size.height = buffer.get<uint16_t>();

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processImageSize.");

    onImageSize(image_size);
}

#if 0
void cAxisCommunicationsSerializer::writeBitmap(const QBitmap& in)
{
    assert(mpDataFile);

    mImageWriter.setFormat("bmp");
    mImageWriter.write(in.toImage());

    mBlockID.dataID(DataID::JPEG);

    mDataBuffer.clear();
    mDataBuffer << mImageData.size();
    mDataBuffer.write(mImageData.constData(), mImageData.size());

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
    mImageData.clear();
}

void cAxisCommunicationsSerializer::writeJPEG(const QImage& in)
{
    assert(mpDataFile);

    mImageWriter.setFormat("jpeg");
    mImageWriter.write(in);

    mBlockID.dataID(DataID::JPEG);

    mDataBuffer.clear();
    mDataBuffer << mImageData.size();
    mDataBuffer.write(mImageData.constData(), mImageData.size());

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
    mImageData.clear();
}

#endif


