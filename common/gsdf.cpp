#include "gsdf.h"
#include <algorithm>

namespace GSDF
{

// DICOM Part 14 Table B-1 polynomial coefficients (Barten model)
static const double A = 71.498068;
static const double B = 94.593053;
static const double C = 41.912053;
static const double D =  9.8247004;
static const double E =  0.28175407;
static const double F = -1.1878455;
static const double G = -0.18014349;
static const double H =  0.14710899;

// State
static bool s_enabled = false;
static std::vector<unsigned char> s_lut(256);
static double s_lmin = 0.5;
static double s_lmax = 250.0;
static double s_ambient = 0.0;

double luminance_to_jnd(double luminance)
{
	if (luminance <= 0.0) luminance = 1e-10;
	const double ln_l = std::log(luminance);
	const double ln2 = ln_l * ln_l;
	const double ln3 = ln2 * ln_l;
	const double ln4 = ln3 * ln_l;
	const double ln5 = ln4 * ln_l;
	const double ln6 = ln5 * ln_l;
	const double ln7 = ln6 * ln_l;
	return A + B*ln_l + C*ln2 + D*ln3 + E*ln4 + F*ln5 + G*ln6 + H*ln7;
}

double jnd_to_luminance(double jnd_index)
{
	// Newton-Raphson: find L such that luminance_to_jnd(L) = jnd_index
	// Initial guess via linear interpolation in log domain
	// DICOM range: ~0.05 cd/m² (JND~1) to ~4000 cd/m² (JND~1023)
	double L = 1.0;
	for (int i = 0; i < 50; ++i)
	{
		if (L <= 0.0) L = 1e-10;
		const double ln_l = std::log(L);
		const double ln2 = ln_l * ln_l;
		const double ln3 = ln2 * ln_l;
		const double ln4 = ln3 * ln_l;
		const double ln5 = ln4 * ln_l;
		const double ln6 = ln5 * ln_l;
		const double ln7 = ln6 * ln_l;
		const double f = A + B*ln_l + C*ln2 + D*ln3 + E*ln4 + F*ln5 + G*ln6 + H*ln7 - jnd_index;
		// Derivative: df/dL = (1/L) * (B + 2C*ln + 3D*ln² + 4E*ln³ + 5F*ln⁴ + 6G*ln⁵ + 7H*ln⁶)
		const double df = (1.0 / L) * (B + 2.0*C*ln_l + 3.0*D*ln2 + 4.0*E*ln3 + 5.0*F*ln4 + 6.0*G*ln5 + 7.0*H*ln6);
		if (std::fabs(df) < 1e-20) break;
		const double L_new = L - f / df;
		if (L_new <= 0.0)
		{
			L *= 0.5;
			continue;
		}
		if (std::fabs(L_new - L) < 1e-10 * L) { L = L_new; break; }
		L = L_new;
	}
	return L;
}

std::vector<unsigned char> generate_gsdf_lut(
    double lmin, double lmax, double ambient, size_t num_entries)
{
	std::vector<unsigned char> lut(num_entries);
	if (num_entries < 2 || lmax <= lmin)
	{
		for (size_t i = 0; i < num_entries; ++i)
			lut[i] = static_cast<unsigned char>(i < 256 ? i : 255);
		return lut;
	}
	const double jnd_min = luminance_to_jnd(lmin + ambient);
	const double jnd_max = luminance_to_jnd(lmax + ambient);
	const double range = static_cast<double>(num_entries - 1);
	const double l_range = lmax - lmin;
	for (size_t i = 0; i < num_entries; ++i)
	{
		const double jnd_target = jnd_min + (jnd_max - jnd_min) * static_cast<double>(i) / range;
		const double l_target = jnd_to_luminance(jnd_target) - ambient;
		double ddl = (l_target - lmin) / l_range * range;
		if (ddl < 0.0) ddl = 0.0;
		if (ddl > range) ddl = range;
		lut[i] = static_cast<unsigned char>(ddl + 0.5);
	}
	return lut;
}

bool is_enabled()
{
	return s_enabled;
}

void configure(double lmin, double lmax, double ambient)
{
	s_lmin = lmin;
	s_lmax = lmax;
	s_ambient = ambient;
	s_lut = generate_gsdf_lut(lmin, lmax, ambient, 256);
	s_enabled = true;
}

unsigned char apply(unsigned char input)
{
	if (!s_enabled) return input;
	return s_lut[input];
}

void disable()
{
	s_enabled = false;
}

} // namespace GSDF
