/**
 * @file
 * @brief Serializer for storing SSNX GPS data into a block based data file
 */
#pragma once

#include "BlockSerializer.hpp"
#include "ExperimentBlockId.hpp"

#include <ctime>

 // Forward Declarations


class cExperimentSerializer : public cBlockSerializer
{
public:
	cExperimentSerializer();
	explicit cExperimentSerializer(std::size_t n, cBlockDataFileWriter* pDataFile = nullptr);
	~cExperimentSerializer() = default;

	void startTime(time_t time);
	void endTime(time_t time);

protected:
	cBlockID& blockID() override;

private:
    cExperimentID         mBlockID;
};

