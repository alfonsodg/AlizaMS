#ifndef A_AUDITLOG_H
#define A_AUDITLOG_H

#include <QString>
#include <QDateTime>

enum class AuditEvent
{
	APP_START,
	APP_STOP,
	STUDY_OPENED,
	STUDY_CLOSED,
	SERIES_VIEWED,
	MEASUREMENT_CREATED,
	ANNOTATION_CREATED,
	ANNOTATION_MODIFIED,
	ANNOTATION_DELETED,
	IMAGE_EXPORTED,
	ANONYMIZATION_STARTED,
	ANONYMIZATION_COMPLETED,
	SETTINGS_CHANGED,
	AUTH_LOGIN,
	AUTH_LOGOUT,
	AUTH_FAILED
};

class AuditLog
{
public:
	static AuditLog & instance();

	// Initialize logging to file
	bool open(const QString & log_dir = QString());
	void close();

	// Log an audit event
	void log(AuditEvent event, const QString & details = QString());

	// Log with patient context
	void log(AuditEvent event, const QString & patient_id,
	         const QString & study_uid, const QString & details = QString());

	// Set current user (from auth module)
	void set_user(const QString & username);

	// Get log file path
	QString log_path() const;

private:
	AuditLog() = default;
	QString event_name(AuditEvent event) const;
	void rotate_if_needed();
	void write_entry(const QString & json_line);

	QString m_log_dir;
	QString m_current_file;
	QString m_username{"local_user"};
	bool m_open{false};
	static constexpr long long MAX_LOG_SIZE = 10 * 1024 * 1024; // 10MB
};

#endif
