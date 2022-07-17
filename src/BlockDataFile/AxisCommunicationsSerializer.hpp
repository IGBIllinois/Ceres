/**
 * @file
 * @brief Serializer for storing Axis Communications image data into a block based data file
 */
#pragma once

#include "BlockSerializer.hpp"
#include "AxisCommunicationsBlockId.hpp"

#include <QImageWriter>
#include <QBuffer>
#include <QByteArray>

 // Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QBitmap;
class QImage;
QT_END_NAMESPACE

namespace axis
{
	enum class eIMAGE_FORMAT;
	struct sImageSize_t;
}

class cAxisCommunicationsSerializer : public cBlockSerializer
{
public:
	cAxisCommunicationsSerializer();
	explicit cAxisCommunicationsSerializer(std::size_t n, cBlockDataFileWriter* pDataFile);
	~cAxisCommunicationsSerializer() = default;

	void writeActiveCameraId(int in);
	void writeFramesPerSecond(int frames_per_sec);
	void writeBitmap(const QBitmap& in);
	void writeJPEG(const QImage& in);
	void writeMpegFrame(const QImage& in);

	void write(const axis::sImageSize_t&  in);

protected:
	cBlockID& blockID() override;

private:
	QByteArray		mImageData;
	QBuffer			mImageBuffer;
	QImageWriter	mImageWriter;

	cAxisCommunicationsID    mBlockID;
};

