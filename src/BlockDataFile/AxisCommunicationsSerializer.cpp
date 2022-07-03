
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
    mDataBuffer << in;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cAxisCommunicationsSerializer::writeBitmap(const QBitmap& in)
{
    assert(mpDataFile);

    mImageWriter.setFormat("bmp");
    mImageWriter.write(in.toImage());

    mBlockID.setVersion(1, 0);
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

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::JPEG);

    mDataBuffer.clear();
    mDataBuffer << mImageData.size();
    mDataBuffer.write(mImageData.constData(), mImageData.size());

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
    mImageData.clear();
}

void cAxisCommunicationsSerializer::write(const axis::sImageSize_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::RESOLUTION);

    mDataBuffer.clear();
    mDataBuffer << in.width;
    mDataBuffer << in.height;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}



