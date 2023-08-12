

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch_all.hpp>

#include "Hyperspectral/HyperspectralData.hpp"

#if 0

using namespace hyperspectral;

TEST_CASE("Dynamic Hyperspectral Buffer Tests", "[basic tests]")
{
	SECTION("Testing spectral_major_data<int> creation...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data<int> test;
		REQUIRE(test.empty());
		REQUIRE(test.size() == 0);
		REQUIRE(test.num_pixels() == 0);
		REQUIRE(test.num_wavelengths() == 0);

		test.resize(10, 5);
		REQUIRE(!test.empty());
		REQUIRE(test.size() == 50);
		REQUIRE(test.num_pixels() == 10);
		REQUIRE(test.num_wavelengths() == 5);

		test.clear();
		REQUIRE(test.empty());
		REQUIRE(test.size() == 0);
		REQUIRE(test.num_pixels() == 0);
		REQUIRE(test.num_wavelengths() == 0);
	}

	SECTION("Testing spatial_major_data<int> creation...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data<int> test;
		REQUIRE(test.empty());
		REQUIRE(test.size() == 0);
		REQUIRE(test.num_pixels() == 0);
		REQUIRE(test.num_wavelengths() == 0);

		test.resize(10, 5);
		REQUIRE(!test.empty());
		REQUIRE(test.size() == 50);
		REQUIRE(test.num_pixels() == 10);
		REQUIRE(test.num_wavelengths() == 5);

		test.clear();
		REQUIRE(test.empty());
		REQUIRE(test.size() == 0);
		REQUIRE(test.num_pixels() == 0);
		REQUIRE(test.num_wavelengths() == 0);
	}
}


TEST_CASE("Fixed Hyperspectral Buffer Tests", "[basic tests]")
{
	SECTION("Testing spectral_major_data_fixed<int> creation...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data_fixed<int, 10, 5> test;
		REQUIRE(test.size() == 50);
		REQUIRE(test.num_pixels() == 10);
		REQUIRE(test.num_wavelengths() == 5);
	}

	SECTION("Testing spatial_major_data_fixed<int> creation...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data_fixed<int, 10, 5> test;
		REQUIRE(test.size() == 50);
		REQUIRE(test.num_pixels() == 10);
		REQUIRE(test.num_wavelengths() == 5);
	}
}


TEST_CASE("Dynamic Hyperspectral Buffer Tests", "[storage/retrieval tests]")
{
	SECTION("Testing spectral_major_data<int> data storage/retrieval...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data<int> test;
		test.resize(10, 5);

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		REQUIRE(test.get(0, 0) == 100);
		REQUIRE(test.get(1, 0) == 101);
		REQUIRE(test.get(2, 0) == 102);
		REQUIRE(test.get(3, 0) == 103);
		REQUIRE(test.get(4, 0) == 104);
		REQUIRE(test.get(5, 0) == 105);
		REQUIRE(test.get(6, 0) == 106);
		REQUIRE(test.get(7, 0) == 107);
		REQUIRE(test.get(8, 0) == 108);
		REQUIRE(test.get(9, 0) == 109);

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		REQUIRE(test.get(1, 0) == 100);
		REQUIRE(test.get(1, 1) == 101);
		REQUIRE(test.get(1, 2) == 102);
		REQUIRE(test.get(1, 3) == 103);
		REQUIRE(test.get(1, 4) == 104);
	}

	SECTION("Testing spatial_major_data<int> data storage/retrieval...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data<int> test;
		test.resize(10, 5);

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		REQUIRE(test.get(0, 0) == 100);
		REQUIRE(test.get(1, 0) == 101);
		REQUIRE(test.get(2, 0) == 102);
		REQUIRE(test.get(3, 0) == 103);
		REQUIRE(test.get(4, 0) == 104);
		REQUIRE(test.get(5, 0) == 105);
		REQUIRE(test.get(6, 0) == 106);
		REQUIRE(test.get(7, 0) == 107);
		REQUIRE(test.get(8, 0) == 108);
		REQUIRE(test.get(9, 0) == 109);

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		REQUIRE(test.get(1, 0) == 100);
		REQUIRE(test.get(1, 1) == 101);
		REQUIRE(test.get(1, 2) == 102);
		REQUIRE(test.get(1, 3) == 103);
		REQUIRE(test.get(1, 4) == 104);
	}
}


