#ifndef A_SUVUTILS_H
#define A_SUVUTILS_H

#include <QString>

// SUV calculation parameters extracted from DICOM PET images
struct SUVParams
{
	bool valid{false};
	double patient_weight_kg{0.0};       // (0010,1030) Patient's Weight
	double radionuclide_total_dose{0.0}; // (0018,1074) Radionuclide Total Dose (Bq)
	double half_life{0.0};               // (0018,1075) Radionuclide Half Life (s)
	double scan_datetime{0.0};           // Series Date/Time as seconds since epoch
	double injection_datetime{0.0};      // (0018,1078) Radiopharmaceutical Start DateTime
	double rescale_slope{1.0};           // (0028,1053)
	double rescale_intercept{0.0};       // (0028,1052)
	QString units;                       // (0054,1001) Units (BQML, CNTS, etc.)
};

namespace SUVUtils
{

// Calculate decay correction factor
double decay_correction(double half_life, double scan_time, double injection_time);

// Convert raw pixel value to SUVbw (body weight)
double pixel_to_suvbw(double pixel_value, const SUVParams & params);

// Convert raw pixel value to activity concentration (Bq/ml)
double pixel_to_activity(double pixel_value, const SUVParams & params);

// Format SUV value for display
QString format_suv(double suv_value);

} // namespace SUVUtils

#endif
