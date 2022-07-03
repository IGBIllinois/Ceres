/**
 * @file
 * @brief Serializer for storing Spidercam data into a block based data file
 */
#pragma once

#include "BlockSerializer.hpp"
#include "SpidercamBlockId.hpp"

// Forward Declarations


class cSpidercamSerializer : public cBlockSerializer
{
public:
	cSpidercamSerializer();
	explicit cSpidercamSerializer(std::size_t n, cBlockDataFileWriter* pDataFile = nullptr);
	~cSpidercamSerializer() = default;

protected:
	cBlockID& blockID() override;

private:
    cSpidercamID    mBlockID;
};

