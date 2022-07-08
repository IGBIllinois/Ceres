

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "DataBuffer.hpp"


TEST_CASE("Simple block file creation tests", "[initialization]")
{
	SECTION("Testing opening read/write files...")
	{
		cBlockDataFileWriter wrt;
		wrt.open("test.ceres");

		REQUIRE(wrt.isOpen());

		wrt.close();

		cBlockDataFileReader rd;
		rd.open("test.ceres");

		REQUIRE(rd.isOpen());

		rd.close();
	}

	SECTION("Testing read/write of string data...")
	{
		cBlockDataFileWriter wrt;
		wrt.open("test.ceres");

		REQUIRE(wrt.isOpen());

		wrt.close();

		cBlockDataFileReader rd;
		rd.open("test.ceres");

		REQUIRE(rd.isOpen());

		rd.close();
	}
}


