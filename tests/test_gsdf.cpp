#include "catch.hpp"
#include "gsdf.h"
#include <cmath>
#include <vector>

// DICOM Part 14 Table B-1 reference values (luminance -> JND index)
// Selected known points from the GSDF standard.

TEST_CASE("GSDF JND index at known luminance values", "[gsdf]")
{
	// Per DICOM Part 14, approximate JND indices:
	// L=0.05 cd/m2 => JND ~1
	// L=1.0  cd/m2 => JND ~173
	// L=10   cd/m2 => JND ~358
	// L=100  cd/m2 => JND ~604
	// L=1000 cd/m2 => JND ~869
	// L=4000 cd/m2 => JND ~1023
	REQUIRE(gsdf::luminance_to_jnd(0.05)  == Approx(1.0).margin(5.0));
	REQUIRE(gsdf::luminance_to_jnd(1.0)   == Approx(173.0).margin(10.0));
	REQUIRE(gsdf::luminance_to_jnd(10.0)  == Approx(358.0).margin(10.0));
	REQUIRE(gsdf::luminance_to_jnd(100.0) == Approx(604.0).margin(10.0));
	REQUIRE(gsdf::luminance_to_jnd(1000.0) == Approx(869.0).margin(10.0));
	REQUIRE(gsdf::luminance_to_jnd(4000.0) == Approx(1023.0).margin(10.0));
}

TEST_CASE("GSDF LUT generation for typical monitor", "[gsdf]")
{
	const double Lmin = 0.5;
	const double Lmax = 500.0;
	const int lut_size = 256;

	std::vector<double> lut = gsdf::generate_gsdf_lut(Lmin, Lmax, lut_size);

	REQUIRE(lut.size() == static_cast<size_t>(lut_size));

	// First entry should map to Lmin region
	REQUIRE(lut[0] >= 0.0);
	REQUIRE(lut[0] <= 1.0);

	// Last entry should map to Lmax region
	REQUIRE(lut[lut_size - 1] >= 0.0);
	REQUIRE(lut[lut_size - 1] <= 1.0);
}

TEST_CASE("GSDF correction is monotonically increasing", "[gsdf]")
{
	const double Lmin = 0.5;
	const double Lmax = 500.0;
	const int lut_size = 256;

	std::vector<double> lut = gsdf::generate_gsdf_lut(Lmin, Lmax, lut_size);

	for (int i = 1; i < lut_size; ++i)
	{
		REQUIRE(lut[i] >= lut[i - 1]);
	}
}
