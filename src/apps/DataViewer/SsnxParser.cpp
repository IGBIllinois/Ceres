
#include "SsnxParser.hpp"
#include "SsnxDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <cassert>

using namespace ssnx;

namespace
{
    eSolutionType to_solution_type(uint8_t mode)
    {
        switch (mode & 0x0F)
        {
        case 1: return eSolutionType::STAND_ALONE;
        case 2: return eSolutionType::DIFFERENTIAL;
        case 3: return eSolutionType::FIXED;
        case 4: return eSolutionType::RTK_FIXED;
        case 5: return eSolutionType::RTK_FLOAT;
        case 6: return eSolutionType::SBAS;
        case 7: return eSolutionType::MOVING_RTK_FIXED;
        case 8: return eSolutionType::MOVING_RTK_FLOAT;
        case 10: return eSolutionType::PPP;
        }

        return eSolutionType::NONE;
    }

    eSolutionType get_solution_type(cDataBuffer& buffer)
    {
        return to_solution_type(buffer.get<uint8_t>());
    }

    eTimeSystem to_time_system(uint8_t ts)
    {
        switch (ts)
        {
        case 1: return eTimeSystem::GALILEO;
        case 3: return eTimeSystem::GLONASS;
        case 4: return eTimeSystem::BEIDOU;
        }

        return eTimeSystem::GPS;
    }

    eTimeSystem get_time_system(cDataBuffer& buffer)
    {
        return to_time_system(buffer.get<uint8_t>());
    }


    eDatum to_datum(uint8_t datum)
    {
        switch (datum)
        {
        case 19: return eDatum::DGNSS;
        case 30: return eDatum::ETRS89;
        case 31: return eDatum::NAD83;
        case 32: return eDatum::NAD83_PA11;
        case 33: return eDatum::NAD83_MA11;
        case 34: return eDatum::GDA94;
        case 250: return eDatum::FIRST_USER_DATUM;
        case 251: return eDatum::SECOND_USER_DATUM;
        }

        return eDatum::WGS84;
    }

    eDatum get_datum(cDataBuffer& buffer)
    {
        return to_datum(buffer.get<uint8_t>());
    }


    ePPP_LastSeed to_last_seed(uint16_t ppp_info)
    {
        auto type = (ppp_info & 0xE000) >> 12;
        switch (type)
        {
        case 1: return ePPP_LastSeed::MANUAL;
        case 2: return ePPP_LastSeed::DGPS;
        case 3: return ePPP_LastSeed::RTK_FIXED;
        }

        return ePPP_LastSeed::NONE;
    }

    ePPP_LastSeed get_last_seed(cDataBuffer& buffer)
    {
        return to_last_seed(buffer.get<uint16_t>());
    }


    eHeightType to_height_type(uint8_t type)
    {
        switch (type & 0x03)
        {
        case 1: return eHeightType::PHYSICAL_TARGET_CRS;
        case 2: return eHeightType::PHYSICAL_SOURCE_CRS;
        }

        return eHeightType::GEOMETRIC;
    }

    eHeightType get_height_type(cDataBuffer& buffer)
    {
        return to_height_type(buffer.get<uint8_t>());
    }


    eQualityIndicator to_quality_indicator(uint8_t indicator)
    {
        switch (indicator & 0x0F)
        {
        case 1: return eQualityIndicator::BETTER_THAN_21_MM;
        case 2: return eQualityIndicator::BETWEEN_21_MM_AND_50_MM;
        case 3: return eQualityIndicator::BETWEEN_51_MM_AND_200_MM;
        case 4: return eQualityIndicator::BETWEEN_201_MM_AND_500_MM;
        case 5: return eQualityIndicator::BETWEEN_501_MM_AND_2000_MM;
        case 6: return eQualityIndicator::BETWEEN_2001_MM_AND_5000_MM;
        case 7: return eQualityIndicator::WORSE_THAN_5001_MM;
        case 9: return eQualityIndicator::BETWEEN_0_MM_AND_10_MM;
        case 10: return eQualityIndicator::BETWEEN_11_MM_AND_20_MM;
        case 11: return eQualityIndicator::BETWEEN_21_MM_AND_50_MM;
        case 12: return eQualityIndicator::BETWEEN_51_MM_AND_100_MM;
        case 13: return eQualityIndicator::BETWEEN_101_MM_AND_200_MM;
        case 14: return eQualityIndicator::BETWEEN_201_MM_AND_500_MM;
        case 15: return eQualityIndicator::WORSE_THAN_501_MM;
        }

        return eQualityIndicator::UNKNOWN;
    }

