/**
 * @file
 * @brief Serializer for storing data into a block based data file
 */
#pragma once

#include "DataBuffer.hpp"

// Forward Declarations
class cBlockDataFileWriter;
class cBlockID;


class cBlockSerializer
{
public:
	cBlockSerializer();
	explicit cBlockSerializer(std::size_t n, cBlockDataFileWriter* pDataFile = nullptr);
	~cBlockSerializer() = default;

	void attach(cBlockDataFileWriter* pDataFile);
	cBlockDataFileWriter* detach();

	void setBufferCapacity(std::size_t n);

	void setVersion(uint8_t major, uint8_t minor);

	explicit operator bool() const noexcept
	{
		return mpDataFile != nullptr;
	}

protected:
	virtual cBlockID& blockID() = 0;

protected:
    cDataBuffer     mDataBuffer;
	cBlockDataFileWriter* mpDataFile;
};

