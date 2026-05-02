#ifndef A_GSDF_H
#define A_GSDF_H

#include <vector>
#include <cmath>
#include <cstddef>

// DICOM Part 14 Grayscale Standard Display Function
// Maps digital driving levels to perceptually linear luminance
namespace GSDF
{

// Calculate JND (Just Noticeable Difference) index from luminance (cd/m²)
// Based on Barten model per DICOM PS3.14
double luminance_to_jnd(double luminance);

// Calculate luminance from JND index
double jnd_to_luminance(double jnd_index);

// Generate GSDF correction LUT
// Maps input DDL [0..num_entries-1] to corrected DDL [0..num_entries-1]
// lmin: minimum display luminance (cd/m²), typically 0.5-1.0
// lmax: maximum display luminance (cd/m²), typically 250-500
// ambient: ambient light contribution (cd/m²), typically 0-10
std::vector<unsigned char> generate_gsdf_lut(
    double lmin, double lmax, double ambient, size_t num_entries = 256);

// Check if GSDF correction is enabled
bool is_enabled();

// Set GSDF parameters and regenerate LUT
void configure(double lmin, double lmax, double ambient);

// Apply GSDF correction to a single value [0..255] -> [0..255]
unsigned char apply(unsigned char input);

// Disable GSDF correction (pass-through)
void disable();

} // namespace GSDF

#endif
