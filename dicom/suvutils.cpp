#include "suvutils.h"
#include <cmath>
#include <QString>

namespace SUVUtils
{

double decay_correction(double half_life, double scan_time, double injection_time)
{
	if (half_life <= 0.0)
		return 1.0;
	const double dt = scan_time - injection_time;
	return std::exp(-std::log(2.0) * dt / half_life);
}

double pixel_to_suvbw(double pixel_value, const SUVParams & params)
{
	if (!params.valid || params.radionuclide_total_dose <= 0.0 || params.patient_weight_kg <= 0.0)
		return 0.0;
	const double activity = pixel_value * params.rescale_slope + params.rescale_intercept;
	const double decay = decay_correction(params.half_life, params.scan_datetime, params.injection_datetime);
	if (decay <= 0.0)
		return 0.0;
	// Weight in grams (1 kg = 1000 g) to match Bq/ml units
	return activity * (params.patient_weight_kg * 1000.0) / (params.radionuclide_total_dose * decay);
}

double pixel_to_activity(double pixel_value, const SUVParams & params)
{
	return pixel_value * params.rescale_slope + params.rescale_intercept;
}

QString format_suv(double suv_value)
{
	return QString::number(suv_value, 'f', 2);
}

} // namespace SUVUtils
