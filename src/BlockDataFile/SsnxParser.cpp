
#include "SsnxParser.hpp"
#include "SsnxDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <ssnx/ssn_utils.hpp>

#include <optional>
#include <cassert>

using namespace ssnx;

namespace
{
    gps::eSolutionType to_solution_type(cDataBuffer& buffer)
    {
        uint8_t data = 0;
        buffer >> data;
        return static_cast<gps::eSolutionType>(data);
    };

    gps::eTimeSystem to_time_system(cDataBuffer& buffer)
    {
        uint8_t data = 0;
        buffer >> data;
        return static_cast<gps::eTimeSystem>(data);
    };

    gps::eDatum to_datum(cDataBuffer& buffer)
    {
        uint8_t data = 0;
        buffer >> data;
        return static_cast<gps::eDatum>(data);
    };

    gps::ePPP_LastSeed to_last_seed(cDataBuffer& buffer)
    {
        uint16_t data = 0;
        buffer >> data;
        return static_cast<gps::ePPP_LastSeed>(data);
    };

    gps::eHeightType to_height_type(cDataBuffer& buffer)
    {
        uint8_t data = 0;
        buffer >> data;
        return static_cast<gps::eHeightType>(data);
    };

    gps::eQualityIndicator to_quality_indicator(cDataBuffer& buffer)
    {
        uint8_t data = 0;
        buffer >> data;
        return static_cast<gps::eQualityIndicator>(data);
    };
}

cSsnxParser::cSsnxParser()
:
    cBlockParser()
{}

cBlockID& cSsnxParser::blockID()
{
    return mBlockID;
}

void cSsnxParser::processData(BLOCK_MAJOR_VERSION_t major_version,
    BLOCK_MINOR_VERSION_t minor_version,
    BLOCK_DATA_ID_t data_id,
    cDataBuffer& buffer)
{
    blockID().setVersion(major_version, minor_version);
    blockID().dataID(data_id);

    switch (static_cast<ssnx::DataID>(data_id))
    {
    case DataID::PVT_CARTESIAN:
        if (major_version == 1)
            processPVT_Cartesian_1_t(buffer);
        else
            processPVT_Cartesian_2_t(buffer);
        break;
    case DataID::PVT_GEODETIC:
        if (major_version == 1)
            processPVT_Geodetic_1_t(buffer);
        else
            processPVT_Geodetic_2_t(buffer);
        break;
    case DataID::POS_COV_GEODETIC:
        processPosCovGeodetic_1_t(buffer);
        break;
    case DataID::VEL_COV_GEODETIC:
        processVelCovGeodetic_1_t(buffer);
        break;
    case DataID::DOP:
        processDOP_1_t(buffer);
        break;
    case DataID::PVT_RESIDUALS:
        processPVT_Residuals_1_t(buffer);
        break;
    case DataID::RAIM_STATISTICS:
        processRAIMStatistics_1_t(buffer);
        break;
    case DataID::POS_LOCAL:
        processPOS_Local_1_t(buffer);
        break;
    case DataID::POS_PROJECTED:
        processPOS_Projected_1_t(buffer);
        break;
    case DataID::RECEIVER_TIME:
        processReceiverTime_1_t(buffer);
        break;
    case DataID::RTCM_DATUM:
        processRtcmDatum_1_t(buffer);
        break;
    }
}

void cSsnxParser::processPVT_Cartesian_1_t(cDataBuffer& buffer)
{
    buffer >> mPVT_Cartesian_1.dataValid;
    buffer >> mPVT_Cartesian_1.timestamp_s;
    buffer >> mPVT_Cartesian_1.NrSV;
    buffer >> mPVT_Cartesian_1.Error;
    buffer >> mPVT_Cartesian_1.Mode;
    buffer >> mPVT_Cartesian_1.System;
    buffer >> mPVT_Cartesian_1.Info;
    buffer >> mPVT_Cartesian_1.SBASprn;
    buffer >> mPVT_Cartesian_1.X_m;
    buffer >> mPVT_Cartesian_1.Y_m;
    buffer >> mPVT_Cartesian_1.Z_m;
    buffer >> mPVT_Cartesian_1.Vx_mps;
    buffer >> mPVT_Cartesian_1.Vy_mps;
    buffer >> mPVT_Cartesian_1.Vz_mps;
    buffer >> mPVT_Cartesian_1.RxClkBias_ms;
    buffer >> mPVT_Cartesian_1.RxClkDrift_ppm;
    buffer >> mPVT_Cartesian_1.MeanCorrAge_s;
    buffer >> mPVT_Cartesian_1.BaseStationID;
    buffer >> mPVT_Cartesian_1.GroundTrack_deg;
}