TEST_CASE("Fixed Hyperspectral Buffer Tests", "[storage/retrieval tests]")
{
	SECTION("Testing spectral_major_data_fixed<int> data storage/retrieval...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data_fixed<int, 10, 5> test;

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		REQUIRE(test.get(0, 0) == 100);
		REQUIRE(test.get(1, 0) == 101);
		REQUIRE(test.get(2, 0) == 102);
		REQUIRE(test.get(3, 0) == 103);
		REQUIRE(test.get(4, 0) == 104);
		REQUIRE(test.get(5, 0) == 105);
		REQUIRE(test.get(6, 0) == 106);
		REQUIRE(test.get(7, 0) == 107);
		REQUIRE(test.get(8, 0) == 108);
		REQUIRE(test.get(9, 0) == 109);

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		REQUIRE(test.get(1, 0) == 100);
		REQUIRE(test.get(1, 1) == 101);
		REQUIRE(test.get(1, 2) == 102);
		REQUIRE(test.get(1, 3) == 103);
		REQUIRE(test.get(1, 4) == 104);
	}

	SECTION("Testing spatial_major_data_fixed<int> data storage/retrieval...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data_fixed<int, 10, 5> test;

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		REQUIRE(test.get(0, 0) == 100);
		REQUIRE(test.get(1, 0) == 101);
		REQUIRE(test.get(2, 0) == 102);
		REQUIRE(test.get(3, 0) == 103);
		REQUIRE(test.get(4, 0) == 104);
		REQUIRE(test.get(5, 0) == 105);
		REQUIRE(test.get(6, 0) == 106);
		REQUIRE(test.get(7, 0) == 107);
		REQUIRE(test.get(8, 0) == 108);
		REQUIRE(test.get(9, 0) == 109);

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		REQUIRE(test.get(1, 0) == 100);
		REQUIRE(test.get(1, 1) == 101);
		REQUIRE(test.get(1, 2) == 102);
		REQUIRE(test.get(1, 3) == 103);
		REQUIRE(test.get(1, 4) == 104);
	}
}


TEST_CASE("Dynamic Hyperspectral Buffer Tests", "[view tests]")
{
	SECTION("Testing spectral_major_data<int> view...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data<int> test;
		test.resize(10, 5);

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);
		
		// This is required for force the use of the const version of the pixels method
		const hyperspectral::spectral_major_data<int> test1(test);

		view<int> pixels = test1.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() == pixels.cbegin());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.end() == pixels.cend());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() == pixels.crbegin());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.rend() == pixels.crend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		// This is required for force the use of the const version of the wavelengths method
		const hyperspectral::spectral_major_data<int> test2(test);

		view<int> wavelengths = test2.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() == wavelengths.cbegin());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.end() == wavelengths.cend());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() == wavelengths.crbegin());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.rend() == wavelengths.crend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());

			REQUIRE(*(--wavelength) == 104);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 100);

			REQUIRE(wavelength == wavelengths.cbegin());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());

			REQUIRE(*(--wavelength) == 100);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 104);

			REQUIRE(wavelength == wavelengths.crbegin());
		}
	}

	SECTION("Testing spatial_major_data<int> view...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data<int> test;
		test.resize(10, 5);

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		// This is required for force the use of the const version of the pixels method
		const hyperspectral::spatial_major_data<int> test1(test);

		view<int> pixels = test1.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() == pixels.cbegin());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.end() == pixels.cend());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() == pixels.crbegin());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.rend() == pixels.crend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		// This is required for force the use of the const version of the wavelengths method
		const hyperspectral::spatial_major_data<int> test2(test);

		view<int> wavelengths = test2.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() == wavelengths.cbegin());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.end() == wavelengths.cend());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() == wavelengths.crbegin());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.rend() == wavelengths.crend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());

			REQUIRE(*(--wavelength) == 104);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 100);

			REQUIRE(wavelength == wavelengths.cbegin());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());

			REQUIRE(*(--wavelength) == 100);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 104);

			REQUIRE(wavelength == wavelengths.crbegin());
		}
	}
}


TEST_CASE("Fixed Hyperspectral Buffer Tests", "[view tests]")
{
	SECTION("Testing spectral_major_data_fixed<int, 10, 5> view...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data_fixed<int, 10, 5> test;

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		// This is required for force the use of the const version of the pixels method
		const hyperspectral::spectral_major_data_fixed<int, 10, 5> test1(test);

		view<int> pixels = test1.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() == pixels.cbegin());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.end() == pixels.cend());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() == pixels.crbegin());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.rend() == pixels.crend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		// This is required for force the use of the const version of the wavelengths method
		const hyperspectral::spectral_major_data_fixed<int, 10, 5> test2(test);

		view<int> wavelengths = test2.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() == wavelengths.cbegin());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.end() == wavelengths.cend());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() == wavelengths.crbegin());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.rend() == wavelengths.crend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());

			REQUIRE(*(--wavelength) == 104);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 100);

			REQUIRE(wavelength == wavelengths.cbegin());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());

			REQUIRE(*(--wavelength) == 100);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 104);

			REQUIRE(wavelength == wavelengths.crbegin());
		}
	}

	SECTION("Testing spatial_major_data_fixed<int, 10, 5> view...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data_fixed<int, 10, 5> test;

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		// This is required for force the use of the const version of the pixels method
		const hyperspectral::spatial_major_data_fixed<int, 10, 5> test1(test);

		view<int> pixels = test1.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() == pixels.cbegin());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.end() == pixels.cend());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() == pixels.crbegin());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.rend() == pixels.crend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		// This is required for force the use of the const version of the wavelengths method
		const hyperspectral::spatial_major_data_fixed<int, 10, 5> test2(test);

		view<int> wavelengths = test2.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() == wavelengths.cbegin());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.end() == wavelengths.cend());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() == wavelengths.crbegin());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.rend() == wavelengths.crend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());

			REQUIRE(*(--wavelength) == 104);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 100);

			REQUIRE(wavelength == wavelengths.cbegin());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());

			REQUIRE(*(--wavelength) == 100);
			REQUIRE(*(--wavelength) == 101);
			REQUIRE(*(--wavelength) == 102);
			REQUIRE(*(--wavelength) == 103);
			REQUIRE(*(--wavelength) == 104);

			REQUIRE(wavelength == wavelengths.crbegin());
		}
	}
}


