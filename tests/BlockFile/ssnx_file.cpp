

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/SsnxSerializer.hpp>
#include <cbdf/SsnxParser.hpp>

#include <ssnx/gps_data.hpp>


class cSsnxTestParser : public cSsnxParser
{
public:
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

protected:
	void onPVT_Cartesian(ssnx::gps::PVT_Cartesian_1_t pos) override { mPVT_Cartesian_1 = pos; };
	void onPVT_Cartesian(ssnx::gps::PVT_Cartesian_2_t pos) override { mPVT_Cartesian_2 = pos; };
	void onPVT_Geodetic(ssnx::gps::PVT_Geodetic_1_t pos) override { mPVT_Geodetic_1 = pos; };
	void onPVT_Geodetic(ssnx::gps::PVT_Geodetic_2_t pos) override { mPVT_Geodetic_2 = pos; };
	void onPosCovGeodetic(ssnx::gps::PosCovGeodetic_1_t cov) override { mPosCovGeodetic_1 = cov; };
	void onVelCovGeodetic(ssnx::gps::VelCovGeodetic_1_t cov) override { mVelCovGeodetic_1 = cov; };
	void onDOP(ssnx::gps::DOP_1_t dop) override { mDOP_1 = dop; };
	void onPVT_Residuals(ssnx::gps::PVT_Residuals_1_t residuals) override { mPVT_Residuals_1 = residuals; };
	void onRAIMStatistics(ssnx::gps::RAIMStatistics_1_t raim) override { mRAIMStatistics_1 = raim; };
	void onPOS_Local(ssnx::gps::POS_Local_1_t pos) override { mPOS_Local_1 = pos; };
	void onPOS_Projected(ssnx::gps::POS_Projected_1_t pos) override { mPOS_Projected_1 = pos; };
	void onReceiverTime(ssnx::gps::ReceiverTime_1_t time) override { mReceiverTime_1 = time; };
	void onRtcmDatum(ssnx::gps::RtcmDatum_1_t datum) override { mRtcmDatum_1 = datum; };
};


