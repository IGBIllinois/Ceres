/**
 * @file
 * @brief Parser for reading SSNX GPS data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "SsnxBlockId.hpp"

 // Forward Declarations
namespace ssnx
{
	namespace gps
	{
		struct PVT_Cartesian_1_t;
		struct PVT_Cartesian_2_t;
		struct PVT_Geodetic_1_t;
		struct PVT_Geodetic_2_t;
		struct PosCovGeodetic_1_t;
		struct VelCovGeodetic_1_t;
		struct DOP_1_t;
		struct PVT_Residuals_1_t;
		struct RAIMStatistics_1_t;
		struct POS_Projected_1_t;
		struct ReceiverTime_1_t;
		struct RtcmDatum_1_t;
	}
}

class cSsnxParser : public cBlockParser
{
public:
	cSsnxParser();
	~cSsnxParser() = default;

	cBlockID& blockID() override;

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	virtual void process_DataField(cDataBuffer& buffer);

/*
	void write(const ssnx::gps::PVT_Cartesian_1_t& in);
	void write(const ssnx::gps::PVT_Cartesian_2_t& in);
	void write(const ssnx::gps::PVT_Geodetic_1_t& in);
	void write(const ssnx::gps::PVT_Geodetic_2_t& in);
	void write(const ssnx::gps::PosCovGeodetic_1_t& in);
	void write(const ssnx::gps::VelCovGeodetic_1_t& in);
	void write(const ssnx::gps::DOP_1_t& in);
	void write(const ssnx::gps::PVT_Residuals_1_t& in);
	void write(const ssnx::gps::RAIMStatistics_1_t& in);
	void write(const ssnx::gps::POS_Projected_1_t& in);
	void write(const ssnx::gps::ReceiverTime_1_t& in);
	void write(const ssnx::gps::RtcmDatum_1_t& in);
*/


private:
    cSsnxID         mBlockID;
};

