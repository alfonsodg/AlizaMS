#ifndef A_ANNOTATIONDB_H
#define A_ANNOTATIONDB_H

#include <QString>
#include <QList>
#include <QVariant>

struct Annotation
{
	long long id{-1};
	QString study_uid;
	QString series_uid;
	QString instance_uid;
	int frame_number{0};
	QString type;           // "distance", "ellipse_roi", "rect_roi", "freehand_roi", "text"
	QString data_json;      // JSON with coordinates, value, unit
	QString created_at;
	QString modified_at;
};

class AnnotationDB
{
public:
	static AnnotationDB & instance();
	bool open(const QString & db_path = QString());
	void close();
	bool save(Annotation & a);
	bool remove(long long id);
	QList<Annotation> find_by_instance(const QString & instance_uid);
	QList<Annotation> find_by_study(const QString & study_uid);
private:
	AnnotationDB() = default;
	bool create_tables();
	bool m_open{false};
};

#endif