TEST_CASE("PVT Cartesian tests", "[ssnx tests]")
{
	SECTION("Testing write/read of PVT_Cartesian_1_t data...")
	{
		const char* TEST_FILENAME = "ssnx_cartesian_1_test.ceres";

		ssnx::gps::PVT_Cartesian_1_t original_data;

		original_data.dataValid = true;
		original_data.timestamp_s = 12345.6789;
		original_data.BaseStationID = 1;
		original_data.Error = 0;
		original_data.GroundTrack_deg = 45.0;
		original_data.Info = 1;
		original_data.MeanCorrAge_s = 19;
		original_data.Mode = 2;
		original_data.NrSV = 6;
		original_data.RxClkBias_ms = 0.01;
		original_data.RxClkDrift_ppm = 2;
		original_data.SBASprn = 15;
		original_data.System = 3;
		original_data.Vx_mps = 0.0;
		original_data.Vy_mps = 0.0;
		original_data.Vz_mps = 0.0;
		original_data.X_m = 1000000;
		original_data.Y_m = 1500000;
		original_data.Z_m = 2000000;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cSsnxSerializer ssnx(1024, &wrt);
			ssnx.setVersion(1, 0);
			ssnx.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cSsnxTestParser ssnx;
			rd.attach(&ssnx);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto info = ssnx.mPVT_Cartesian_1;

			REQUIRE(info.BaseStationID == original_data.BaseStationID);
			REQUIRE(info.dataValid == original_data.dataValid);
			REQUIRE(info.Error == original_data.Error);
			REQUIRE(info.GroundTrack_deg == original_data.GroundTrack_deg);
			REQUIRE(info.Info == original_data.Info);
			REQUIRE(info.MeanCorrAge_s == original_data.MeanCorrAge_s);
			REQUIRE(info.Mode == original_data.Mode);
			REQUIRE(info.NrSV == original_data.NrSV);
			REQUIRE(info.RxClkBias_ms == original_data.RxClkBias_ms);
			REQUIRE(info.RxClkDrift_ppm == original_data.RxClkDrift_ppm);
			REQUIRE(info.SBASprn == original_data.SBASprn);
			REQUIRE(info.System == original_data.System);
			REQUIRE(info.timestamp_s == original_data.timestamp_s);
			REQUIRE(info.Vx_mps == original_data.Vx_mps);
			REQUIRE(info.Vy_mps == original_data.Vy_mps);
			REQUIRE(info.Vz_mps == original_data.Vz_mps);
			REQUIRE(info.X_m == original_data.X_m);
			REQUIRE(info.Y_m == original_data.Y_m);
			REQUIRE(info.Z_m == original_data.Z_m);

			rd.close();
		}
	}

	SECTION("Testing write/read of PVT_Cartesian_2_t data...")
	{
		const char* TEST_FILENAME = "ssnx_cartesian_2_test.ceres";

		ssnx::gps::PVT_Cartesian_2_t original_data;

		original_data.dataValid = true;
		original_data.timestamp_s = 12345.6789;
		original_data.Mode = ssnx::gps::eSolutionType::STAND_ALONE;
		original_data.HeightComputed = true;
		original_data.Error = 0;
		original_data.X_m = 1000000;
		original_data.Y_m = 1500000;
		original_data.Z_m = 2000000;
		original_data.Undulation_m = 1.5;
		original_data.Vx_mps = 0.0;
		original_data.Vy_mps = 0.0;
		original_data.Vz_mps = 0.0;
		original_data.GroundTrack_deg = 279.0;
		original_data.RxClkBias_ms = 100.0;
		original_data.RxClkDrift_ppm = 150.0;
		original_data.TimeSystem = ssnx::gps::eTimeSystem::GPS;
		original_data.Datum = ssnx::gps::eDatum::NAD83;
		original_data.NrSV = 0;
		original_data.SatClockCorrectionUsed = false;
		original_data.RangeCorrectionUsed = false;
		original_data.IonosphericInfoUsed = false;
		original_data.OrbitAccuracyInfoUsed = false;
		original_data.PrecisionApproachModeActive = false;
		original_data.ReferenceId = 1;
		original_data.MeanCorrAge_s = 30.2;
		original_data.SignalInfo = 12345;
		original_data.AlertFlag = 0;

		/* Version 2.1 of this packet*/
		original_data.NrBases = 1;
		original_data.AgeOfSeed_s = 5;
		original_data.LastSeed = ssnx::gps::ePPP_LastSeed::RTK_FIXED;

		/* Version 2.2 of this packet*/
		original_data.Latency_s = 6.12f;
		original_data.HAccuracy_m = 0.5f;
		original_data.VAccuracy_m = 0.5f;
		original_data.InRtkMode = true;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cSsnxSerializer ssnx(1024, &wrt);
			ssnx.setVersion(1, 0);
			ssnx.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cSsnxTestParser ssnx;
			rd.attach(&ssnx);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto info = ssnx.mPVT_Cartesian_2;

			REQUIRE(info.dataValid == original_data.dataValid);
			REQUIRE(info.timestamp_s == original_data.timestamp_s);
			REQUIRE(info.Error == original_data.Error);
			REQUIRE(info.GroundTrack_deg == original_data.GroundTrack_deg);
			REQUIRE(info.MeanCorrAge_s == original_data.MeanCorrAge_s);
			REQUIRE(info.NrSV == original_data.NrSV);
			REQUIRE(info.RxClkBias_ms == original_data.RxClkBias_ms);
			REQUIRE(info.RxClkDrift_ppm == original_data.RxClkDrift_ppm);
			REQUIRE(info.Vx_mps == original_data.Vx_mps);
			REQUIRE(info.Vy_mps == original_data.Vy_mps);
			REQUIRE(info.Vz_mps == original_data.Vz_mps);
			REQUIRE(info.X_m == original_data.X_m);
			REQUIRE(info.Y_m == original_data.Y_m);
			REQUIRE(info.Z_m == original_data.Z_m);

			rd.close();
		}
	}
}


