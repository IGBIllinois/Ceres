

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/WeatherSerializer.hpp>
#include <cbdf/WeatherParser.hpp>

#include <string>

class cWeatherTestParser : public cWeatherParser
{
public:
	std::string mConfigInfo;
	bool mWindValid = false;
	double mWindSpeed_mps = 0.0;
	double mWindSpeed_knots = 0.0;
	double mWindDirection_deg = 0.0;

	void onConfigInfo(const std::string& info) override
	{
		mConfigInfo = info;
	}

	void onWindData_mps(bool valid, double speed_mps, double dir_deg) override
	{
		mWindValid = valid;
		mWindSpeed_mps = speed_mps;
		mWindDirection_deg = dir_deg;
	}

	void onWindData_knots(bool valid, double speed_knots, double dir_deg) override
	{
		mWindValid = valid;
		mWindSpeed_knots = speed_knots;
		mWindDirection_deg = dir_deg;
	}

};

TEST_CASE("Simple Weather Data Tests", "[weather tests]")
{
	SECTION("Testing write/read of weather config info...")
	{
		const char* TEST_FILENAME = "weather_test.ceres";

		std::string info = "Test Config Info";
		bool valid = true;
		double speed_mps = 10.1234;
		double dir_deg = 14.725;
		double speed_knots = 5.6789;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cWeatherSerializer wx(1024, &wrt);
			wx.setVersion(1, 0);
			wx.writeConfigInfo(info);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cWeatherTestParser wx;
			rd.attach(&wx);

			auto result = rd.processBlock();
			REQUIRE(result);

			REQUIRE(info == wx.mConfigInfo);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}

	SECTION("Testing write/read of wind speed (mps) info...")
	{
		const char* TEST_FILENAME = "weather_test.ceres";

		bool valid = true;
		double speed_mps = 10.1234;
		double dir_deg = 14.725;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cWeatherSerializer wx(1024, &wrt);
			wx.setVersion(1, 0);
			wx.writeWindData_mps(valid, speed_mps, dir_deg);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cWeatherTestParser wx;
			rd.attach(&wx);

			// Reading wind valid
			auto result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(!wx.mWindValid);

			// Reading wind speed
			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(!wx.mWindValid);

			// Reading wind direction
			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(wx.mWindValid);
			REQUIRE(speed_mps == wx.mWindSpeed_mps);
			REQUIRE(dir_deg == wx.mWindDirection_deg);
			REQUIRE(0.0 == wx.mWindSpeed_knots);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}

	SECTION("Testing write/read of wind speed (knots) info...")
	{
		const char* TEST_FILENAME = "weather_test.ceres";

		bool valid = true;
		double speed_knots = 10.1234;
		double dir_deg = 14.725;

		{
			cBlockDataFileWriter wrt;
			wrt.open(TEST_FILENAME);

			REQUIRE(wrt.isOpen());
			cWeatherSerializer wx(1024, &wrt);
			wx.setVersion(1, 0);
			wx.writeWindData_knots(valid, speed_knots, dir_deg);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open(TEST_FILENAME);

			REQUIRE(rd.isOpen());

			cWeatherTestParser wx;
			rd.attach(&wx);

			// Reading wind valid
			auto result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(!wx.mWindValid);

			// Reading wind speed
			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(!wx.mWindValid);

			// Reading wind direction
			result = rd.processBlock();
			REQUIRE(result);
			REQUIRE(wx.mWindValid);
			REQUIRE(speed_knots == wx.mWindSpeed_knots);
			REQUIRE(dir_deg == wx.mWindDirection_deg);
			REQUIRE(0.0 == wx.mWindSpeed_mps);

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}

}
