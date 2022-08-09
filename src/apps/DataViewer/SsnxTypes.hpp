/**
 * \file
 * \brief Declarations and definitions of GPS data blocks.
 *
 */

#pragma once

#include <cstdint>
#include <string>

namespace ssnx
{
	/*--Base Header : -----------------------------------------------------------*/
	struct BaseHeader_t
	{
		bool dataValid;

		BaseHeader_t() : dataValid(false) {}
	};

	/*--Time Header : -----------------------------------------------------------*/
	struct TimeHeader_t : public BaseHeader_t
	{
		double timestamp_s;

		TimeHeader_t() : timestamp_s(0.0) {}
	};

	/*--Receiver Time Header : --------------------------------------------------*/
	struct ReceiverTimeHeader_t : public TimeHeader_t
	{};

	/*--SIS Time Header : -------------------------------------------------------*/
	struct SISTimeHeader_t : public TimeHeader_t
	{
		TimeHeader_t time;
	};

	/*--External Time Header : --------------------------------------------------*/
	struct ExternalTimeHeader_t : public TimeHeader_t
	{
		TimeHeader_t time;
	};

	/*--Navigation Header : -----------------------------------------------------*/
	struct NavigationHeader_t : public SISTimeHeader_t
	{
		uint8_t        SVID;
		uint8_t        CRCPassed;

		NavigationHeader_t() : SVID(0), CRCPassed(0) {}
	};

	/*--MeasEpoch Header : ------------------------------------------------------*/
	struct MeasEpochHeader_t : public ReceiverTimeHeader_t
	{
		uint8_t        N1;
		uint8_t        SB1Length;
		uint8_t        SB2Length;
	};


	/******************************************************************************
		* Position, Velocity and Time Blocks
		*****************************************************************************/
	enum class eTimeSystem
	{
		GPS,
		GALILEO,
		GLONASS,
		BEIDOU
	};

	enum class eSolutionType
	{
		NONE,
		STAND_ALONE,
		DIFFERENTIAL,
		FIXED,
		RTK_FIXED,
		RTK_FLOAT,
		SBAS,
		MOVING_RTK_FIXED,
		MOVING_RTK_FLOAT,
		PPP
	};

	enum class eDatum
	{
		WGS84,
		DGNSS,
		ETRS89,
		NAD83,
		NAD83_PA11,
		NAD83_MA11,
		GDA94,
		FIRST_USER_DATUM,
		SECOND_USER_DATUM
	};

	enum class ePPP_LastSeed
	{
		NONE,
		MANUAL,
		DGPS,
		RTK_FIXED
	};

	/* PVT in Cartesian coordinates */
	struct PVT_Cartesian_1_t : public ReceiverTimeHeader_t
	{
		uint8_t        NrSV;
		uint8_t        Error;
		uint8_t        Mode;
		uint8_t        System;
		uint8_t        Info;
		uint8_t        SBASprn;
		double		   X_m;
		double		   Y_m;
		double		   Z_m;
		float          Vx_mps;
		float          Vy_mps;
		float          Vz_mps;
		double		   RxClkBias_ms;
		float          RxClkDrift_ppm;
		float          MeanCorrAge_s;
		uint16_t       BaseStationID;
		float          GroundTrack_deg;
	};

	struct PVT_Cartesian_2_t : public ReceiverTimeHeader_t
	{
		eSolutionType  Mode;
		bool		   HeightComputed;
		uint8_t        Error;
		double		   X_m;
		double		   Y_m;
		double		   Z_m;
		float          Undulation_m;
		float          Vx_mps;
		float          Vy_mps;
		float          Vz_mps;
		float          GroundTrack_deg;
		double		   RxClkBias_ms;
		float          RxClkDrift_ppm;
		eTimeSystem    TimeSystem;
		eDatum         Datum;
		uint8_t        NrSV;
		bool		   SatClockCorrectionUsed;
		bool		   RangeCorrectionUsed;
		bool		   IonosphericInfoUsed;
		bool		   OrbitAccuracyInfoUsed;
		bool		   PrecisionApproachModeActive;
		uint16_t       ReferenceId;
		float          MeanCorrAge_s;
		uint32_t       SignalInfo;
		uint8_t        AlertFlag;
	};

