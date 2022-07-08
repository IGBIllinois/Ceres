

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "PvtSerializer.hpp"
#include "PvtParser.hpp"


TEST_CASE("Position/Velocity/Time tests", "[pvt tests]")
{
	SECTION("Testing write/read of simple pvt data...")
	{
		{
			cBlockDataFileWriter wrt;
			wrt.open("test.pvt");

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

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open("test.pvt");

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
			REQUIRE(!result);

			rd.close();
		}
	}
}


