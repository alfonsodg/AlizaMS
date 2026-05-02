#include "authmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QCryptographicHash>
#include <QRandomGenerator>

static const QString conn_name = QStringLiteral("alizams_auth");

AuthManager & AuthManager::instance()
{
	static AuthManager mgr;
	return mgr;
}

bool AuthManager::open(const QString & db_path)
{
	if (m_open) return true;
	QString path = db_path;
	if (path.isEmpty())
	{
		const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QDir().mkpath(dir);
		path = dir + QStringLiteral("/users.db");
	}
	QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), conn_name);
	db.setDatabaseName(path);
	if (!db.open()) return false;
	m_open = create_tables();
	if (!m_open)
	{
		db.close();
		return false;
	}
	ensure_admin_exists();
	return true;
}

void AuthManager::close()
{
	if (!m_open) return;
	logout();
	{
		QSqlDatabase db = QSqlDatabase::database(conn_name);
		if (db.isOpen()) db.close();
	}
	QSqlDatabase::removeDatabase(conn_name);
	m_open = false;
}

bool AuthManager::create_tables()
{
	QSqlQuery q(QSqlDatabase::database(conn_name));
	return q.exec(QStringLiteral(
		"CREATE TABLE IF NOT EXISTS users ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"username TEXT UNIQUE NOT NULL,"
		"password_hash TEXT NOT NULL,"
		"salt TEXT NOT NULL,"
		"role INTEGER NOT NULL,"
		"active INTEGER DEFAULT 1,"
		"created_at TEXT)"));
}

QString AuthManager::hash_password(const QString & password, const QString & salt) const
{
	QCryptographicHash h(QCryptographicHash::Sha256);
	h.addData((salt + password).toUtf8());
	return QString::fromLatin1(h.result().toHex());
}

QString AuthManager::generate_salt() const
{
	QByteArray bytes(16, '\0');
	QRandomGenerator * rng = QRandomGenerator::global();
	for (int i = 0; i < 16; ++i)
		bytes[i] = static_cast<char>(rng->bounded(256));
	return QString::fromLatin1(bytes.toHex());
}

bool AuthManager::authenticate(const QString & username, const QString & password)
{
	if (!m_open) return false;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	q.prepare(QStringLiteral("SELECT password_hash,salt,role FROM users WHERE username=? AND active=1"));
	q.addBindValue(username);
	if (!q.exec() || !q.next()) return false;
	const QString stored_hash = q.value(0).toString();
	const QString salt = q.value(1).toString();
	const int role = q.value(2).toInt();
	if (hash_password(password, salt) != stored_hash) return false;
	m_current_user = username;
	m_current_role = static_cast<UserRole>(role);
	m_authenticated = true;
	m_last_activity = QDateTime::currentDateTime();
	return true;
}

void AuthManager::logout()
{
	m_current_user.clear();
	m_current_role = UserRole::Viewer;
	m_authenticated = false;
}

bool AuthManager::is_authenticated() const
{
	return m_authenticated && !is_session_expired();
}

QString AuthManager::current_user() const
{
	return m_current_user;
}

UserRole AuthManager::current_role() const
{
	return m_current_role;
}

bool AuthManager::has_permission(UserRole required) const
{
	return is_authenticated() && static_cast<int>(m_current_role) >= static_cast<int>(required);
}

bool AuthManager::create_user(const QString & username, const QString & password, UserRole role)
{
	if (!m_open) return false;
	const QString salt = generate_salt();
	const QString pw_hash = hash_password(password, salt);
	const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
	QSqlQuery q(QSqlDatabase::database(conn_name));
	q.prepare(QStringLiteral(
		"INSERT INTO users (username,password_hash,salt,role,active,created_at) VALUES (?,?,?,?,1,?)"));
	q.addBindValue(username);
	q.addBindValue(pw_hash);
	q.addBindValue(salt);
	q.addBindValue(static_cast<int>(role));
	q.addBindValue(now);
	return q.exec();
}

bool AuthManager::delete_user(const QString & username)
{
	if (!m_open) return false;
	// Cannot delete self
	if (username == m_current_user) return false;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	q.prepare(QStringLiteral("DELETE FROM users WHERE username=?"));
	q.addBindValue(username);
	return q.exec() && q.numRowsAffected() > 0;
}

QList<User> AuthManager::list_users() const
{
	QList<User> result;
	if (!m_open) return result;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	if (!q.exec(QStringLiteral("SELECT id,username,role,active,created_at FROM users")))
		return result;
	while (q.next())
	{
		User u;
		u.id = q.value(0).toLongLong();
		u.username = q.value(1).toString();
		u.role = static_cast<UserRole>(q.value(2).toInt());
		u.active = q.value(3).toBool();
		u.created_at = q.value(4).toString();
		result.append(u);
	}
	return result;
}

bool AuthManager::change_password(const QString & username, const QString & old_pw, const QString & new_pw)
{
	if (!m_open) return false;
	QSqlQuery q(QSqlDatabase::database(conn_name));
	q.prepare(QStringLiteral("SELECT password_hash,salt FROM users WHERE username=?"));
	q.addBindValue(username);
	if (!q.exec() || !q.next()) return false;
	const QString stored_hash = q.value(0).toString();
	const QString old_salt = q.value(1).toString();
	if (hash_password(old_pw, old_salt) != stored_hash) return false;
	const QString new_salt = generate_salt();
	const QString new_hash = hash_password(new_pw, new_salt);
	q.prepare(QStringLiteral("UPDATE users SET password_hash=?,salt=? WHERE username=?"));
	q.addBindValue(new_hash);
	q.addBindValue(new_salt);
	q.addBindValue(username);
	return q.exec();
}

void AuthManager::ensure_admin_exists()
{
	QSqlQuery q(QSqlDatabase::database(conn_name));
	if (!q.exec(QStringLiteral("SELECT COUNT(*) FROM users")) || !q.next()) return;
	if (q.value(0).toInt() > 0) return;
	create_user(QStringLiteral("admin"), QStringLiteral("admin"), UserRole::Admin);
}

void AuthManager::set_timeout_minutes(int minutes)
{
	m_timeout_minutes = minutes;
}

bool AuthManager::is_session_expired() const
{
	if (!m_authenticated) return true;
	return m_last_activity.addSecs(m_timeout_minutes * 60) < QDateTime::currentDateTime();
}

void AuthManager::refresh_session()
{
	m_last_activity = QDateTime::currentDateTime();
}
