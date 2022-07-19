

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "ExperimentSerializer.hpp"
#include "ExperimentParser.hpp"

const char* TEST_FILENAME = "test.exp";
/*
void writeBeginHeader();
void writeEndOfHeader();

void writeBeginFooter();
void writeEndOfFooter();

void writeTitle(const std::string& title);
void writeResearcher(const std::string& researcher);
void writeCultivar(const std::string& cultivar);
void writeExperimentDoc(const std::string& doc);

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

//			REQUIRE(pvt.timeUnit() == pvt::eTIME_UNITS::NANOSECONDS);

			rd.close();
		}
	}
}


