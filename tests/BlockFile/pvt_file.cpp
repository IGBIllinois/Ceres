

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "PvtSerializer.hpp"
#include "PvtParser.hpp"

TEST_CASE("Position/Velocity/Time tests", "[pvt tests]")
{
	SECTION("Testing write/read of simple pvt data...")
	{
		const char* TEST_FILENAME = "pvt_simple_test.ceres";

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cPvtSerializer pvt(1024, &wrt);
			pvt.write(pvt::eTIME_UNITS::NANOSECONDS);
			pvt.write_timestamp(1);
			pvt.write_timestamp(10);
			pvt.write_timestamp(100);
			pvt.write_timestamp(1000);
			pvt.write_timestamp(10000);
			pvt.write_timestamp(100000);
			pvt.write_timestamp(1000000);

			pvt.write(pvt::ePOSTION_UNITS::METERS);
			pvt.write_position(1.0);
			pvt.write_position(1.5, 1.5);
			pvt.write_position(1.23456789, 1.23456789, 1.23456789);

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cPvtParser pvt;
			rd.attach(&pvt);

			auto result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeUnit() == pvt::eTIME_UNITS::NANOSECONDS);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeStamp() == 1);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeStamp() == 10);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeStamp() == 100);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeStamp() == 1000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeStamp() == 10000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeStamp() == 100000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.timeStamp() == 1000000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.positionUnit() == pvt::ePOSTION_UNITS::METERS);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.X() == 1.0);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.X() == 1.5);
			REQUIRE(pvt.Y() == 1.5);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.X() == 1.23456789);
			REQUIRE(pvt.Y() == 1.23456789);
			REQUIRE(pvt.Z() == 1.23456789);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}


