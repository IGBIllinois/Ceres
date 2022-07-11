/**
 * @file
 * @brief Serializer for storing Spidercam data into a block based data file
 */
#pragma once

#include "BlockSerializer.hpp"
#include "SpidercamBlockId.hpp"

// Forward Declarations
namespace spidercam
{
	struct sPosition;
}

class cSpidercamSerializer : public cBlockSerializer
{
public:
	cSpidercamSerializer();
	explicit cSpidercamSerializer(std::size_t n, cBlockDataFileWriter* pDataFile = nullptr);
	~cSpidercamSerializer() = default;

	void write(const spidercam::sPosition& pos);

protected:
	cBlockID& blockID() override;

private:
    cSpidercamID    mBlockID;
};

