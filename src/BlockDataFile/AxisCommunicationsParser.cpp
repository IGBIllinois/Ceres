
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
    cBlockParser(),
    mImageData(),
    mImageBuffer(&mImageData)
{
    mImageBuffer.open(QIODevice::ReadWrite);
    mImageWriter.setDevice(&mImageBuffer);

    auto list = QImageWriter::supportedImageFormats();
}

cBlockID& cAxisCommunicationsParser::blockID()
{
	return mBlockID;
}

void cAxisCommunicationsParser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    switch (static_cast<axis::DataID>(data_id))
    {
    }
}

void cAxisCommunicationsParser::process_DataField(cDataBuffer& buffer)
{
    uint8_t u;
    buffer >> u;
    //    mPositionUnit = static_cast<pvt::ePOSTION_UNITS>(u);
}


#if 0
void cAxisCommunicationsSerializer::writeActiveCameraId(int in)
{
    assert(mpDataFile);

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

void cAxisCommunicationsSerializer::write(const axis::sImageSize_t& in)
{
    assert(mpDataFile);

    mBlockID.dataID(DataID::RESOLUTION);

    mDataBuffer.clear();
    mDataBuffer << in.width;
    mDataBuffer << in.height;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}
#endif


