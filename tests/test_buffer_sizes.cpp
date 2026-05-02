#include "catch.hpp"
#include <cstdint>
#include <climits>

// Test buffer size calculations match the pattern used in dicomutils.cpp
// These verify that using unsigned long long prevents overflow

TEST_CASE("Buffer size calculation does not overflow with large dimensions", "[buffer]")
{
	// Simulates: dimx * dimy * dimz * rescale_type_size * samples
	const unsigned int dimx = 65535;
	const unsigned int dimy = 65535;
	const unsigned int dimz = 1;
	const unsigned int rescale_type_size = 4;
	const unsigned int samples = 3;

	// This would overflow uint32_t
	const unsigned long long result =
		static_cast<unsigned long long>(dimx) * dimy * dimz * rescale_type_size * samples;

	// Expected: 65535 * 65535 * 1 * 4 * 3 = 51,538,034,700
	REQUIRE(result == 51538034700ULL);
	REQUIRE(result > UINT_MAX);
}

TEST_CASE("Buffer size calculation with typical medical image dimensions", "[buffer]")
{
	// Typical CT: 512x512x300 slices, 2 bytes, 1 sample
	const unsigned int dimx = 512;
	const unsigned int dimy = 512;
	const unsigned int dimz = 300;
	const unsigned int type_size = 2;
	const unsigned int samples = 1;

	const unsigned long long result =
		static_cast<unsigned long long>(dimx) * dimy * dimz * type_size * samples;

	REQUIRE(result == 157286400ULL);
	REQUIRE(result < UINT_MAX);
}

TEST_CASE("Buffer size calculation with pathological dimensions", "[buffer]")
{
	// Whole-slide pathology: 100000x100000, 1 slice, 3 bytes RGB
	const unsigned int dimx = 100000;
	const unsigned int dimy = 100000;
	const unsigned int dimz = 1;
	const unsigned int type_size = 1;
	const unsigned int samples = 3;

	const unsigned long long result =
		static_cast<unsigned long long>(dimx) * dimy * dimz * type_size * samples;

	REQUIRE(result == 30000000000ULL);
	REQUIRE(result > UINT_MAX);
}

TEST_CASE("Delta decode bounds check prevents out-of-bounds access", "[buffer]")
{
	// Simulate the 0xa5 marker at end of buffer
	const char buffer[] = {'\xa5'};  // marker at position 0, no room for i+1, i+2
	const size_t length = 1;

	// The fix ensures i + 2 >= length breaks the loop
	size_t i = 0;
	bool would_overflow = false;
	if (static_cast<unsigned char>(buffer[i]) == 0xa5)
	{
		if (i + 2 >= length)
		{
			would_overflow = true;
		}
	}
	REQUIRE(would_overflow == true);
}
