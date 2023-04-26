

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/PvtSerializer.hpp>
#include <cbdf/PvtParser.hpp>


class cPvtTestParser : public cPvtParser
{
public:
	pvt::ePOSTION_UNITS  mPositionUnit = pvt::ePOSTION_UNITS::UNKNOWN;
	pvt::eVELOCITY_UNITS mVelocityUnit = pvt::eVELOCITY_UNITS::UNKNOWN;
	pvt::eTIME_UNITS mTimeUnit = pvt::eTIME_UNITS::UNKNOWN;

	double mX = 0.0;
	double mY = 0.0;
	double mZ = 0.0;

	double mVx = 0.0;
	double mVy = 0.0;
	double mVz = 0.0;

	std::uint64_t mTimeStamp = 0;

protected:
	void onPositionUnit(pvt::ePOSTION_UNITS units) override { mPositionUnit = units; }
	void onPosition(double x) override { mX = x; }
	void onPosition(double x, double y) override { mX = x; mY = y; }
	void onPosition(double x, double y, double z) override { mX = x; mY = y; mZ = z; }

	void onVelocityUnit(pvt::eVELOCITY_UNITS units) override { mVelocityUnit = units; }
	void onVelocity(double Vx) override { mVx = Vx; }
	void onVelocity(double Vx, double Vy) override { mVx = Vx; mVy = Vy; }
	void onVelocity(double Vx, double Vy, double Vz) override { mVx = Vx; mVy = Vy; mVz = Vz; }

	void onTimeUnit(pvt::eTIME_UNITS units) override { mTimeUnit = units; }
	void onTimeStamp(std::uint64_t timeStamp) override { mTimeStamp = timeStamp; }
};


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

			cPvtTestParser pvt;
			rd.attach(&pvt);

			auto result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeUnit == pvt::eTIME_UNITS::NANOSECONDS);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeStamp == 1);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeStamp == 10);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeStamp == 100);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeStamp == 1000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeStamp == 10000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeStamp == 100000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mTimeStamp == 1000000);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mPositionUnit == pvt::ePOSTION_UNITS::METERS);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mX == 1.0);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mX == 1.5);
			REQUIRE(pvt.mY == 1.5);

			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(pvt.mX == 1.23456789);
			REQUIRE(pvt.mY == 1.23456789);
			REQUIRE(pvt.mZ == 1.23456789);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}


