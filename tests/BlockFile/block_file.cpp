

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "DataBuffer.hpp"
#include "Crc.hpp"


TEST_CASE("Simple CRC tests", "[crc]")
{
	make_crc_table();

	SECTION("Testing simple crc checks...")
	{
		cBlockID id(static_cast<ClassIDs>(1000), 0, 0);

		auto crc = ::crc(id);

		REQUIRE(crc == 0xA04F762DUL);

		id.minorVersion(1);

		auto crc_min_ver = ::crc(id);
		REQUIRE(crc != crc_min_ver);
		REQUIRE(crc_min_ver == 0x6B13A588UL);

		id.majorVersion(1);
		auto crc_maj_ver = ::crc(id);
		REQUIRE(crc != crc_maj_ver);
		REQUIRE(crc_min_ver != crc_maj_ver);
		REQUIRE(crc_maj_ver == 0x56738C38UL);

		id.classID(static_cast<ClassIDs>(2000));
		crc = ::crc(id);

		REQUIRE(crc == 0x64E5F4AAUL);
	}

	SECTION("Testing crc with buffer checks...")
	{
		cBlockID id(static_cast<ClassIDs>(2000), 1, 0);

		// Empty buffer
		std::vector<std::byte> buffer;
		auto crc = ::crc(id, buffer.data(), buffer.size());
		REQUIRE(crc == 0xAFB9270FUL);

		// 64 byte buffer with zeros
		buffer.resize(64);
		crc = ::crc(id, buffer.data(), buffer.size());
		REQUIRE(crc == 0xB1C457FAUL);
	}
}

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


