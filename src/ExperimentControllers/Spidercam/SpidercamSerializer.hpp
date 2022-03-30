/**
 * @file
 * @brief Update function for simple point cloud visualizer
 */
#pragma once

#include "../../BlockDataFile/DataBuffer.hpp"
#include "SpidercamBlockId.hpp"

// Forward Declarations
class cBlockDataFile;


class cSpidercamSerializer
{
public:
	cSpidercamSerializer();
	explicit cSpidercamSerializer(cBlockDataFile* pDataFile);
	~cSpidercamSerializer() = default;

	void attach(cBlockDataFile* pDataFile);
	cBlockDataFile* detach();

	void setVersion(uint8_t major, uint8_t minor);
	void setBufferCapacity(std::size_t n);

private:
    cSpidercamID    mBlockID;
    cDataBuffer     mDataBuffer;
	cBlockDataFile* mpDataFile;
};

