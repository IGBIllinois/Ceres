
#include "SsnxSerializer.hpp"
#include "SsnxDataIdentifiers.hpp"
#include "../../BlockDataFile/BlockDataFile.hpp"

#include <ssnx/gps_data.hpp>
#include <ssnx/ssn_utils.hpp>

#include <optional>
#include <cassert>

using namespace ssnx;

cSsnxSerializer::cSsnxSerializer()
:
    cBlockSerializer()
{}

cSsnxSerializer::cSsnxSerializer(std::size_t n, cBlockDataFile* pDataFile)
:
    cBlockSerializer(n, pDataFile)
{
}

void cSsnxSerializer::write(const ssnx::gps::PVT_Cartesian_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::PVT_CARTESIAN);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << in.NrSV;
    mDataBuffer << in.Error;
    mDataBuffer << in.Mode;
    mDataBuffer << in.System;
    mDataBuffer << in.Info;
    mDataBuffer << in.SBASprn;
    mDataBuffer << in.X_m;
    mDataBuffer << in.Y_m;
    mDataBuffer << in.Z_m;
    mDataBuffer << in.Vx_mps;
    mDataBuffer << in.Vy_mps;
    mDataBuffer << in.Vz_mps;
    mDataBuffer << in.RxClkBias_ms;
    mDataBuffer << in.RxClkDrift_ppm;
    mDataBuffer << in.MeanCorrAge_s;
    mDataBuffer << in.BaseStationID;
    mDataBuffer << in.GroundTrack_deg;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::PVT_Cartesian_2_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(2, 0);
    mBlockID.dataID(DataID::PVT_CARTESIAN);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << to_int(in.Mode);
    mDataBuffer << in.HeightComputed;
    mDataBuffer << in.Error;
    mDataBuffer << in.X_m;
    mDataBuffer << in.Y_m;
    mDataBuffer << in.Z_m;
    mDataBuffer << in.Undulation_m;
    mDataBuffer << in.Vx_mps;
    mDataBuffer << in.Vy_mps;
    mDataBuffer << in.Vz_mps;
    mDataBuffer << in.GroundTrack_deg;
    mDataBuffer << in.RxClkBias_ms;
    mDataBuffer << in.RxClkDrift_ppm;
    mDataBuffer << to_int(in.TimeSystem);
    mDataBuffer << to_int(in.Datum);
    mDataBuffer << in.NrSV;
    mDataBuffer << in.SatClockCorrectionUsed;
    mDataBuffer << in.RangeCorrectionUsed;
    mDataBuffer << in.IonosphericInfoUsed;
    mDataBuffer << in.OrbitAccuracyInfoUsed;
    mDataBuffer << in.PrecisionApproachModeActive;
    mDataBuffer << in.ReferenceId;
    mDataBuffer << in.MeanCorrAge_s;
    mDataBuffer << in.SignalInfo;
    mDataBuffer << in.AlertFlag;

    /* Version 2.1 of this packet*/
    if (in.NrBases.has_value())
    {
        mBlockID.minorVersion(1);
        mDataBuffer << in.NrBases.value();
        mDataBuffer << in.AgeOfSeed_s.value();
        mDataBuffer << to_int(in.LastSeed.value());
    }

    /* Version 2.2 of this packet*/
    if (in.Latency_s.has_value())
    {
        mBlockID.minorVersion(2);
        mDataBuffer << in.Latency_s.value();
        mDataBuffer << in.HAccuracy_m.value();
        mDataBuffer << in.VAccuracy_m.value();
    }

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::PVT_Geodetic_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::PVT_GEODETIC);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << in.NrSV;
    mDataBuffer << in.Error;
    mDataBuffer << in.Mode;
    mDataBuffer << in.System;
    mDataBuffer << in.Info;
    mDataBuffer << in.SBASprn;
    mDataBuffer << in.Lat_rad;
    mDataBuffer << in.Lon_rad;
    mDataBuffer << in.Alt_m;
    mDataBuffer << in.Vn_mps;
    mDataBuffer << in.Ve_mps;
    mDataBuffer << in.Vu_mps;
    mDataBuffer << in.RxClkBias_ms;
    mDataBuffer << in.RxClkDrift_ppm;
    mDataBuffer << in.GeoidHeight_m;
    mDataBuffer << in.MeanCorrAge_s;
    mDataBuffer << in.BaseStationID;
    mDataBuffer << in.GroundTrack_deg;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::PVT_Geodetic_2_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(2, 0);
    mBlockID.dataID(DataID::PVT_GEODETIC);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << to_int(in.Mode);
    mDataBuffer << in.HeightComputed;
    mDataBuffer << in.Error;
    mDataBuffer << in.Lat_rad;
    mDataBuffer << in.Lon_rad;
    mDataBuffer << in.Height_m;
    mDataBuffer << in.Undulation_m;
    mDataBuffer << in.Vn_mps;
    mDataBuffer << in.Ve_mps;
    mDataBuffer << in.Vu_mps;
    mDataBuffer << in.GroundTrack_deg;
    mDataBuffer << in.RxClkBias_ms;
    mDataBuffer << in.RxClkDrift_ppm;
    mDataBuffer << to_int(in.TimeSystem);
    mDataBuffer << to_int(in.Datum);
    mDataBuffer << in.NrSV;
    mDataBuffer << in.SatClockCorrectionUsed;
    mDataBuffer << in.RangeCorrectionUsed;
    mDataBuffer << in.IonosphericInfoUsed;
    mDataBuffer << in.OrbitAccuracyInfoUsed;
    mDataBuffer << in.PrecisionApproachModeActive;
    mDataBuffer << in.ReferenceId;
    mDataBuffer << in.MeanCorrAge_s;
    mDataBuffer << in.SignalInfo;
    mDataBuffer << in.AlertFlag;

    /* Version 2.1 of this packet*/
    if (in.NrBases.has_value())
    {
        mBlockID.minorVersion(1);
        mDataBuffer << in.NrBases.value();
        mDataBuffer << in.AgeOfSeed_s.value();
        mDataBuffer << to_int(in.LastSeed.value());
    }

    /* Version 2.2 of this packet*/
    if (in.Latency_s.has_value())
    {
        mBlockID.minorVersion(2);
        mDataBuffer << in.Latency_s.value();
        mDataBuffer << in.HAccuracy_m.value();
        mDataBuffer << in.VAccuracy_m.value();
    }

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::PosCovGeodetic_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::POS_COV_GEODETIC);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << to_int(in.Mode);
    mDataBuffer << in.HeightComputed;
    mDataBuffer << in.Error;
    mDataBuffer << in.Cov_latlat_m;
    mDataBuffer << in.Cov_lonlon_m;
    mDataBuffer << in.Cov_hgthgt_m;
    mDataBuffer << in.Cov_bb_m;
    mDataBuffer << in.Cov_latlon_m;
    mDataBuffer << in.Cov_lathgt_m;
    mDataBuffer << in.Cov_latb_m;
    mDataBuffer << in.Cov_lonhgt_m;
    mDataBuffer << in.Cov_lonb_m;
    mDataBuffer << in.Cov_hgtb_m;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::VelCovGeodetic_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::VEL_COV_GEODETIC);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << to_int(in.Mode);
    mDataBuffer << in.HeightComputed;
    mDataBuffer << in.Error;
    mDataBuffer << in.Cov_VnVn_mps;
    mDataBuffer << in.Cov_VeVe_mps;
    mDataBuffer << in.Cov_VuVu_mps;
    mDataBuffer << in.Cov_DtDt_mps;
    mDataBuffer << in.Cov_VnVe_mps;
    mDataBuffer << in.Cov_VnVu_mps;
    mDataBuffer << in.Cov_VnDt_mps;
    mDataBuffer << in.Cov_VeVu_mps;
    mDataBuffer << in.Cov_VeDt_mps;
    mDataBuffer << in.Cov_VuDt_mps;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::DOP_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::DOP);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << in.NrSV;
    mDataBuffer << in.PDOP;
    mDataBuffer << in.TDOP;
    mDataBuffer << in.HDOP;
    mDataBuffer << in.VDOP;
    mDataBuffer << in.HPL_m;
    mDataBuffer << in.VPL_m;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::PVT_Residuals_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::PVT_RESIDUALS);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << in.N;
    mDataBuffer << in.SBSize;
