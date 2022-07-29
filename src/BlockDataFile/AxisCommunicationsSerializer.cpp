
#include "AxisCommunicationsSerializer.hpp"
#include "AxisDataIdentifiers.hpp"
#include "BlockDataFile.hpp"
#include "../Sensors/RGB/AxisCommunicationsUtils.hpp"

#include <QBitmap>
#include <QImage>

#include <cassert>

using namespace axis;

cAxisCommunicationsSerializer::cAxisCommunicationsSerializer()
:
    cBlockSerializer(),
    mImageData(),
    mImageBuffer(&mImageData)
{
    mImageBuffer.open(QIODevice::ReadWrite);
    mImageWriter.setDevice(&mImageBuffer);

    auto list = QImageWriter::supportedImageFormats();
}

cAxisCommunicationsSerializer::cAxisCommunicationsSerializer(std::size_t n, cBlockDataFileWriter* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

cBlockID& cAxisCommunicationsSerializer::blockID()
{
	return mBlockID;
}

void cAxisCommunicationsSerializer::writeActiveCameraId(int in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::CAMERA_ID);

    mDataBuffer.clear();
    mDataBuffer << static_cast<int32_t>(in);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeActiveCameraId data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cAxisCommunicationsSerializer::writeFramesPerSecond(int frames_per_sec)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::FRAMES_PER_SECOND);

    mDataBuffer.clear();
    mDataBuffer << static_cast<int32_t>(frames_per_sec);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeFramesPerSecond data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cAxisCommunicationsSerializer::writeBitmap(const QBitmap& img)
{
    assert(mpDataFile);

    mImageData.clear();
    mImageBuffer.seek(0);

    mImageWriter.setFormat("bmp");
    mImageWriter.write(img.toImage());

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::BITMAP);

    mDataBuffer.clear();
    mDataBuffer << mImageData.size();
    mDataBuffer.write(mImageData.constData(), mImageData.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeBitmap data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mImageData.clear();
    mImageBuffer.seek(0);
}

void cAxisCommunicationsSerializer::writeJPEG(const QImage& img)
{
    assert(mpDataFile);

    mImageData.clear();
    mImageBuffer.seek(0);

    mImageWriter.setFormat("jpeg");
    if (!mImageWriter.write(img))
    {
        throw std::runtime_error(mImageWriter.errorString().toStdString());
    }

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::JPEG);

    mDataBuffer.clear();
    mDataBuffer << mImageData.size();
    mDataBuffer.write(mImageData.constData(), mImageData.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing writeJPEG data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());

    mImageData.clear();
    mImageBuffer.seek(0);
}

void cAxisCommunicationsSerializer::writeMpegFrame(const QImage& img)
{
    assert(mpDataFile);

    mImageData.clear();
    mImageBuffer.seek(0);

    mImageWriter.setFormat("jpeg");
    if (!mImageWriter.write(img))
    {
        mImageBuffer.seek(0);
        mImageData.clear();
        throw std::runtime_error(mImageWriter.errorString().toStdString());
    }

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::MPEG_FRAME);

    mDataBuffer.clear();
    mDataBuffer << mImageData.size();
    mDataBuffer.write(mImageData.constData(), mImageData.size());

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
    {
        std::string msg = "ERROR, Buffer Overrun in writing writeMpegFrame data.  ";
        msg += "Image size = ";
        msg += std::to_string(mImageData.size());
        throw std::runtime_error(msg);
    }

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
    mImageData.clear();
    mImageBuffer.seek(0);
}

void cAxisCommunicationsSerializer::write(const axis::sImageSize_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::RESOLUTION);

    mDataBuffer.clear();
    mDataBuffer << static_cast<uint16_t>(in.width);
    mDataBuffer << static_cast<uint16_t>(in.height);

    assert(!mDataBuffer.overrun());

    if (mDataBuffer.overrun())
        throw std::runtime_error("ERROR, Buffer Overrun in writing sImageSize_t data.");

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}



