/**
 * @file
 * @brief Parser for reading Spidercam data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "SpidercamBlockId.hpp"

// Spidercam Data Types
namespace spidercam
{
	struct sPosition_1_t
	{
		double X_mm;
		double Y_mm;
		double Z_mm;
		double height_mm;
		uint32_t speed_mmps;
		float pan_deg;
		float pan_speed_dps;
		float tilt_deg;
		float tilt_speed_dps;
		float roll_gimbal_deg;
		uint32_t zoom;
		uint32_t focus;
		uint32_t iris;
		float pitch_deg;
		float roll_deg;
		uint64_t timestamp;

		sPosition_1_t()
			: X_mm(0), Y_mm(0), Z_mm(0), height_mm(0), speed_mmps(0),
			pan_deg(0), pan_speed_dps(0), tilt_deg(0), tilt_speed_dps(0), roll_gimbal_deg(0),
			zoom(0), focus(0), iris(0), pitch_deg(0), roll_deg(0), timestamp(0)
		{}
	};
}


class cSpidercamParser : public cBlockParser
{
public:
	cSpidercamParser();
	~cSpidercamParser() = default;

	cBlockID& blockID() override;

protected:
	virtual void onPosition(const spidercam::sPosition_1_t& pos) = 0;

private:
	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void processPosition(cDataBuffer& buffer);

private:
    cSpidercamID    mBlockID;
};