void cSsnxParser::processPVT_Cartesian_2_t(cDataBuffer& buffer)
{
    buffer >> mPVT_Cartesian_2.dataValid;
    buffer >> mPVT_Cartesian_2.timestamp_s;
    mPVT_Cartesian_2.Mode = to_solution_type(buffer);
    buffer >> mPVT_Cartesian_2.HeightComputed;
    buffer >> mPVT_Cartesian_2.Error;
    buffer >> mPVT_Cartesian_2.X_m;
    buffer >> mPVT_Cartesian_2.Y_m;
    buffer >> mPVT_Cartesian_2.Z_m;
    buffer >> mPVT_Cartesian_2.Undulation_m;
    buffer >> mPVT_Cartesian_2.Vx_mps;
    buffer >> mPVT_Cartesian_2.Vy_mps;
    buffer >> mPVT_Cartesian_2.Vz_mps;
    buffer >> mPVT_Cartesian_2.GroundTrack_deg;
    buffer >> mPVT_Cartesian_2.RxClkBias_ms;
    buffer >> mPVT_Cartesian_2.RxClkDrift_ppm;
    mPVT_Cartesian_2.TimeSystem = to_time_system(buffer);
    mPVT_Cartesian_2.Datum = to_datum(buffer);
    buffer >> mPVT_Cartesian_2.NrSV;
    buffer >> mPVT_Cartesian_2.SatClockCorrectionUsed;
    buffer >> mPVT_Cartesian_2.RangeCorrectionUsed;
    buffer >> mPVT_Cartesian_2.IonosphericInfoUsed;
    buffer >> mPVT_Cartesian_2.OrbitAccuracyInfoUsed;
    buffer >> mPVT_Cartesian_2.PrecisionApproachModeActive;
    buffer >> mPVT_Cartesian_2.ReferenceId;
    buffer >> mPVT_Cartesian_2.MeanCorrAge_s;
    buffer >> mPVT_Cartesian_2.SignalInfo;
    buffer >> mPVT_Cartesian_2.AlertFlag;

    /* Version 2.1 of this packet*/
    if (mBlockID.minorVersion() > 0)
    {
        mPVT_Cartesian_2.NrBases = buffer.get<uint8_t>();
        mPVT_Cartesian_2.AgeOfSeed_s = buffer.get<uint16_t>();
        mPVT_Cartesian_2.LastSeed = to_last_seed(buffer);
    }

    /* Version 2.2 of this packet*/
    if (mBlockID.minorVersion() > 1)
    {
        mPVT_Cartesian_2.Latency_s = buffer.get<float>();
        mPVT_Cartesian_2.HAccuracy_m = buffer.get<float>();
        mPVT_Cartesian_2.VAccuracy_m = buffer.get<float>();
        mPVT_Cartesian_2.InRtkMode = buffer.get<bool>();
    }
}

void cSsnxParser::processPVT_Geodetic_1_t(cDataBuffer& buffer)
{
    buffer >> mPVT_Geodetic_1.dataValid;
    buffer >> mPVT_Geodetic_1.timestamp_s;
    buffer >> mPVT_Geodetic_1.NrSV;
    buffer >> mPVT_Geodetic_1.Error;
    buffer >> mPVT_Geodetic_1.Mode;
    buffer >> mPVT_Geodetic_1.System;
    buffer >> mPVT_Geodetic_1.Info;
    buffer >> mPVT_Geodetic_1.SBASprn;
    buffer >> mPVT_Geodetic_1.Lat_rad;
    buffer >> mPVT_Geodetic_1.Lon_rad;
    buffer >> mPVT_Geodetic_1.Alt_m;
    buffer >> mPVT_Geodetic_1.Vn_mps;
    buffer >> mPVT_Geodetic_1.Ve_mps;
    buffer >> mPVT_Geodetic_1.Vu_mps;
    buffer >> mPVT_Geodetic_1.RxClkBias_ms;
    buffer >> mPVT_Geodetic_1.RxClkDrift_ppm;
    buffer >> mPVT_Geodetic_1.GeoidHeight_m;
    buffer >> mPVT_Geodetic_1.MeanCorrAge_s;
    buffer >> mPVT_Geodetic_1.BaseStationID;
    buffer >> mPVT_Geodetic_1.GroundTrack_deg;
}