    eQualityIndicator get_quality_indicator(cDataBuffer& buffer)
    {
        return to_quality_indicator(buffer.get<uint8_t>());
    }


    void get_PVT_Cartesian_2_t(cDataBuffer& buffer, PVT_Cartesian_2_t& data)
    {
        buffer >> data.dataValid;
        buffer >> data.timestamp_s;
        data.Mode = to_solution_type(buffer.get<uint8_t>());
        buffer >> data.HeightComputed;
        buffer >> data.Error;
        buffer >> data.X_m;
        buffer >> data.Y_m;
        buffer >> data.Z_m;
        buffer >> data.Undulation_m;
        buffer >> data.Vx_mps;
        buffer >> data.Vy_mps;
        buffer >> data.Vz_mps;
        buffer >> data.GroundTrack_deg;
        buffer >> data.RxClkBias_ms;
        buffer >> data.RxClkDrift_ppm;
        data.TimeSystem = get_time_system(buffer);
        data.Datum = get_datum(buffer);
        buffer >> data.NrSV;
        buffer >> data.SatClockCorrectionUsed;
        buffer >> data.RangeCorrectionUsed;
        buffer >> data.IonosphericInfoUsed;
        buffer >> data.OrbitAccuracyInfoUsed;
        buffer >> data.PrecisionApproachModeActive;
        buffer >> data.ReferenceId;
        buffer >> data.MeanCorrAge_s;
        buffer >> data.SignalInfo;
        buffer >> data.AlertFlag;
    }

    void get_PVT_Geodetic_2_t(cDataBuffer& buffer, PVT_Geodetic_2_t& data)
    {
        buffer >> data.dataValid;
        buffer >> data.timestamp_s;
        data.Mode = get_solution_type(buffer);
        buffer >> data.HeightComputed;
        buffer >> data.Error;
        buffer >> data.Lat_rad;
        buffer >> data.Lon_rad;
        buffer >> data.Height_m;
        buffer >> data.Undulation_m;
        buffer >> data.Vn_mps;
        buffer >> data.Ve_mps;
        buffer >> data.Vu_mps;
        buffer >> data.GroundTrack_deg;
        buffer >> data.RxClkBias_ms;
        buffer >> data.RxClkDrift_ppm;
        data.TimeSystem = get_time_system(buffer);
        data.Datum = get_datum(buffer);
        buffer >> data.NrSV;
        buffer >> data.SatClockCorrectionUsed;
        buffer >> data.RangeCorrectionUsed;
        buffer >> data.IonosphericInfoUsed;
        buffer >> data.OrbitAccuracyInfoUsed;
        buffer >> data.PrecisionApproachModeActive;
        buffer >> data.ReferenceId;
        buffer >> data.MeanCorrAge_s;
        buffer >> data.SignalInfo;
        buffer >> data.AlertFlag;
    }
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
        else if (major_version == 2)
        {
            switch (minor_version)
            {
            case 0:
                processPVT_Cartesian_2_t(buffer);
                break;
            case 1:
                processPVT_Cartesian_2_1_t(buffer);
                break;
            case 2:
                processPVT_Cartesian_2_2_t(buffer);
                break;
            }
        }
        break;
    case DataID::PVT_GEODETIC:
        if (major_version == 1)
            processPVT_Geodetic_1_t(buffer);
        else if (major_version == 2)
        {
            switch (minor_version)
            {
            case 0:
                processPVT_Geodetic_2_t(buffer);
                break;
            case 1:
                processPVT_Geodetic_2_1_t(buffer);
                break;
            case 2:
                processPVT_Geodetic_2_2_t(buffer);
                break;
            }
        }
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
    PVT_Cartesian_1_t data;

   buffer >> data.dataValid;
   buffer >> data.timestamp_s;
   buffer >> data.NrSV;
   buffer >> data.Error;
   buffer >> data.Mode;
   buffer >> data.System;
   buffer >> data.Info;
   buffer >> data.SBASprn;
   buffer >> data.X_m;
   buffer >> data.Y_m;
   buffer >> data.Z_m;
   buffer >> data.Vx_mps;
   buffer >> data.Vy_mps;
   buffer >> data.Vz_mps;
   buffer >> data.RxClkBias_ms;
   buffer >> data.RxClkDrift_ppm;
   buffer >> data.MeanCorrAge_s;
   buffer >> data.BaseStationID;
   buffer >> data.GroundTrack_deg;

