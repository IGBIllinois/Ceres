

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/ExperimentSerializer.hpp>
#include <cbdf/ExperimentParser.hpp>


class cExperimentTestParser : public cExperimentParser
{
public:
	bool mHasBeginHeader = false;
	bool mHasEndOfHeader = false;

	bool mHasBeginFooter = false;
	bool mHasEndOfFooter = false;

	bool mHasBeginSensorList = false;
	bool mHasEndOfSensorList = false;

	std::string mTitle;
	std::string mResearcher;
	std::string mCultivar;
	std::string mExperimentDoc;

protected:
	void onBeginHeader() override { mHasBeginHeader = true; }
	void onEndOfHeader() override { mHasEndOfHeader = true; }

	void onBeginFooter() override { mHasBeginFooter = true; }
	void onEndOfFooter() override { mHasEndOfFooter = true; }

	void onTitle(const std::string& title) override {};
	void onResearcher(const std::string& researcher) override {};
	void onCultivar(const std::string& cultivar) override {};
	void onExperimentDoc(const std::string& doc) override {};

	void onBeginTreatmentList() override {};
	void onEndOfTreatmentList() override {};
	void onTreatment(const std::string& treatment) override {};

	void onFileDate(std::uint16_t year, std::uint8_t month, std::uint8_t day) override {};
	void onFileTime(std::uint8_t hour, std::uint8_t minute, std::uint8_t seconds) override {};

	void onDayOfYear(std::uint16_t day_of_year) override {};

	void onBeginSensorList() override {};
	void onEndOfSensorList() override {};
	void onSensorBlockInfo(uint16_t class_id, const std::string& name) override {};

	void onStartTime(sExperimentTime_t time) override {};
	void onEndTime(sExperimentTime_t time) override {};

	void onStartRecordingTimestamp(uint64_t timestamp_ns) override {};
	void onEndRecordingTimestamp(uint64_t timestamp_ns) override {};
	void onHeartbeatTimestamp(uint64_t timestamp_ns) override {};
};


TEST_CASE("Begin/End Marker tests", "[experiment tests]")
{
	SECTION("Testing write/read of begin/end header markers...")
	{
		const char* TEST_FILENAME = "exp_header_test.ceres";

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cExperimentSerializer exp(1024, &wrt);
			exp.setVersion(1, 0);

			exp.writeBeginHeader();
			exp.writeEndOfHeader();

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cExperimentTestParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(exp.mHasBeginHeader);
			REQUIRE(!exp.mHasEndOfHeader);
			REQUIRE(!exp.mHasBeginFooter);
			REQUIRE(!exp.mHasEndOfFooter);

			result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(exp.mHasBeginHeader);
			REQUIRE(exp.mHasEndOfHeader);
			REQUIRE(!exp.mHasBeginFooter);
			REQUIRE(!exp.mHasEndOfFooter);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}

	SECTION("Testing write/read of begin/end footer markers...")
	{
		const char* TEST_FILENAME = "exp_footer_test.ceres";

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cExperimentSerializer exp(1024, &wrt);
			exp.setVersion(1, 0);

			exp.writeBeginFooter();
			exp.writeEndOfFooter();

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cExperimentTestParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(!exp.mHasBeginHeader);
			REQUIRE(!exp.mHasEndOfHeader);
			REQUIRE(exp.mHasBeginFooter);
			REQUIRE(!exp.mHasEndOfFooter);

			result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(!exp.mHasBeginHeader);
			REQUIRE(!exp.mHasEndOfHeader);
			REQUIRE(exp.mHasBeginFooter);
			REQUIRE(exp.mHasEndOfFooter);

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
		const char* TEST_FILENAME = "exp_empty_header_test.ceres";

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

			cExperimentTestParser exp;
			rd.attach(&exp);

			auto result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.mHasBeginHeader);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.mTitle.empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.mResearcher.empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.mCultivar.empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.mExperimentDoc.empty());

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(exp.mHasEndOfHeader);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}