	/* Version 2.1 of this packet*/
	struct PVT_Cartesian_2_1_t : public PVT_Cartesian_2_t
	{
		uint8_t        NrBases;
		uint16_t 	   AgeOfSeed_s;
		ePPP_LastSeed  LastSeed;
	};

	/* Version 2.2 of this packet*/
	struct PVT_Cartesian_2_2_t : public PVT_Cartesian_2_1_t
	{
		float  Latency_s;
		float  HAccuracy_m;
		float  VAccuracy_m;
		bool   InRtkMode;
	};

	/* PVT in geodetic coordinates */
	struct PVT_Geodetic_1_t : public ReceiverTimeHeader_t
	{
		uint8_t        NrSV;
		uint8_t        Error;
		uint8_t        Mode;
		uint8_t        System;
		uint8_t        Info;
		uint8_t        SBASprn;
		double	       Lat_rad;
		double		   Lon_rad;
		double		   Alt_m;
		float          Vn_mps;
		float          Ve_mps;
		float          Vu_mps;
		double		   RxClkBias_ms;
		float          RxClkDrift_ppm;
		float          GeoidHeight_m;
		float          MeanCorrAge_s;
		uint16_t       BaseStationID;
		float          GroundTrack_deg;
	};

	/* PVT in geodetic coordinates */
	struct PVT_Geodetic_2_t : public ReceiverTimeHeader_t
	{
		eSolutionType  Mode;
		bool		   HeightComputed;
		uint8_t        Error;
		double	       Lat_rad;
		double		   Lon_rad;
		double		   Height_m;
		float          Undulation_m;
		float          Vn_mps;
		float          Ve_mps;
		float          Vu_mps;
		float          GroundTrack_deg;
		double		   RxClkBias_ms;
		float          RxClkDrift_ppm;
		eTimeSystem    TimeSystem;
		eDatum         Datum;
		uint8_t        NrSV;
		bool		   SatClockCorrectionUsed;
		bool		   RangeCorrectionUsed;
		bool		   IonosphericInfoUsed;
		bool		   OrbitAccuracyInfoUsed;
		bool		   PrecisionApproachModeActive;
		uint16_t       ReferenceId;
		float          MeanCorrAge_s;
		uint32_t       SignalInfo;
		uint8_t        AlertFlag;
	};

	/* Version 2.1 of this packet*/
	struct PVT_Geodetic_2_1_t : public PVT_Geodetic_2_t
	{
		uint8_t        NrBases;
		uint16_t       AgeOfSeed_s;
		ePPP_LastSeed  LastSeed;
	};

	/* Version 2.2 of this packet*/
	struct PVT_Geodetic_2_2_t : public PVT_Geodetic_2_1_t
	{
		float	Latency_s;
		float   HAccuracy_m;
		float   VAccuracy_m;
	};

	/* Position Variance-Covariance matrix */
	struct PosCovGeodetic_1_t : public ReceiverTimeHeader_t
	{
		eSolutionType  Mode;
		bool		   HeightComputed;
		uint8_t        Error;
		float		   Cov_latlat_m;
		float		   Cov_lonlon_m;
		float		   Cov_hgthgt_m;
		float		   Cov_bb_m;
		float		   Cov_latlon_m;
		float		   Cov_lathgt_m;
		float		   Cov_latb_m;
		float		   Cov_lonhgt_m;
		float		   Cov_lonb_m;
		float		   Cov_hgtb_m;
	};

	/* Velocity Variance-Covariance matrix */
	struct VelCovGeodetic_1_t : public ReceiverTimeHeader_t
	{
		eSolutionType  Mode;
		bool		   HeightComputed;
		uint8_t        Error;
		float		   Cov_VnVn_mps;
		float		   Cov_VeVe_mps;
		float		   Cov_VuVu_mps;
		float		   Cov_DtDt_mps;
		float		   Cov_VnVe_mps;
		float		   Cov_VnVu_mps;
		float		   Cov_VnDt_mps;
		float		   Cov_VeVu_mps;
		float		   Cov_VeDt_mps;
		float		   Cov_VuDt_mps;
	};

	/* Dilution of precision */
	struct DOP_1_t : public TimeHeader_t
	{
		uint8_t     NrSV;
		float       PDOP;
		float       TDOP;
		float       HDOP;
		float       VDOP;
		float       HPL_m;
		float       VPL_m;
	};

