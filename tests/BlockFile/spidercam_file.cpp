

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/SpidercamSerializer.hpp>
#include <cbdf/SpidercamParser.hpp>

#include <ssnx/gps_data.hpp>


class cSpidercamTestParser : public cSpidercamParser
{
public:
	spidercam::sPosition_1_t mPosition;

protected:
	void onPosition(spidercam::sPosition_1_t position) override { mPosition = position; }
};


TEST_CASE("Spidercam tests", "[spidercam tests]")
{
	SECTION("Testing write/read of spidercam position data...")
	{
		const char* TEST_FILENAME = "spidercam_test.ceres";

		spidercam::sPosition_1_t original_data;

		original_data.X_mm = 1000000;
		original_data.Y_mm = 1500000;
		original_data.Z_mm = 2000000;
		original_data.height_mm = 5000;
		original_data.speed_mmps = 490;
		original_data.pan_deg = 23.0;
		original_data.pan_speed_dps = 0.0;
		original_data.tilt_deg = 33.0;
		original_data.tilt_speed_dps = 0.0;
		original_data.roll_gimbal_deg = -67;
		original_data.zoom = 0;
		original_data.focus = 0;
		original_data.iris = 0;
		original_data.pitch_deg = 78.7654321;
		original_data.roll_deg = 32.987654321;
		original_data.timestamp = 1234567890;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cSpidercamSerializer spidercam(1024, &wrt);
			spidercam.setVersion(1, 0);

			spidercam.write(original_data);

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cSpidercamTestParser spidercam;
			rd.attach(&spidercam);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(spidercam.mPosition.X_mm == original_data.X_mm);
			REQUIRE(spidercam.mPosition.Y_mm == original_data.Y_mm);
			REQUIRE(spidercam.mPosition.Z_mm == original_data.Z_mm);
			REQUIRE(spidercam.mPosition.height_mm == original_data.height_mm);
			REQUIRE(spidercam.mPosition.speed_mmps == original_data.speed_mmps);
			REQUIRE(spidercam.mPosition.pan_deg == original_data.pan_deg);
			REQUIRE(spidercam.mPosition.pan_speed_dps == original_data.pan_speed_dps);
			REQUIRE(spidercam.mPosition.tilt_deg == original_data.tilt_deg);
			REQUIRE(spidercam.mPosition.tilt_speed_dps == original_data.tilt_speed_dps);
			REQUIRE(spidercam.mPosition.roll_gimbal_deg == original_data.roll_gimbal_deg);
			REQUIRE(spidercam.mPosition.zoom == original_data.zoom);
			REQUIRE(spidercam.mPosition.focus == original_data.focus);
			REQUIRE(spidercam.mPosition.iris == original_data.iris);
			REQUIRE(spidercam.mPosition.pitch_deg == original_data.pitch_deg);
			REQUIRE(spidercam.mPosition.roll_deg == original_data.roll_deg);
			REQUIRE(spidercam.mPosition.timestamp == original_data.timestamp);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}