TEST_CASE("Dynamic Hyperspectral Buffer Tests", "[span tests]")
{
	SECTION("Testing spectral_major_data<int> span...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data<int> test;
		test.resize(10, 5);

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		span<int> pixels = test.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		span<int> wavelengths = test.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());
		}
	}

	SECTION("Testing spatial_major_data<int> span...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data<int> test;
		test.resize(10, 5);

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		span<int> pixels = test.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		span<int> wavelengths = test.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());
		}
	}
}


TEST_CASE("Fixed Hyperspectral Buffer Tests", "[span tests]")
{
	SECTION("Testing spectral_major_data_fixed<int, 10, 5> span...")
	{
		// Create test buffer
		hyperspectral::spectral_major_data_fixed<int, 10, 5> test;

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		span<int> pixels = test.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		span<int> wavelengths = test.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());
		}
	}

	SECTION("Testing spatial_major_data_fixed<int, 10, 5> span...")
	{
		// Create test buffer
		hyperspectral::spatial_major_data_fixed<int, 10, 5> test;

		test.set(0, 0, 100);
		test.set(1, 0, 101);
		test.set(2, 0, 102);
		test.set(3, 0, 103);
		test.set(4, 0, 104);
		test.set(5, 0, 105);
		test.set(6, 0, 106);
		test.set(7, 0, 107);
		test.set(8, 0, 108);
		test.set(9, 0, 109);

		span<int> pixels = test.pixels(0);
		REQUIRE(!pixels.empty());
		REQUIRE(10 == pixels.size());
		REQUIRE(pixels.begin() != pixels.end());
		REQUIRE(pixels.cbegin() != pixels.cend());
		REQUIRE(pixels.rbegin() != pixels.rend());
		REQUIRE(pixels.crbegin() != pixels.crend());

		// Check forward iterator
		{
			auto pixel = pixels.cbegin();

			REQUIRE(*(pixel++) == 100);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 109);

			REQUIRE(pixel == pixels.cend());

			REQUIRE(*(--pixel) == 109);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 100);

			REQUIRE(pixel == pixels.cbegin());
		}

		// Check reverse iterator
		{
			auto pixel = pixels.crbegin();

			REQUIRE(*(pixel++) == 109);
			REQUIRE(*(pixel++) == 108);
			REQUIRE(*(pixel++) == 107);
			REQUIRE(*(pixel++) == 106);
			REQUIRE(*(pixel++) == 105);
			REQUIRE(*(pixel++) == 104);
			REQUIRE(*(pixel++) == 103);
			REQUIRE(*(pixel++) == 102);
			REQUIRE(*(pixel++) == 101);
			REQUIRE(*(pixel++) == 100);

			REQUIRE(pixel == pixels.crend());

			REQUIRE(*(--pixel) == 100);
			REQUIRE(*(--pixel) == 101);
			REQUIRE(*(--pixel) == 102);
			REQUIRE(*(--pixel) == 103);
			REQUIRE(*(--pixel) == 104);
			REQUIRE(*(--pixel) == 105);
			REQUIRE(*(--pixel) == 106);
			REQUIRE(*(--pixel) == 107);
			REQUIRE(*(--pixel) == 108);
			REQUIRE(*(--pixel) == 109);

			REQUIRE(pixel == pixels.crbegin());
		}

		test.set(1, 0, 100);
		test.set(1, 1, 101);
		test.set(1, 2, 102);
		test.set(1, 3, 103);
		test.set(1, 4, 104);

		span<int> wavelengths = test.wavelengths(1);
		REQUIRE(!wavelengths.empty());
		REQUIRE(5 == wavelengths.size());
		REQUIRE(wavelengths.begin() != wavelengths.end());
		REQUIRE(wavelengths.cbegin() != wavelengths.cend());
		REQUIRE(wavelengths.rbegin() != wavelengths.rend());
		REQUIRE(wavelengths.crbegin() != wavelengths.crend());

		// Check forward iterator
		{
			auto wavelength = wavelengths.cbegin();

			REQUIRE(*(wavelength++) == 100);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 104);

			REQUIRE(wavelength == wavelengths.cend());
		}

		// Check reverse iterator
		{
			auto wavelength = wavelengths.crbegin();

			REQUIRE(*(wavelength++) == 104);
			REQUIRE(*(wavelength++) == 103);
			REQUIRE(*(wavelength++) == 102);
			REQUIRE(*(wavelength++) == 101);
			REQUIRE(*(wavelength++) == 100);

			REQUIRE(wavelength == wavelengths.crend());
		}
	}
}

#endif

