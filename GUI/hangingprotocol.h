#ifndef A_HANGINGPROTOCOL_H
#define A_HANGINGPROTOCOL_H

#include <QString>
#include <QList>
#include <QMap>

struct SeriesAssignment
{
	int viewport_index{0};
	QString description_match;  // regex to match series description
	double window_center{0.0};
	double window_width{0.0};
	bool use_default_wl{true};
};

struct HangingProtocol
{
	QString name;
	QString modality;           // e.g. "CT", "MR", "PT"
	QString body_part;          // e.g. "CHEST", "HEAD", "BREAST"
	QString description_regex;  // regex to match study description
	int priority{0};            // higher = more specific
	int grid_rows{1};
	int grid_cols{1};
	QList<SeriesAssignment> assignments;
};

class HangingProtocolEngine
{
public:
	static HangingProtocolEngine & instance();

	// Find best matching protocol for a study
	const HangingProtocol * match(
		const QString & modality,
		const QString & body_part,
		const QString & study_description,
		int num_series) const;

	// Get all protocols
	const QList<HangingProtocol> & protocols() const;

	// Add custom protocol
	void add(const HangingProtocol & protocol);

private:
	HangingProtocolEngine();
	void load_builtin();
	QList<HangingProtocol> m_protocols;
};

#endif
