/**
 * @file
 * @brief Parser for reading Axis Communications image data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
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

class cAxisCommunicationsParser : public cBlockParser
{
public:
	cAxisCommunicationsParser();
	~cAxisCommunicationsParser() = default;

	cBlockID& blockID() override;

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void process_DataField(cDataBuffer& buffer);

private:
	QByteArray		mImageData;
	QBuffer			mImageBuffer;
	QImageWriter	mImageWriter;

	cAxisCommunicationsID    mBlockID;
};