void cSsnxParser::processPVT_Geodetic_2_t(cDataBuffer& buffer)
{
    mPVT_Geodetic_2;

    buffer >> mPVT_Geodetic_2.dataValid;
    buffer >> mPVT_Geodetic_2.timestamp_s;
    mPVT_Geodetic_2.Mode = to_solution_type(buffer);
    buffer >> mPVT_Geodetic_2.HeightComputed;
    buffer >> mPVT_Geodetic_2.Error;
    buffer >> mPVT_Geodetic_2.Lat_rad;
    buffer >> mPVT_Geodetic_2.Lon_rad;
    buffer >> mPVT_Geodetic_2.Height_m;
    buffer >> mPVT_Geodetic_2.Undulation_m;
    buffer >> mPVT_Geodetic_2.Vn_mps;
    buffer >> mPVT_Geodetic_2.Ve_mps;
    buffer >> mPVT_Geodetic_2.Vu_mps;
    buffer >> mPVT_Geodetic_2.GroundTrack_deg;
    buffer >> mPVT_Geodetic_2.RxClkBias_ms;
    buffer >> mPVT_Geodetic_2.RxClkDrift_ppm;
    mPVT_Geodetic_2.TimeSystem = to_time_system(buffer);
    mPVT_Geodetic_2.Datum = to_datum(buffer);
    buffer >> mPVT_Geodetic_2.NrSV;
    buffer >> mPVT_Geodetic_2.SatClockCorrectionUsed;
    buffer >> mPVT_Geodetic_2.RangeCorrectionUsed;
    buffer >> mPVT_Geodetic_2.IonosphericInfoUsed;
    buffer >> mPVT_Geodetic_2.OrbitAccuracyInfoUsed;
    buffer >> mPVT_Geodetic_2.PrecisionApproachModeActive;
    buffer >> mPVT_Geodetic_2.ReferenceId;
    buffer >> mPVT_Geodetic_2.MeanCorrAge_s;
    buffer >> mPVT_Geodetic_2.SignalInfo;
    buffer >> mPVT_Geodetic_2.AlertFlag;

    /* Version 2.1 of this packet*/
    if (mBlockID.minorVersion() > 0)
    {
        mPVT_Geodetic_2.NrBases = buffer.get<uint8_t>();
        mPVT_Geodetic_2.AgeOfSeed_s = buffer.get<uint16_t>();
        mPVT_Geodetic_2.LastSeed = to_last_seed(buffer);
    }

    /* Version 2.2 of this packet*/
    if (mBlockID.minorVersion() > 1)
    {
        mPVT_Geodetic_2.Latency_s = buffer.get<float>();
        mPVT_Geodetic_2.HAccuracy_m = buffer.get<float>();
        mPVT_Geodetic_2.VAccuracy_m = buffer.get<float>();
    }
}

void cSsnxParser::processPosCovGeodetic_1_t(cDataBuffer& buffer)
{
    buffer >> mPosCovGeodetic_1.dataValid;
    buffer >> mPosCovGeodetic_1.timestamp_s;
    mPosCovGeodetic_1.Mode = to_solution_type(buffer);
    buffer >> mPosCovGeodetic_1.HeightComputed;
    buffer >> mPosCovGeodetic_1.Error;
    buffer >> mPosCovGeodetic_1.Cov_latlat_m2;
    buffer >> mPosCovGeodetic_1.Cov_lonlon_m2;
    buffer >> mPosCovGeodetic_1.Cov_hgthgt_m2;
    buffer >> mPosCovGeodetic_1.Cov_bb_m2;
    buffer >> mPosCovGeodetic_1.Cov_latlon_m2;
    buffer >> mPosCovGeodetic_1.Cov_lathgt_m2;
    buffer >> mPosCovGeodetic_1.Cov_latb_m2;
    buffer >> mPosCovGeodetic_1.Cov_lonhgt_m2;
    buffer >> mPosCovGeodetic_1.Cov_lonb_m2;
    buffer >> mPosCovGeodetic_1.Cov_hgtb_m2;
}

void cSsnxParser::processVelCovGeodetic_1_t(cDataBuffer& buffer)
{
    buffer >> mVelCovGeodetic_1.dataValid;
    buffer >> mVelCovGeodetic_1.timestamp_s;
    mVelCovGeodetic_1.Mode = to_solution_type(buffer);
    buffer >> mVelCovGeodetic_1.HeightComputed;
    buffer >> mVelCovGeodetic_1.Error;
    buffer >> mVelCovGeodetic_1.Cov_VnVn_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VeVe_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VuVu_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_DtDt_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VnVe_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VnVu_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VnDt_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VeVu_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VeDt_m2ps2;
    buffer >> mVelCovGeodetic_1.Cov_VuDt_m2ps2;
}

void cSsnxParser::processDOP_1_t(cDataBuffer& buffer)
{
    buffer >> mDOP_1.dataValid;
    buffer >> mDOP_1.timestamp_s;
    buffer >> mDOP_1.NrSV;
    buffer >> mDOP_1.PDOP;
    buffer >> mDOP_1.TDOP;
    buffer >> mDOP_1.HDOP;
    buffer >> mDOP_1.VDOP;
    buffer >> mDOP_1.HPL_m;
    buffer >> mDOP_1.VPL_m;
}

