/**
 * @file
 * @brief Update function for simple point cloud visualizer
 */
#pragma once

#include "DataBuffer.hpp"
#include "SpidercamBlockId.hpp"

// Forward Declarations
class cBlockDataFileWriter;


class cSpidercamSerializer
{
public:
	cSpidercamSerializer();
	explicit cSpidercamSerializer(cBlockDataFileWriter* pDataFile);
	~cSpidercamSerializer() = default;

	void attach(cBlockDataFileWriter* pDataFile);
	cBlockDataFileWriter* detach();

	void setVersion(uint8_t major, uint8_t minor);
	void setBufferCapacity(std::size_t n);

private:
    cSpidercamID    mBlockID;
    cDataBuffer     mDataBuffer;
	cBlockDataFileWriter* mpDataFile;
};

