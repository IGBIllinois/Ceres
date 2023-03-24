/**
 * @file
 * @brief Parser for reading Axis Communications image data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "AxisCommunicationsBlockId.hpp"
#include "ImageBuffers.hpp"

//#include <QImageWriter>
//#include <QBuffer>
//#include <QByteArray>

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
//class QBitmap;
//class QImage;
//QT_END_NAMESPACE


namespace rgb
{
	enum class eIMAGE_FORMAT;
	struct sImageSize_t;
}

class cAxisCommunicationsParser : public cBlockParser
{
public:
	cAxisCommunicationsParser();
	~cAxisCommunicationsParser() = default;

	cBlockID& blockID() override;

	virtual void onActiveCameraId(int id) = 0;
	virtual void onFramesPerSecond(int frames_per_sec) = 0;
	virtual void onBitmap(const cBitmapBuffer& buffer) = 0;
	virtual void onJPEG(const cJpegBuffer& buffer) = 0;
	virtual void onMpegFrame(const cMpegFrameBuffer& buffer) = 0;
	virtual void onImageSize(const rgb::sImageSize_t& image_size) = 0;

protected:
	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	void processActiveCameraId(cDataBuffer& buffer);
	void processFramesPerSecond(cDataBuffer& buffer);
	void processBitmap(cDataBuffer& buffer);
	void processJPEG(cDataBuffer& buffer);
	void processMpegFrame(cDataBuffer& buffer);
	void processImageSize(cDataBuffer& buffer);

private:
	cBitmapBuffer	 mBitmapBuffer;
	cJpegBuffer		 mJpegBuffer;
	cMpegFrameBuffer mMpegFrameBuffer;

//	QByteArray		mImageData;
//	QBuffer			mImageBuffer;
//	QImageWriter	mImageWriter;

	cAxisCommunicationsID    mBlockID;
};

