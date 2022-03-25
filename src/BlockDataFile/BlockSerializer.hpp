/**
 * @file
 * @brief Serializer for storing SSNX GPS data into a block based data file
 */
#pragma once

#include "DataBuffer.hpp"

// Forward Declarations
class cBlockDataFile;


class cBlockSerializer
{
public:
	cBlockSerializer();
	explicit cBlockSerializer(std::size_t n, cBlockDataFile* pDataFile = nullptr);
	~cBlockSerializer() = default;

	void attach(cBlockDataFile* pDataFile);
	cBlockDataFile* detach();

	void setBufferCapacity(std::size_t n);

	explicit operator bool() const noexcept
	{
		return mpDataFile != nullptr;
	}

protected:
    cDataBuffer     mDataBuffer;
	cBlockDataFile* mpDataFile;
};

