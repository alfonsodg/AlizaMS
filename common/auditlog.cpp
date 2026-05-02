#include "auditlog.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>

AuditLog & AuditLog::instance()
{
	static AuditLog s;
	return s;
}

bool AuditLog::open(const QString & log_dir)
{
	if (log_dir.isEmpty())
	{
		m_log_dir = QStandardPaths::writableLocation(
			QStandardPaths::AppDataLocation) + QString("/audit");
	}
	else
	{
		m_log_dir = log_dir;
	}
	QDir d(m_log_dir);
	if (!d.exists() && !d.mkpath(QString(".")))
	{
		return false;
	}
	m_current_file = m_log_dir + QString("/audit_") +
		QDate::currentDate().toString(QString("yyyy-MM-dd")) +
		QString(".jsonl");
	m_open = true;
	return true;
}

void AuditLog::close()
{
	m_open = false;
}

void AuditLog::log(AuditEvent event, const QString & details)
{
	if (!m_open) return;
	QJsonObject obj;
	obj[QString("ts")]    = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
	obj[QString("event")] = event_name(event);
	obj[QString("user")]  = m_username;
	if (!details.isEmpty())
	{
		obj[QString("details")] = details;
	}
	write_entry(QString::fromUtf8(
		QJsonDocument(obj).toJson(QJsonDocument::Compact)));
}

void AuditLog::log(
	AuditEvent event,
	const QString & patient_id,
	const QString & study_uid,
	const QString & details)
{
	if (!m_open) return;
	QJsonObject obj;
	obj[QString("ts")]         = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
	obj[QString("event")]      = event_name(event);
	obj[QString("user")]       = m_username;
	obj[QString("patient_id")] = patient_id;
	obj[QString("study_uid")]  = study_uid;
	if (!details.isEmpty())
	{
		obj[QString("details")] = details;
	}
	write_entry(QString::fromUtf8(
		QJsonDocument(obj).toJson(QJsonDocument::Compact)));
}

void AuditLog::set_user(const QString & username)
{
	m_username = username;
}

QString AuditLog::log_path() const
{
	return m_current_file;
}

QString AuditLog::event_name(AuditEvent event) const
{
	switch (event)
	{
	case AuditEvent::APP_START:                return QString("app_start");
	case AuditEvent::APP_STOP:                 return QString("app_stop");
	case AuditEvent::STUDY_OPENED:             return QString("study_opened");
	case AuditEvent::STUDY_CLOSED:             return QString("study_closed");
	case AuditEvent::SERIES_VIEWED:            return QString("series_viewed");
	case AuditEvent::MEASUREMENT_CREATED:      return QString("measurement_created");
	case AuditEvent::ANNOTATION_CREATED:       return QString("annotation_created");
	case AuditEvent::ANNOTATION_MODIFIED:      return QString("annotation_modified");
	case AuditEvent::ANNOTATION_DELETED:       return QString("annotation_deleted");
	case AuditEvent::IMAGE_EXPORTED:           return QString("image_exported");
	case AuditEvent::ANONYMIZATION_STARTED:    return QString("anonymization_started");
	case AuditEvent::ANONYMIZATION_COMPLETED:  return QString("anonymization_completed");
	case AuditEvent::SETTINGS_CHANGED:         return QString("settings_changed");
	case AuditEvent::AUTH_LOGIN:               return QString("auth_login");
	case AuditEvent::AUTH_LOGOUT:              return QString("auth_logout");
	case AuditEvent::AUTH_FAILED:              return QString("auth_failed");
	}
	return QString("unknown");
}

void AuditLog::rotate_if_needed()
{
	const QFileInfo fi(m_current_file);
	if (!fi.exists() || fi.size() < MAX_LOG_SIZE)
	{
		return;
	}
	// Find next available suffix
	int n = 1;
	QString base = m_current_file;
	base.chop(6); // remove ".jsonl"
	QString next;
	do
	{
		next = base + QString("_%1.jsonl").arg(n);
		++n;
	}
	while (QFileInfo::exists(next));
	m_current_file = next;
}

void AuditLog::write_entry(const QString & json_line)
{
	rotate_if_needed();
	QFile f(m_current_file);
	if (!f.open(QIODevice::Append | QIODevice::Text))
	{
		return;
	}
	f.write(json_line.toUtf8());
	f.write("\n");
	f.close();
}
