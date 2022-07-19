

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "ExperimentSerializer.hpp"
#include "ExperimentParser.hpp"

const char* TEST_FILENAME = "test.exp";
/*
void writeBeginSensorList();
void writeEndOfSensorList();
void writeSensorBlockInfo(uint16_t class_id, const std::string& name);

void startTime(time_t time);
void endTime(time_t time);

void startRecordingTimestamp(uint64_t timestamp);
void endRecordingTimestamp(uint64_t timestamp);
*/

TEST_CASE("Begin/End Marker tests", "[experiment tests]")
{
	SECTION("Testing write/read of begin/end header markers...")
	{
		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cExperimentSerializer exp(1024, &wrt);
			exp.setVersion(1, 0);

			exp.writeBeginHeader();

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cExperimentParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(exp.hasBeginHeader());
			REQUIRE(!exp.hasEndOfHeader());
			REQUIRE(!exp.hasBeginFooter());
			REQUIRE(!exp.hasEndOfFooter());

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cExperimentSerializer exp(1024, &wrt);
			exp.setVersion(1, 0);

			exp.writeEndOfHeader();

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cExperimentParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(!exp.hasBeginHeader());
			REQUIRE(exp.hasEndOfHeader());
			REQUIRE(!exp.hasBeginFooter());
			REQUIRE(!exp.hasEndOfFooter());

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}

	SECTION("Testing write/read of begin/end footer markers...")
	{
		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cExperimentSerializer exp(1024, &wrt);
			exp.setVersion(1, 0);

			exp.writeBeginFooter();

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cExperimentParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(!exp.hasBeginHeader());
			REQUIRE(!exp.hasEndOfHeader());
			REQUIRE(exp.hasBeginFooter());
			REQUIRE(!exp.hasEndOfFooter());

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cExperimentSerializer exp(1024, &wrt);
			exp.setVersion(1, 0);

			exp.writeEndOfFooter();

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cExperimentParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(!exp.hasBeginHeader());
			REQUIRE(!exp.hasEndOfHeader());
			REQUIRE(!exp.hasBeginFooter());
			REQUIRE(exp.hasEndOfFooter());

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}


TEST_CASE("Experiment header tests", "[experiment tests]")
{
	SECTION("Testing write/read of empty header data...")
	{
		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cExperimentSerializer exp(1024, &wrt);
			exp.setVersion(1, 0);

			exp.writeBeginHeader();

			exp.writeTitle("");
			exp.writeResearcher("");
			exp.writeCultivar("");
			exp.writeExperimentDoc("");

			exp.writeEndOfHeader();


			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cExperimentParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.hasBeginHeader());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.title().empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.researcher().empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.cultivar().empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.experimentDoc().empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.hasEndOfHeader());

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}