	/* Measurement residuals */

	struct PVT_Residual_t
	{
		int16_t        CACodeRes;
		int16_t        P1CodeRes;
		int16_t        P2CodeRes;
		int16_t        DopplerL1Res;
		int16_t        DopplerL2Res;
		uint8_t        PRN;
	};

	struct PVT_Residuals_1_t : public TimeHeader_t
	{
		uint8_t        N;
		uint8_t        SBSize;
		PVT_Residual_t SatResidual[72];
	};


	/* Integrity statistics */

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

	struct RAIMStatistics_1_t : public TimeHeader_t
	{
		uint8_t        Integrity;
		float          PositionHERL;
		float          PositionVERL;
		float          VelocityHERL;
		float          VelocityVERL;
		uint16_t       UnityOverallModelP;
		uint16_t       UnityOverallModelV;
		uint8_t        N;
		uint8_t        SBSize;
		RAIMSatData_t  RAIMChannel[72];
	};

	/* OSNMA-Authenticated Position, velocity, and time in geodetic coordinates */

	/** PVTGeodeticAuth_1_2_t */
	struct PVT_GeodeticAuth_t : public TimeHeader_t
	{
		uint8_t        Mode;
		uint8_t        Error;
		double         Latitude;
		double         Longitude;
		double         Height;
		float          Undulation;
		float          Vn;
		float          Ve;
		float          Vu;
		float          COG;
		double		   RxClkBias;
		float          RxClkDrift;
		uint8_t        TimeSystem;
		uint8_t        Datum;
		uint8_t        NrSV;
		uint8_t        WACorrInfo;
		uint16_t       ReferenceID;
		uint16_t       MeanCorrAge;
		uint32_t       SignalInfo;
		uint8_t        AlertFlag;
		uint8_t        NrBases;
		uint16_t       PPPInfo;
		uint16_t       Latency;
		uint16_t       HAccuracy;
		uint16_t       VAccuracy;
		uint8_t        Misc;
	};

	/* Position projected in local coordinates */
	struct POS_Projected_1_t : public ReceiverTimeHeader_t
	{
		eSolutionType  Mode;
		bool		   HeightComputed;
		uint8_t        Error;
		double	       Northing_m;
		double		   Easting_m;
		double		   Alt_m;
		uint8_t        Datum;
	};

	/* Receiver Time */
	struct ReceiverTime_1_t : public ReceiverTimeHeader_t
	{
		int8_t      utcYear;
		int8_t      utcMonth;
		int8_t      utcDay;
		int8_t      utcHour;
		int8_t      utcMinute;
		int8_t      utcSecond;
		int8_t      deltaLS;
		bool		WeekNumberValid;
		bool		TimeOfWeekWithin20ms;
		bool		TimeOfWeekWithinThreshold;
		bool		TimeFromPTTI;
		bool		TimeFromNTP;
	};


	/******************************************************************************
		* Differential Correction Blocks
		*****************************************************************************/

	enum class eHeightType
	{
		GEOMETRIC,
		PHYSICAL_TARGET_CRS,
		PHYSICAL_SOURCE_CRS
	};

	enum class eQualityIndicator
	{
		UNKNOWN,
		BETTER_THAN_21_MM,
		BETWEEN_21_MM_AND_50_MM,
		BETWEEN_51_MM_AND_200_MM,
		BETWEEN_201_MM_AND_500_MM,
		BETWEEN_501_MM_AND_2000_MM,
		BETWEEN_2001_MM_AND_5000_MM,
		WORSE_THAN_5001_MM,
		BETWEEN_0_MM_AND_10_MM,
		BETWEEN_11_MM_AND_20_MM,
		BETWEEN_51_MM_AND_100_MM,
		BETWEEN_101_MM_AND_200_MM,
		WORSE_THAN_501_MM,
	};

	/* RTCM Datum */
	struct RtcmDatum_1_t : public ReceiverTimeHeader_t
	{
		std::string		   sourceCRS;
		std::string	  	   targetCRS;
		int8_t			   datum;
		eHeightType		   heightType;
		eQualityIndicator  horizontalQualityInd;
		eQualityIndicator  verticalQualityInd;
	};

}	/* End of Namespace ssnx */