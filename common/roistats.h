#ifndef A_ROISTATS_H
#define A_ROISTATS_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <QString>

struct ROIStatistics
{
	double mean{0.0};
	double std_dev{0.0};
	double min_val{0.0};
	double max_val{0.0};
	double median{0.0};
	double area_mm2{0.0};
	size_t pixel_count{0};
	bool valid{false};
};

namespace ROIStats
{

// Calculate statistics from a vector of pixel values
ROIStatistics calculate(const std::vector<double> & values, double pixel_area_mm2 = 1.0);

// Format statistics for display
QString format(const ROIStatistics & stats, const QString & unit = QString());

} // namespace ROIStats

#endif
