#include "annotationdb.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

static const QString conn_name = QStringLiteral("alizams_annotations");

AnnotationDB & AnnotationDB::instance()
{
	static AnnotationDB db;
	return db;
}

bool AnnotationDB::open(const QString & db_path)
{
	if (m_open) return true;
	QString path = db_path;
	if (path.isEmpty())
	{
		const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QDir().mkpath(dir);
		path = dir + QStringLiteral("/annotations.db");
	}
	QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), conn_name);
	db.setDatabaseName(path);
	if (!db.open()) return false;
	m_open = create_tables();
	if (!m_open) db.close();
	return m_open;
}

void AnnotationDB::close()
{
	if (!m_open) return;
	{
		QSqlDatabase db = QSqlDatabase::database(conn_name);
		if (db.isOpen()) db.close();
	}
	QSqlDatabase::removeDatabase(conn_name);
	m_open = false;
}

bool AnnotationDB::create_tables()
{
	QSqlQuery q(QSqlDatabase::database(conn_name));
	return q.exec(QStringLiteral(
		"CREATE TABLE IF NOT EXISTS annotations ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"study_uid TEXT,"
		"series_uid TEXT,"
		"instance_uid TEXT,"
		"frame_number INTEGER,"
		"type TEXT,"
		"data_json TEXT,"
		"created_at TEXT,"
		"modified_at TEXT)"));
}

bool AnnotationDB::save(Annotation & a)
{
	if (!m_open) return false;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
	if (a.id >= 0)
	{
		q.prepare(QStringLiteral(
			"UPDATE annotations SET study_uid=?,series_uid=?,instance_uid=?,"
			"frame_number=?,type=?,data_json=?,modified_at=? WHERE id=?"));
		q.addBindValue(a.study_uid);
		q.addBindValue(a.series_uid);
		q.addBindValue(a.instance_uid);
		q.addBindValue(a.frame_number);
		q.addBindValue(a.type);
		q.addBindValue(a.data_json);
		q.addBindValue(now);
		q.addBindValue(a.id);
		if (!q.exec()) return false;
		a.modified_at = now;
	}
	else
	{
		q.prepare(QStringLiteral(
			"INSERT INTO annotations (study_uid,series_uid,instance_uid,"
			"frame_number,type,data_json,created_at) VALUES (?,?,?,?,?,?,?)"));
		q.addBindValue(a.study_uid);
		q.addBindValue(a.series_uid);
		q.addBindValue(a.instance_uid);
		q.addBindValue(a.frame_number);
		q.addBindValue(a.type);
		q.addBindValue(a.data_json);
		q.addBindValue(now);
		if (!q.exec()) return false;
		a.id = q.lastInsertId().toLongLong();
		a.created_at = now;
	}
	return true;
}

bool AnnotationDB::remove(long long id)
{
	if (!m_open) return false;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	q.prepare(QStringLiteral("DELETE FROM annotations WHERE id=?"));
	q.addBindValue(id);
	return q.exec();
}

static Annotation row_to_annotation(QSqlQuery & q)
{
	Annotation a;
	a.id           = q.value(0).toLongLong();
	a.study_uid    = q.value(1).toString();
	a.series_uid   = q.value(2).toString();
	a.instance_uid = q.value(3).toString();
	a.frame_number = q.value(4).toInt();
	a.type         = q.value(5).toString();
	a.data_json    = q.value(6).toString();
	a.created_at   = q.value(7).toString();
	a.modified_at  = q.value(8).toString();
	return a;
}

QList<Annotation> AnnotationDB::find_by_instance(const QString & instance_uid)
{
	QList<Annotation> result;
	if (!m_open) return result;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	q.prepare(QStringLiteral("SELECT id,study_uid,series_uid,instance_uid,frame_number,type,data_json,created_at,modified_at FROM annotations WHERE instance_uid=?"));
	q.addBindValue(instance_uid);
	if (q.exec()) { while (q.next()) result.append(row_to_annotation(q)); }
	return result;
}

QList<Annotation> AnnotationDB::find_by_study(const QString & study_uid)
{
	QList<Annotation> result;
	if (!m_open) return result;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	q.prepare(QStringLiteral("SELECT id,study_uid,series_uid,instance_uid,frame_number,type,data_json,created_at,modified_at FROM annotations WHERE study_uid=?"));
	q.addBindValue(study_uid);
	if (q.exec()) { while (q.next()) result.append(row_to_annotation(q)); }
	return result;
}
