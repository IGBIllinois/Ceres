

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/DataBuffer.hpp>
#include <cbdf/extra/Crc.hpp>
#include <cbdf/BlockID.hpp>


TEST_CASE("Simple CRC tests", "[crc]")
{
	bdf::make_crc_table();

	SECTION("Testing simple crc checks...")
	{
		cBlockID id(static_cast<BLOCK_CLASS_ID_t>(1000), 0, 0);

		auto crc = bdf::crc(id);

		REQUIRE(crc == 0xA04F762DUL);

		id.minorVersion(1);

		auto crc_min_ver = bdf::crc(id);
		REQUIRE(crc != crc_min_ver);
		REQUIRE(crc_min_ver == 0x6B13A588UL);

		id.majorVersion(1);
		auto crc_maj_ver = bdf::crc(id);
		REQUIRE(crc != crc_maj_ver);
		REQUIRE(crc_min_ver != crc_maj_ver);
		REQUIRE(crc_maj_ver == 0x56738C38UL);

		id.classID(static_cast<BLOCK_CLASS_ID_t>(2000));
		crc = bdf::crc(id);

		REQUIRE(crc == 0x64E5F4AAUL);
	}

	SECTION("Testing crc with buffer checks...")
	{
		cBlockID id(static_cast<BLOCK_CLASS_ID_t>(2000), 1, 0);

		// Empty buffer
		std::vector<std::byte> buffer;
		auto crc = bdf::crc(id, buffer.data(), buffer.size());
		REQUIRE(crc == 0xAFB9270FUL);

		// 64 byte buffer with zeros
		buffer.resize(64);
		crc = bdf::crc(id, buffer.data(), buffer.size());
		REQUIRE(crc == 0xB1C457FAUL);

		// 64 byte buffer with zeros
		for (int i = 0; i < buffer.size(); ++i)
			buffer[i] = static_cast<std::byte>(i);
		crc = bdf::crc(id, buffer.data(), buffer.size());
		REQUIRE(crc == 0xD447FA40UL);
	}
}

TEST_CASE("Simple block file creation tests", "[initialization]")
{
	SECTION("Testing opening read/write files...")
	{
		const char* TEST_FILENAME = "block_file_empty_test.ceres";

		cBlockDataFileWriter wrt;
		wrt.open(TEST_FILENAME);

		REQUIRE(wrt.isOpen());

		wrt.close();

		cBlockDataFileReader rd;
		rd.open(TEST_FILENAME);

		REQUIRE(rd.isOpen());

		rd.close();
	}
}


