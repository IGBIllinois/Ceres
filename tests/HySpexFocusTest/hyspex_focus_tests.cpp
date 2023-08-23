

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "Constants.hpp"

#include <HySpexConnect/HySpexData.hpp>

#include <valarray>

using namespace HySpexConnect;

namespace
{
	template<typename T>
	std::valarray<double> convolution1D(view<T>& f)
	{
		const std::array<double, 3> focus_convolution = { -1, 2, -1 };
		const int nf = f.size();
		constexpr int ng = focus_convolution.size();
		int const n = nf + ng - 1;

		std::valarray<double> out;

		// Note: we shrink n by four bacause we want to get rid of edge effects
		out.resize(n-4, 0.0);

		// Note: we start at 2 and goto n-2 to avoid edge effects
		for (auto i(2); i < (n-2); ++i)
		{
			const int jmn = (i >= ng - 1) ? i - (ng - 1) : 0;
			const int jmx = (i < nf - 1) ? i : nf - 1;
			for (auto j(jmn); j <= jmx; ++j)
			{
				out[i-2] += (f[j] * focus_convolution[i - j]);
			}
		}
		return std::move(out);
	}

	double computeFocusNumber(const cSpatialMajorData<float>& image)
	{
		double focusNumber = 0;

		for (int b = 0; b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			std::valarray<double> convolution_result = convolution1D(chns);
			double mean = convolution_result.sum() / convolution_result.size();
			double sd = 0.0;
			for (auto v : convolution_result)
				sd += (v - mean) * (v - mean);
			focusNumber += sd / convolution_result.size();
		}

		focusNumber = sqrt(focusNumber / image.num_bands());

		return focusNumber;
	}
}


TEST_CASE("Compute focus number on constant \"flat\" spectra", "[constant tests]")
{
	SECTION("Testing an image of all zeros...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);

		auto fn = computeFocusNumber(image);

		REQUIRE(fn == 0);
	}

	SECTION("Testing an image of all 1000...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);
		image += 1000;

		auto fn = computeFocusNumber(image);

		REQUIRE(fn == 0);
	}

	SECTION("Testing an image of all 123456.0f...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);
		image += 123456.0f;

		auto fn = computeFocusNumber(image);

		REQUIRE(fn == 0);
	}

	SECTION("Testing an image of all 0.123456f...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);
		image += 0.123456f;

		auto fn = computeFocusNumber(image);

		REQUIRE(fn == 0);
	}
}


TEST_CASE("Compute focus number on flat spectra", "[linear tests]")
{
	SECTION("Testing an image linear increasing...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);
		for (std::size_t i = 0; i < image.size(); ++i)
			image.data()[i] = i;

		auto fn = computeFocusNumber(image);

		REQUIRE(fn == 0);
	}

	SECTION("Testing an image linear decreasing...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);
		for (std::size_t i = 0; i < image.size(); ++i)
			image.data()[i] = image.size() - i;

		auto fn = computeFocusNumber(image);

		REQUIRE(fn == 0);
	}

	SECTION("Testing an image linear increasing...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 3.0 * c;
		}

		auto fn = computeFocusNumber(image);

		REQUIRE(fn == 0);
	}

}


TEST_CASE("Compute focus number on cosine spectra", "[cosine tests]")
{
	double prevFocusNumber = 0.0;

	SECTION("Testing a cosine image...")
	{
		cSpatialMajorData<float> image;
		image.resize(3408, 700);

		double omega = nConstants::PI / 3408;
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		prevFocusNumber = computeFocusNumber(image);

		omega = nConstants::PI / (3408 * 0.9);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.8);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.7);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.6);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.5);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.4);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.3);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.2);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}

		omega = nConstants::PI / (3408 * 0.1);
		for (std::size_t b(0); b < image.num_bands(); ++b)
		{
			auto chns = image.band(b);
			for (std::size_t c(0); c < chns.size(); ++c)
				chns[c] = 1000.0 + 200.0 * cos(omega * c);
		}

		{
			auto fn = computeFocusNumber(image);
			REQUIRE(fn > prevFocusNumber);
			prevFocusNumber = fn;
		}
	}

}