/*
 	struct PVT_Residual_t
	{
		int16_t        CACodeRes;
		int16_t        P1CodeRes;
		int16_t        P2CodeRes;
		int16_t        DopplerL1Res;
		int16_t        DopplerL2Res;
		uint8_t        PRN;
    }

    PVT_Residual_t SatResidual[72];
*/

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::RAIMStatistics_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::RAIM_STATISTICS);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << in.Integrity;
    mDataBuffer << in.PositionHERL;
    mDataBuffer << in.PositionVERL;
    mDataBuffer << in.VelocityHERL;
    mDataBuffer << in.VelocityVERL;
    mDataBuffer << in.UnityOverallModelP;
    mDataBuffer << in.UnityOverallModelV;
    mDataBuffer << in.N;
    mDataBuffer << in.SBSize;
/*
    struct RAIMSatData_t
    {
        uint8_t        PRN;
        uint8_t        AntennaID;
        uint8_t        TestResults;
        uint16_t       UnityRangeW;
        uint16_t       UnityRrateW;
        uint16_t       RangeMDB;
        uint16_t       RrateMDB;
    };

    RAIMSatData_t  RAIMChannel[72];
*/

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::POS_Projected_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::POS_PROJECTED);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << to_int(in.Mode);
    mDataBuffer << in.HeightComputed;
    mDataBuffer << in.Error;
    mDataBuffer << in.Northing_m;
    mDataBuffer << in.Easting_m;
    mDataBuffer << in.Alt_m;
    mDataBuffer << in.Datum;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::ReceiverTime_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::RECEIVER_TIME);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << in.utcYear;
    mDataBuffer << in.utcMonth;
    mDataBuffer << in.utcDay;
    mDataBuffer << in.utcHour;
    mDataBuffer << in.utcMinute;
    mDataBuffer << in.utcSecond;
    mDataBuffer << in.deltaLS;
    mDataBuffer << in.WeekNumberValid;
    mDataBuffer << in.TimeOfWeekWithin20ms;
    mDataBuffer << in.TimeOfWeekWithinThreshold;
    mDataBuffer << in.TimeFromPTTI;
    mDataBuffer << in.TimeFromNTP;

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}

void cSsnxSerializer::write(const ssnx::gps::RtcmDatum_1_t& in)
{
    assert(mpDataFile);

    mBlockID.setVersion(1, 0);
    mBlockID.dataID(DataID::RTCM_DATUM);

    mDataBuffer.clear();
    mDataBuffer << in.dataValid;
    mDataBuffer << in.timestamp_s;
    mDataBuffer << in.sourceCRS;
    mDataBuffer << in.targetCRS;
    mDataBuffer << in.datum;
    mDataBuffer << to_int(in.heightType);
    mDataBuffer << to_int(in.horizontalQualityInd);
    mDataBuffer << to_int(in.verticalQualityInd);

    mpDataFile->writeBlock(mBlockID, mDataBuffer.data(), mDataBuffer.size());
}




