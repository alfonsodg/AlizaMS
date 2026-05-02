#ifndef A_AUTHMANAGER_H
#define A_AUTHMANAGER_H

#include <QString>
#include <QList>
#include <QDateTime>

enum class UserRole
{
	Viewer = 0,     // View images only
	Clinician = 1,  // View, measure, annotate, export
	Admin = 2       // Full access including settings and user management
};

struct User
{
	long long id{-1};
	QString username;
	QString password_hash;
	QString salt;
	UserRole role{UserRole::Viewer};
	bool active{true};
	QString created_at;
};

class AuthManager
{
public:
	static AuthManager & instance();

	// Initialize user database
	bool open(const QString & db_path = QString());
	void close();

	// Authentication
	bool authenticate(const QString & username, const QString & password);
	void logout();
	bool is_authenticated() const;

	// Current session
	QString current_user() const;
	UserRole current_role() const;
	bool has_permission(UserRole required) const;

	// User management (admin only)
	bool create_user(const QString & username, const QString & password, UserRole role);
	bool delete_user(const QString & username);
	QList<User> list_users() const;
	bool change_password(const QString & username, const QString & old_pw, const QString & new_pw);

	// Session timeout
	void set_timeout_minutes(int minutes);
	bool is_session_expired() const;
	void refresh_session();

private:
	AuthManager() = default;
	bool create_tables();
	QString hash_password(const QString & password, const QString & salt) const;
	QString generate_salt() const;
	void ensure_admin_exists();

	QString m_current_user;
	UserRole m_current_role{UserRole::Viewer};
	bool m_authenticated{false};
	QDateTime m_last_activity;
	int m_timeout_minutes{15};
	bool m_open{false};
};

#endif
