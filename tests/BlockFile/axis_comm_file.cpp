

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "SsnxSerializer.hpp"
#include "SsnxParser.hpp"

#include <ssnx/gps_data.hpp>

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

#if 0

TEST_CASE("PVT Cartesian tests", "[ssnx tests]")
{
	SECTION("Testing write/read of PVT_Cartesian_1_t data...")
	{
		ssnx::gps::PVT_Cartesian_1_t original_data;

		original_data.BaseStationID = 1;
		original_data.dataValid = true;
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
		original_data.timestamp_s = 12345.6789;
		original_data.Vx_mps = 0.0;
		original_data.Vy_mps = 0.0;
		original_data.Vz_mps = 0.0;
		original_data.X_m = 1000000;
		original_data.Y_m = 1500000;
		original_data.Z_m = 2000000;

		{
			cBlockDataFileWriter wrt;
			wrt.open("test.ssnx");

			REQUIRE(wrt.isOpen());
			cSsnxSerializer ssnx(1024, &wrt);
			ssnx.setVersion(1, 0);

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open("test.ssnx");

			REQUIRE(rd.isOpen());

			cSsnxParser ssnx;
			rd.attach(&ssnx);

			auto result = rd.processBlock();
			REQUIRE(result);

//			REQUIRE(pvt.timeUnit() == pvt::eTIME_UNITS::NANOSECONDS);

			rd.close();
		}
	}
}

#endif


