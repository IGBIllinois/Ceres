/**
 * @file
 * @brief Parser for reading SSNX GPS data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "SsnxBlockId.hpp"
#include "SsnxTypes.hpp"

class cSsnxParser : public cBlockParser
{
public:
	cSsnxParser();
	~cSsnxParser() = default;

	cBlockID& blockID() override;

protected:
	virtual void onNewData(const ssnx::PVT_Cartesian_1_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Cartesian_2_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Cartesian_2_1_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Cartesian_2_2_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Geodetic_1_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Geodetic_2_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Geodetic_2_1_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Geodetic_2_2_t& data) = 0;
	virtual void onNewData(const ssnx::PosCovGeodetic_1_t& data) = 0;
	virtual void onNewData(const ssnx::VelCovGeodetic_1_t& data) = 0;
	virtual void onNewData(const ssnx::DOP_1_t& data) = 0;
	virtual void onNewData(const ssnx::PVT_Residuals_1_t& data) = 0;
	virtual void onNewData(const ssnx::RAIMStatistics_1_t& data) = 0;
	virtual void onNewData(const ssnx::POS_Projected_1_t& data) = 0;
	virtual void onNewData(const ssnx::ReceiverTime_1_t& data) = 0;
	virtual void onNewData(const ssnx::RtcmDatum_1_t& data) = 0;

private:
	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

	void processPVT_Cartesian_1_t(cDataBuffer& buffer);
	void processPVT_Cartesian_2_t(cDataBuffer& buffer);
	void processPVT_Cartesian_2_1_t(cDataBuffer& buffer);
	void processPVT_Cartesian_2_2_t(cDataBuffer& buffer);
	void processPVT_Geodetic_1_t(cDataBuffer& buffer);
	void processPVT_Geodetic_2_t(cDataBuffer& buffer);
	void processPVT_Geodetic_2_1_t(cDataBuffer& buffer);
	void processPVT_Geodetic_2_2_t(cDataBuffer& buffer);
	void processPosCovGeodetic_1_t(cDataBuffer& buffer);
	void processVelCovGeodetic_1_t(cDataBuffer& buffer);
	void processDOP_1_t(cDataBuffer& buffer);
	void processPVT_Residuals_1_t(cDataBuffer& buffer);
	void processRAIMStatistics_1_t(cDataBuffer& buffer);
	void processPOS_Projected_1_t(cDataBuffer& buffer);
	void processReceiverTime_1_t(cDataBuffer& buffer);
	void processRtcmDatum_1_t(cDataBuffer& buffer);

private:
    cSsnxID         mBlockID;
};

