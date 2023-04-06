/**
 * @file
 * @brief Parser for reading SSNX GPS data from a block based data file
 */
#pragma once

#include "BlockParser.hpp"
#include "SsnxBlockId.hpp"

#include <ssnx/gps_data.hpp>


class cSsnxParser : public cBlockParser
{
public:
	cSsnxParser();
	~cSsnxParser() = default;

	cBlockID& blockID() override;

	ssnx::gps::PVT_Cartesian_1_t	getPVT_Cartesian_1() { return mPVT_Cartesian_1; };
	ssnx::gps::PVT_Cartesian_2_t	getPVT_Cartesian_2() { return mPVT_Cartesian_2; };
	ssnx::gps::PVT_Geodetic_1_t		getPVT_Geodetic_1() { return mPVT_Geodetic_1; };
	ssnx::gps::PVT_Geodetic_2_t		getPVT_Geodetic_2() { return mPVT_Geodetic_2; };
	ssnx::gps::PosCovGeodetic_1_t	getPosCovGeodetic_1() { return mPosCovGeodetic_1; };
	ssnx::gps::VelCovGeodetic_1_t	getVelCovGeodetic_1() { return mVelCovGeodetic_1; };
	ssnx::gps::DOP_1_t				getDOP_1() { return mDOP_1; };
	ssnx::gps::PVT_Residuals_1_t	getPVT_Residuals_1() { return mPVT_Residuals_1; };
	ssnx::gps::RAIMStatistics_1_t	getRAIMStatistics_1() { return mRAIMStatistics_1; };
	ssnx::gps::POS_Local_1_t		getPOS_Local_1() { return mPOS_Local_1; };
	ssnx::gps::POS_Projected_1_t	getPOS_Projected_1() { return mPOS_Projected_1; };
	ssnx::gps::ReceiverTime_1_t		getReceiverTime_1() { return mReceiverTime_1; };
	ssnx::gps::RtcmDatum_1_t		getRtcmDatum_1() { return mRtcmDatum_1; };

	void processData(BLOCK_MAJOR_VERSION_t major_version,
		BLOCK_MINOR_VERSION_t minor_version,
		BLOCK_DATA_ID_t data_id,
		cDataBuffer& buffer) override;

protected:
	virtual void processPVT_Cartesian_1_t(cDataBuffer& buffer);
	virtual void processPVT_Cartesian_2_t(cDataBuffer& buffer);
	virtual void processPVT_Geodetic_1_t(cDataBuffer& buffer);
	virtual void processPVT_Geodetic_2_t(cDataBuffer& buffer);
	virtual void processPosCovGeodetic_1_t(cDataBuffer& buffer);
	virtual void processVelCovGeodetic_1_t(cDataBuffer& buffer);
	virtual void processDOP_1_t(cDataBuffer& buffer);
	virtual void processPVT_Residuals_1_t(cDataBuffer& buffer);
	virtual void processRAIMStatistics_1_t(cDataBuffer& buffer);
	virtual void processPOS_Local_1_t(cDataBuffer& buffer);
	virtual void processPOS_Projected_1_t(cDataBuffer& buffer);
	virtual void processReceiverTime_1_t(cDataBuffer& buffer);
	virtual void processRtcmDatum_1_t(cDataBuffer& buffer);

protected:
	ssnx::gps::PVT_Cartesian_1_t	mPVT_Cartesian_1;
	ssnx::gps::PVT_Cartesian_2_t	mPVT_Cartesian_2;
	ssnx::gps::PVT_Geodetic_1_t		mPVT_Geodetic_1;
	ssnx::gps::PVT_Geodetic_2_t		mPVT_Geodetic_2;
	ssnx::gps::PosCovGeodetic_1_t	mPosCovGeodetic_1;
	ssnx::gps::VelCovGeodetic_1_t	mVelCovGeodetic_1;
	ssnx::gps::DOP_1_t				mDOP_1;
	ssnx::gps::PVT_Residuals_1_t	mPVT_Residuals_1;
	ssnx::gps::RAIMStatistics_1_t	mRAIMStatistics_1;
	ssnx::gps::POS_Local_1_t		mPOS_Local_1;
	ssnx::gps::POS_Projected_1_t	mPOS_Projected_1;
	ssnx::gps::ReceiverTime_1_t		mReceiverTime_1;
	ssnx::gps::RtcmDatum_1_t		mRtcmDatum_1;

private:
    cSsnxID         mBlockID;
};

