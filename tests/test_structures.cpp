#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// Minimal includes for testing structures without Qt/ITK dependencies
#include <vector>
#include <cstring>

// Test SliceOverlay copy and assignment behavior
// Replicated minimal struct to avoid Qt dependency
struct TestSliceOverlay
{
	int dimx{};
	int dimy{};
	int x{};
	int y{};
	std::vector<char> data;

	TestSliceOverlay() = default;

	TestSliceOverlay(const TestSliceOverlay & j)
		: dimx(j.dimx), dimy(j.dimy), x(j.x), y(j.y), data(j.data) {}

	TestSliceOverlay & operator=(const TestSliceOverlay & j)
	{
		if (this == &j) return *this;
		dimx = j.dimx;
		dimy = j.dimy;
		x = j.x;
		y = j.y;
		data = j.data;
		return *this;
	}
};

TEST_CASE("SliceOverlay copy constructor copies data correctly", "[structures]")
{
	TestSliceOverlay a;
	a.dimx = 512;
	a.dimy = 512;
	a.x = 10;
	a.y = 20;
	a.data = {1, 2, 3, 4, 5};

	TestSliceOverlay b(a);
	REQUIRE(b.dimx == 512);
	REQUIRE(b.dimy == 512);
	REQUIRE(b.data.size() == 5);
	REQUIRE(b.data[0] == 1);
	REQUIRE(b.data[4] == 5);
}

TEST_CASE("SliceOverlay assignment replaces data, not appends", "[structures]")
{
	TestSliceOverlay a;
	a.data = {1, 2, 3};

	TestSliceOverlay b;
	b.data = {10, 20};

	b = a;
	REQUIRE(b.data.size() == 3);
	REQUIRE(b.data[0] == 1);

	// Assign again — should still be size 3, not 6
	b = a;
	REQUIRE(b.data.size() == 3);
}

TEST_CASE("SliceOverlay self-assignment is safe", "[structures]")
{
	TestSliceOverlay a;
	a.dimx = 100;
	a.data = {1, 2, 3};

	a = a;
	REQUIRE(a.dimx == 100);
	REQUIRE(a.data.size() == 3);
}
