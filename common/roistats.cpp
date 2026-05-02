#include "roistats.h"

namespace ROIStats
{

ROIStatistics calculate(const std::vector<double> & values, double pixel_area_mm2)
{
	ROIStatistics s;
	if (values.empty())
		return s;
	s.pixel_count = values.size();
	s.area_mm2 = static_cast<double>(s.pixel_count) * pixel_area_mm2;
	s.mean = std::accumulate(values.begin(), values.end(), 0.0) / static_cast<double>(s.pixel_count);
	double sum_sq = 0.0;
	for (const double v : values)
	{
		const double d = v - s.mean;
		sum_sq += d * d;
	}
	s.std_dev = std::sqrt(sum_sq / static_cast<double>(s.pixel_count));
	s.min_val = *std::min_element(values.begin(), values.end());
	s.max_val = *std::max_element(values.begin(), values.end());
	std::vector<double> sorted(values);
	std::sort(sorted.begin(), sorted.end());
	const size_t n = sorted.size();
	s.median = (n % 2 != 0) ? sorted[n / 2] : (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
	s.valid = true;
	return s;
}

QString format(const ROIStatistics & stats, const QString & unit)
{
	if (!stats.valid)
		return QString("No data");
	const QString u = unit.isEmpty() ? QString() : (QString(" ") + unit);
	return QString(
		"Mean: %1%6\n"
		"Std Dev: %2%6\n"
		"Min: %3%6\n"
		"Max: %4%6\n"
		"Median: %5%6\n"
		"Pixels: %7\n"
		"Area: %8 mm\u00B2")
		.arg(QString::number(stats.mean, 'f', 2))
		.arg(QString::number(stats.std_dev, 'f', 2))
		.arg(QString::number(stats.min_val, 'f', 2))
		.arg(QString::number(stats.max_val, 'f', 2))
		.arg(QString::number(stats.median, 'f', 2))
		.arg(u)
		.arg(stats.pixel_count)
		.arg(QString::number(stats.area_mm2, 'f', 2));
}

} // namespace ROIStats