TEST_CASE("PVT Geodetic tests", "[ssnx tests]")
{
	SECTION("Testing write/read of PVT_Geodetic_1_t data...")
	{
		const char* TEST_FILENAME = "ssnx_geodetic_1_test.ceres";

		ssnx::gps::PVT_Geodetic_1_t original_data;

		original_data.dataValid = true;
		original_data.timestamp_s = 12345.6789;
		original_data.BaseStationID = 1;
		original_data.Error = 0;
		original_data.GroundTrack_deg = 45.0;
		original_data.Info = 1;
		original_data.MeanCorrAge_s = 19;
		original_data.Mode = 2;
		original_data.NrSV = 6;
		original_data.RxClkBias_ms = 0.01;
		original_data.RxClkDrift_ppm = 2;
		original_data.SBASprn = 15;
		original_data.System = 3;
		original_data.Vn_mps = 0.0;
		original_data.Ve_mps = 0.0;
		original_data.Vu_mps = 0.0;
		original_data.Lat_rad = 0.69974304;
		original_data.Lon_rad = -1.53997544;
		original_data.Alt_m = 228.6;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cSsnxSerializer ssnx(1024, &wrt);
			ssnx.setVersion(1, 0);
			ssnx.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cSsnxTestParser ssnx;
			rd.attach(&ssnx);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto info = ssnx.mPVT_Geodetic_1;

			REQUIRE(info.BaseStationID == original_data.BaseStationID);
			REQUIRE(info.dataValid == original_data.dataValid);
			REQUIRE(info.Error == original_data.Error);
			REQUIRE(info.GroundTrack_deg == original_data.GroundTrack_deg);
			REQUIRE(info.Info == original_data.Info);
			REQUIRE(info.MeanCorrAge_s == original_data.MeanCorrAge_s);
			REQUIRE(info.Mode == original_data.Mode);
			REQUIRE(info.NrSV == original_data.NrSV);
			REQUIRE(info.RxClkBias_ms == original_data.RxClkBias_ms);
			REQUIRE(info.RxClkDrift_ppm == original_data.RxClkDrift_ppm);
			REQUIRE(info.SBASprn == original_data.SBASprn);
			REQUIRE(info.System == original_data.System);
			REQUIRE(info.timestamp_s == original_data.timestamp_s);
			REQUIRE(info.Vn_mps == original_data.Vn_mps);
			REQUIRE(info.Ve_mps == original_data.Ve_mps);
			REQUIRE(info.Vu_mps == original_data.Vu_mps);
			REQUIRE(info.Lat_rad == original_data.Lat_rad);
			REQUIRE(info.Lon_rad == original_data.Lon_rad);
			REQUIRE(info.Alt_m == original_data.Alt_m);

			rd.close();
		}
	}

	SECTION("Testing write/read of PVT_Geodetic_2_t data...")
	{
		const char* TEST_FILENAME = "ssnx_geodetic_2_test.ceres";

		ssnx::gps::PVT_Geodetic_2_t original_data;

		original_data.dataValid = true;
		original_data.timestamp_s = 12345.6789;
		original_data.Mode = ssnx::gps::eSolutionType::STAND_ALONE;
		original_data.HeightComputed = true;
		original_data.Error = 0;
		original_data.Lat_rad = 0.69974304;
		original_data.Lon_rad = -1.53997544;
		original_data.Height_m = 228.6;
		original_data.Undulation_m = 1.5;
		original_data.Vn_mps = 0.0;
		original_data.Ve_mps = 0.0;
		original_data.Vu_mps = 0.0;
		original_data.GroundTrack_deg = 279.0;
		original_data.RxClkBias_ms = 100.0;
		original_data.RxClkDrift_ppm = 150.0;
		original_data.TimeSystem = ssnx::gps::eTimeSystem::GPS;
		original_data.Datum = ssnx::gps::eDatum::NAD83;
		original_data.NrSV = 0;
		original_data.SatClockCorrectionUsed = false;
		original_data.RangeCorrectionUsed = false;
		original_data.IonosphericInfoUsed = false;
		original_data.OrbitAccuracyInfoUsed = false;
		original_data.PrecisionApproachModeActive = false;
		original_data.ReferenceId = 1;
		original_data.MeanCorrAge_s = 30.2;
		original_data.SignalInfo = 12345;
		original_data.AlertFlag = 0;

		/* Version 2.1 of this packet*/
		original_data.NrBases = 1;
		original_data.AgeOfSeed_s = 5;
		original_data.LastSeed = ssnx::gps::ePPP_LastSeed::RTK_FIXED;

		/* Version 2.2 of this packet*/
		original_data.Latency_s = 6.12f;
		original_data.HAccuracy_m = 0.5f;
		original_data.VAccuracy_m = 0.5f;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cSsnxSerializer ssnx(1024, &wrt);
			ssnx.setVersion(1, 0);
			ssnx.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cSsnxTestParser ssnx;
			rd.attach(&ssnx);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto info = ssnx.mPVT_Geodetic_2;

			REQUIRE(info.dataValid == original_data.dataValid);
			REQUIRE(info.timestamp_s == original_data.timestamp_s);
			REQUIRE(info.Error == original_data.Error);
			REQUIRE(info.GroundTrack_deg == original_data.GroundTrack_deg);
			REQUIRE(info.MeanCorrAge_s == original_data.MeanCorrAge_s);
			REQUIRE(info.NrSV == original_data.NrSV);
			REQUIRE(info.RxClkBias_ms == original_data.RxClkBias_ms);
			REQUIRE(info.RxClkDrift_ppm == original_data.RxClkDrift_ppm);
			REQUIRE(info.Vn_mps == original_data.Vn_mps);
			REQUIRE(info.Ve_mps == original_data.Ve_mps);
			REQUIRE(info.Vu_mps == original_data.Vu_mps);
			REQUIRE(info.Lat_rad == original_data.Lat_rad);
			REQUIRE(info.Lon_rad == original_data.Lon_rad);
			REQUIRE(info.Height_m == original_data.Height_m);

			rd.close();
		}
	}
}



