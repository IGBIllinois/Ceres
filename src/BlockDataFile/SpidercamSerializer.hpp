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
	struct sPosition_1_t;
}

class cSpidercamSerializer : public cBlockSerializer
{
public:
	cSpidercamSerializer();
	explicit cSpidercamSerializer(std::size_t n, cBlockDataFileWriter* pDataFile = nullptr);
	~cSpidercamSerializer() = default;

	/**
	 * Saves the current position information.  The information is stored at
	 * regular intervals when an experiment is running.
	 */
	void write(const spidercam::sPosition_1_t& pos);

	/**
	 * Saves the dolly information.  The information is stored at
	 * the beginning of the "save phase" experiment.
	 */
	void write(const double x_mm, const double y_mm, const double z_mm,
        const double vx_mmps, const double vy_mmps, const double vz_mmps,
        const double yaw_deg, const double pitch_deg, const double roll_deg);


protected:
	cBlockID& blockID() override;

private:
    cSpidercamID    mBlockID;
};

