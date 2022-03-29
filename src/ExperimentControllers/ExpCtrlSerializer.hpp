/**
 * @file
 * @brief Serializer for storing SSNX GPS data into a block based data file
 */
#pragma once

#include "../../BlockDataFile/BlockSerializer.hpp"
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

class cSsnxSerializer : public cBlockSerializer
{
public:
	cSsnxSerializer();
	explicit cSsnxSerializer(std::size_t n, cBlockDataFile* pDataFile = nullptr);
	~cSsnxSerializer() = default;

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

private:
    cSsnxID         mBlockID;
};