   if (buffer.underrun())
       throw std::runtime_error("ERROR, Buffer under run in processPVT_Cartesian_1_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Cartesian_2_t(cDataBuffer& buffer)
{
    PVT_Cartesian_2_t data;

    get_PVT_Cartesian_2_t(buffer, data);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Cartesian_2_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Cartesian_2_1_t(cDataBuffer& buffer)
{
    PVT_Cartesian_2_1_t data;

    get_PVT_Cartesian_2_t(buffer, data);

    buffer >> data.NrBases;
    buffer >> data.AgeOfSeed_s;
    data.LastSeed = get_last_seed(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Cartesian_2_1_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Cartesian_2_2_t(cDataBuffer& buffer)
{
    PVT_Cartesian_2_2_t data;

    get_PVT_Cartesian_2_t(buffer, data);

    buffer >> data.NrBases;
    buffer >> data.AgeOfSeed_s;
    data.LastSeed = get_last_seed(buffer);
    buffer >> data.Latency_s;
    buffer >> data.HAccuracy_m;
    buffer >> data.VAccuracy_m;
    buffer >> data.InRtkMode;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Cartesian_2_2_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Geodetic_1_t(cDataBuffer& buffer)
{
    PVT_Geodetic_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    buffer >> data.NrSV;
    buffer >> data.Error;
    buffer >> data.Mode;
    buffer >> data.System;
    buffer >> data.Info;
    buffer >> data.SBASprn;
    buffer >> data.Lat_rad;
    buffer >> data.Lon_rad;
    buffer >> data.Alt_m;
    buffer >> data.Vn_mps;
    buffer >> data.Ve_mps;
    buffer >> data.Vu_mps;
    buffer >> data.RxClkBias_ms;
    buffer >> data.RxClkDrift_ppm;
    buffer >> data.GeoidHeight_m;
    buffer >> data.MeanCorrAge_s;
    buffer >> data.BaseStationID;
    buffer >> data.GroundTrack_deg;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Geodetic_1_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Geodetic_2_t(cDataBuffer& buffer)
{
    PVT_Geodetic_2_t data;

    get_PVT_Geodetic_2_t(buffer, data);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Geodetic_2_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Geodetic_2_1_t(cDataBuffer& buffer)
{
    PVT_Geodetic_2_1_t data;

    get_PVT_Geodetic_2_t(buffer, data);

    buffer >> data.NrBases;
    buffer >> data.AgeOfSeed_s;
    data.LastSeed = get_last_seed(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Geodetic_2_1_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Geodetic_2_2_t(cDataBuffer& buffer)
{
    PVT_Geodetic_2_2_t data;

    get_PVT_Geodetic_2_t(buffer, data);

    buffer >> data.NrBases;
    buffer >> data.AgeOfSeed_s;
    data.LastSeed = get_last_seed(buffer);

    buffer >> data.Latency_s;
    buffer >> data.HAccuracy_m;
    buffer >> data.VAccuracy_m;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Geodetic_2_2_t.");

    onNewData(data);
}

void cSsnxParser::processPosCovGeodetic_1_t(cDataBuffer& buffer)
{
    PosCovGeodetic_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    data.Mode = get_solution_type(buffer);
    buffer >> data.HeightComputed;
    buffer >> data.Error;
    buffer >> data.Cov_latlat_m;
    buffer >> data.Cov_lonlon_m;
    buffer >> data.Cov_hgthgt_m;
    buffer >> data.Cov_bb_m;
    buffer >> data.Cov_latlon_m;
    buffer >> data.Cov_lathgt_m;
    buffer >> data.Cov_latb_m;
    buffer >> data.Cov_lonhgt_m;
    buffer >> data.Cov_lonb_m;
    buffer >> data.Cov_hgtb_m;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPosCovGeodetic_1_t.");

    onNewData(data);
}

void cSsnxParser::processVelCovGeodetic_1_t(cDataBuffer& buffer)
{
    VelCovGeodetic_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    data.Mode = get_solution_type(buffer);
    buffer >> data.HeightComputed;
    buffer >> data.Error;
    buffer >> data.Cov_VnVn_mps;
    buffer >> data.Cov_VeVe_mps;
    buffer >> data.Cov_VuVu_mps;
    buffer >> data.Cov_DtDt_mps;
    buffer >> data.Cov_VnVe_mps;
    buffer >> data.Cov_VnVu_mps;
    buffer >> data.Cov_VnDt_mps;
    buffer >> data.Cov_VeVu_mps;
    buffer >> data.Cov_VeDt_mps;
    buffer >> data.Cov_VuDt_mps;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processVelCovGeodetic_1_t.");

    onNewData(data);
}

void cSsnxParser::processDOP_1_t(cDataBuffer& buffer)
{
    DOP_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    buffer >> data.NrSV;
    buffer >> data.PDOP;
    buffer >> data.TDOP;
    buffer >> data.HDOP;
    buffer >> data.VDOP;
    buffer >> data.HPL_m;
    buffer >> data.VPL_m;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processDOP_1_t.");

    onNewData(data);
}

void cSsnxParser::processPVT_Residuals_1_t(cDataBuffer& buffer)
{
    PVT_Residuals_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    buffer >> data.N;
    buffer >> data.SBSize;

    for (auto& satResidual : data.SatResidual)
    {
        buffer >> satResidual.PRN;
        buffer >> satResidual.CACodeRes;
        buffer >> satResidual.P1CodeRes;
        buffer >> satResidual.P2CodeRes;
        buffer >> satResidual.DopplerL1Res;
        buffer >> satResidual.DopplerL2Res;
    }

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPVT_Residuals_1_t.");

    onNewData(data);
}

void cSsnxParser::processRAIMStatistics_1_t(cDataBuffer& buffer)
{
    RAIMStatistics_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    buffer >> data.Integrity;
    buffer >> data.PositionHERL;
    buffer >> data.PositionVERL;
    buffer >> data.VelocityHERL;
    buffer >> data.VelocityVERL;
    buffer >> data.UnityOverallModelP;
    buffer >> data.UnityOverallModelV;
    buffer >> data.N;
    buffer >> data.SBSize;

    for (auto& satData : data.RAIMChannel)
    {
        buffer >> satData.PRN;
        buffer >> satData.AntennaID;
        buffer >> satData.TestResults;
        buffer >> satData.UnityRangeW;
        buffer >> satData.UnityRrateW;
        buffer >> satData.RangeMDB;
        buffer >> satData.RrateMDB;
    }

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processRAIMStatistics_1_t.");

    onNewData(data);
}

void cSsnxParser::processPOS_Projected_1_t(cDataBuffer& buffer)
{
    POS_Projected_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    data.Mode = get_solution_type(buffer);
    buffer >> data.HeightComputed;
    buffer >> data.Error;
    buffer >> data.Northing_m;
    buffer >> data.Easting_m;
    buffer >> data.Alt_m;
    buffer >> data.Datum;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processPOS_Projected_1_t.");

    onNewData(data);
}

void cSsnxParser::processReceiverTime_1_t(cDataBuffer& buffer)
{
    ReceiverTime_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    buffer >> data.utcYear;
    buffer >> data.utcMonth;
    buffer >> data.utcDay;
    buffer >> data.utcHour;
    buffer >> data.utcMinute;
    buffer >> data.utcSecond;
    buffer >> data.deltaLS;
    buffer >> data.WeekNumberValid;
    buffer >> data.TimeOfWeekWithin20ms;
    buffer >> data.TimeOfWeekWithinThreshold;
    buffer >> data.TimeFromPTTI;
    buffer >> data.TimeFromNTP;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processReceiverTime_1_t.");

    onNewData(data);
}

void cSsnxParser::processRtcmDatum_1_t(cDataBuffer& buffer)
{
    RtcmDatum_1_t data;

    buffer >> data.dataValid;
    buffer >> data.timestamp_s;
    buffer >> data.sourceCRS;
    buffer >> data.targetCRS;
    buffer >> data.datum;
    data.heightType = get_height_type(buffer);
    data.horizontalQualityInd = get_quality_indicator(buffer);
    data.verticalQualityInd = get_quality_indicator(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processRtcmDatum_1_t.");

    onNewData(data);
}