void cSsnxParser::processPVT_Residuals_1_t(cDataBuffer& buffer)
{
    buffer >> mPVT_Residuals_1.dataValid;
    buffer >> mPVT_Residuals_1.timestamp_s;
    buffer >> mPVT_Residuals_1.N;
    buffer >> mPVT_Residuals_1.SBSize;

    for (auto& satResidual : mPVT_Residuals_1.SatResidual)
    {
        buffer >> satResidual.PRN;
        buffer >> satResidual.CACodeRes;
        buffer >> satResidual.P1CodeRes;
        buffer >> satResidual.P2CodeRes;
        buffer >> satResidual.DopplerL1Res;
        buffer >> satResidual.DopplerL2Res;
    }
}

void cSsnxParser::processRAIMStatistics_1_t(cDataBuffer& buffer)
{
    buffer >> mRAIMStatistics_1.dataValid;
    buffer >> mRAIMStatistics_1.timestamp_s;
    buffer >> mRAIMStatistics_1.Integrity;
    buffer >> mRAIMStatistics_1.PositionHERL;
    buffer >> mRAIMStatistics_1.PositionVERL;
    buffer >> mRAIMStatistics_1.VelocityHERL;
    buffer >> mRAIMStatistics_1.VelocityVERL;
    buffer >> mRAIMStatistics_1.UnityOverallModelP;
    buffer >> mRAIMStatistics_1.UnityOverallModelV;
    buffer >> mRAIMStatistics_1.N;
    buffer >> mRAIMStatistics_1.SBSize;

    for (auto& satData : mRAIMStatistics_1.RAIMChannel)
    {
        buffer >> satData.PRN;
        buffer >> satData.AntennaID;
        buffer >> satData.TestResults;
        buffer >> satData.UnityRangeW;
        buffer >> satData.UnityRrateW;
        buffer >> satData.RangeMDB;
        buffer >> satData.RrateMDB;
    }
}

void cSsnxParser::processPOS_Local_1_t(cDataBuffer& buffer)
{
    buffer >> mPOS_Local_1.dataValid;
    buffer >> mPOS_Local_1.timestamp_s;
    mPOS_Local_1.Mode = to_solution_type(buffer);
    buffer >> mPOS_Local_1.HeightComputed;
    buffer >> mPOS_Local_1.Error;
    buffer >> mPOS_Local_1.Lat_rad;
    buffer >> mPOS_Local_1.Lon_rad;
    buffer >> mPOS_Local_1.Alt_m;
    buffer >> mPOS_Local_1.Datum;
}

void cSsnxParser::processPOS_Projected_1_t(cDataBuffer& buffer)
{
    buffer >> mPOS_Projected_1.dataValid;
    buffer >> mPOS_Projected_1.timestamp_s;
    mPOS_Projected_1.Mode = to_solution_type(buffer);
    buffer >> mPOS_Projected_1.HeightComputed;
    buffer >> mPOS_Projected_1.Error;
    buffer >> mPOS_Projected_1.Northing_m;
    buffer >> mPOS_Projected_1.Easting_m;
    buffer >> mPOS_Projected_1.Alt_m;
    buffer >> mPOS_Projected_1.Datum;
}

void cSsnxParser::processReceiverTime_1_t(cDataBuffer& buffer)
{
    buffer >> mReceiverTime_1.dataValid;
    buffer >> mReceiverTime_1.timestamp_s;
    buffer >> mReceiverTime_1.utcYear;
    buffer >> mReceiverTime_1.utcMonth;
    buffer >> mReceiverTime_1.utcDay;
    buffer >> mReceiverTime_1.utcHour;
    buffer >> mReceiverTime_1.utcMinute;
    buffer >> mReceiverTime_1.utcSecond;
    buffer >> mReceiverTime_1.deltaLS;
    buffer >> mReceiverTime_1.WeekNumberValid;
    buffer >> mReceiverTime_1.TimeOfWeekWithin20ms;
    buffer >> mReceiverTime_1.TimeOfWeekWithinThreshold;
    buffer >> mReceiverTime_1.TimeFromPTTI;
    buffer >> mReceiverTime_1.TimeFromNTP;
}

void cSsnxParser::processRtcmDatum_1_t(cDataBuffer& buffer)
{
    mRtcmDatum_1;
    buffer >> mRtcmDatum_1.dataValid;
    buffer >> mRtcmDatum_1.timestamp_s;
    buffer >> mRtcmDatum_1.sourceCRS;
    buffer >> mRtcmDatum_1.targetCRS;
    buffer >> mRtcmDatum_1.datum;
    mRtcmDatum_1.heightType = to_height_type(buffer);
    mRtcmDatum_1.horizontalQualityInd = to_quality_indicator(buffer);
    mRtcmDatum_1.verticalQualityInd = to_quality_indicator(buffer);
}





