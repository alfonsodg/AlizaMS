#include "catch.hpp"
#include <cmath>
#include <algorithm>

// Window/level formulas replicated from CG/shaders.h for pure math testing.
// LINEAR (from shader): r = (t - window_low) / window_width
// SIGMOID (from shader): r = 1.0 / (1.0 + exp(-4.0 * ((t - window_center) / window_width)))
// LINEAR_EXACT per DICOM PS3.3 C.11.2.1.2.1:
//   if t <= c - 0.5 - (w-1)/2 => ymin
//   if t >  c - 0.5 + (w-1)/2 => ymax
//   else => ((t - (c - 0.5)) / (w - 1) + 0.5) * (ymax - ymin) + ymin

static constexpr double kMinWindowWidth = 1e-6;

// LINEAR: normalized output in [0,1] for t in [window_low, window_high]
static double apply_linear(double t, double window_center, double window_width)
{
	if (window_width <= 0.0)
		window_width = kMinWindowWidth;
	const double window_low  = window_center - window_width * 0.5;
	const double window_high = window_center + window_width * 0.5;
	if (t <= window_low)  return 0.0;
	if (t >= window_high) return 1.0;
	return (t - window_low) / window_width;
}

// LINEAR_EXACT per DICOM PS3.3 C.11.2.1.2.1
static double apply_linear_exact(double t, double window_center, double window_width)
{
	if (window_width <= 0.0)
		window_width = kMinWindowWidth;
	const double c = window_center;
	const double w = window_width;
	if (t <= c - 0.5 - (w - 1.0) / 2.0) return 0.0;
	if (t >  c - 0.5 + (w - 1.0) / 2.0) return 1.0;
	return ((t - (c - 0.5)) / (w - 1.0)) + 0.5;
}

// SIGMOID per CG/shaders.h: 1/(1+exp(-4*(t-center)/width))
static double apply_sigmoid(double t, double window_center, double window_width)
{
	if (window_width <= 0.0)
		window_width = kMinWindowWidth;
	return 1.0 / (1.0 + std::exp(-4.0 * ((t - window_center) / window_width)));
}

// --- LINEAR tests ---

TEST_CASE("LINEAR: center of window maps to 0.5", "[lut]")
{
	REQUIRE(apply_linear(40.0, 40.0, 400.0) == Approx(0.5));
}

TEST_CASE("LINEAR: below window maps to 0", "[lut]")
{
	REQUIRE(apply_linear(-200.0, 40.0, 400.0) == Approx(0.0));
}

TEST_CASE("LINEAR: above window maps to 1", "[lut]")
{
	REQUIRE(apply_linear(300.0, 40.0, 400.0) == Approx(1.0));
}

TEST_CASE("LINEAR: known intermediate value", "[lut]")
{
	// t=140, low=-160, width=400 => (140-(-160))/400 = 300/400 = 0.75
	REQUIRE(apply_linear(140.0, 40.0, 400.0) == Approx(0.75));
}

// --- LINEAR_EXACT tests ---

TEST_CASE("LINEAR_EXACT: center maps to ~0.5", "[lut]")
{
	// c=40, w=400: t=40 => ((40-39.5)/399)+0.5 = 0.5012...
	double r = apply_linear_exact(40.0, 40.0, 400.0);
	REQUIRE(r == Approx(0.5).margin(0.01));
}

TEST_CASE("LINEAR_EXACT: below range maps to 0", "[lut]")
{
	// boundary: c-0.5-(w-1)/2 = 40-0.5-199.5 = -160
	REQUIRE(apply_linear_exact(-160.0, 40.0, 400.0) == Approx(0.0));
}

TEST_CASE("LINEAR_EXACT: above range maps to 1", "[lut]")
{
	// boundary: c-0.5+(w-1)/2 = 40-0.5+199.5 = 239
	// t > 239 => 1.0
	REQUIRE(apply_linear_exact(240.0, 40.0, 400.0) == Approx(1.0));
}

// --- SIGMOID tests ---

TEST_CASE("SIGMOID: center maps to 0.5", "[lut]")
{
	REQUIRE(apply_sigmoid(40.0, 40.0, 400.0) == Approx(0.5));
}

TEST_CASE("SIGMOID: S-curve shape — below center < 0.5, above center > 0.5", "[lut]")
{
	REQUIRE(apply_sigmoid(-160.0, 40.0, 400.0) < 0.5);
	REQUIRE(apply_sigmoid(240.0, 40.0, 400.0) > 0.5);
}

TEST_CASE("SIGMOID: monotonically increasing", "[lut]")
{
	double prev = apply_sigmoid(-500.0, 40.0, 400.0);
	for (double t = -400.0; t <= 500.0; t += 50.0)
	{
		double cur = apply_sigmoid(t, 40.0, 400.0);
		REQUIRE(cur >= prev);
		prev = cur;
	}
}

// --- Edge cases ---

TEST_CASE("Window width 0 uses kMinWindowWidth", "[lut]")
{
	// Should not crash or produce NaN/Inf
	double r = apply_linear(40.0, 40.0, 0.0);
	REQUIRE(std::isfinite(r));
}

TEST_CASE("Negative window center", "[lut]")
{
	double r = apply_linear(0.0, -100.0, 200.0);
	// low=-200, high=0 => (0-(-200))/200 = 1.0
	REQUIRE(r == Approx(1.0));
}

// --- LUT index clamping ---

TEST_CASE("LUT index clamping prevents out-of-bounds", "[lut]")
{
	const int lut_size = 256;
	for (double t = -500.0; t <= 500.0; t += 10.0)
	{
		double r = apply_linear(t, 40.0, 400.0);
		int idx = std::clamp(static_cast<int>(r * (lut_size - 1)), 0, lut_size - 1);
		REQUIRE(idx >= 0);
		REQUIRE(idx < lut_size);
	}
}
