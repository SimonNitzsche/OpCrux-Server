#pragma once
#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

// TODO: Figure out Linux solution
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <list>
#include "libbcrypt/include/bcrypt/BCrypt.hpp"
#include "odbc/Connection.h"
#include "odbc/Environment.h"
#include "odbc/Exception.h"
#include "odbc/PreparedStatement.h"
#include "odbc/ResultSet.h"
#include <Utils/Logger.hpp>

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

class DBInterface {
public:

private:
	//define handles and variables

	inline static odbc::EnvironmentRef env = odbc::Environment::create();
	inline static odbc::ConnectionRef conn;

	inline static std::string DB_dbDriver;
	inline static std::string DB_dbHost;
	inline static std::string DB_dbUser;
	inline static std::string DB_dbPass;
public:

	static void Setup(std::string a, std::string b, std::string c, std::string d) {
		DB_dbDriver = a;
		DB_dbHost = b;
		DB_dbUser = c;
		DB_dbPass = d;
	}

	static inline odbc::PreparedStatementRef safelyPrepareStmt(odbc::ConnectionRef conn, const char* sql) {
#ifndef _DEBUG
		//Logger::log("DATABASE", sql);
#endif
		return conn->prepareStatement(sql);
	}

	static void Connect() {
		Logger::log("DATABASE", "Attempting connection to SQL Server...");

		std::string connStrBuilder = \
			"DRIVER={"\
			+ DB_dbDriver\
			+ "};SERVER="\
			+ DB_dbHost\
			+ ";UID="\
			+ DB_dbUser\
			+ ";PWD="\
			+ DB_dbPass\
			+ ";";

		conn = env->createConnection();
		conn->connect(connStrBuilder.c_str());
		conn->setAutoCommit(true);
	}

	static void Disconnect() {
		Logger::log("DATABASE", "WARNING!!!! DATABASE HAS BEEN UNLOADED!", LogType::ERR);

		conn->disconnect();
	}

	static bool IsLoginCorrect(std::string_view& s_username, std::string_view& s_password) {
		std::string h_password = BCrypt::generateHash(std::string(s_password.data(), s_password.length()));

		odbc::PreparedStatementRef stmt = safelyPrepareStmt(conn, "SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE username=?");
		stmt->setString(1, std::string(s_username.data(), s_username.length()));
		odbc::ResultSetRef rs = stmt->executeQuery();
		if (rs->next()) {
			std::string db_hash = *rs->getString(1);
			std::string dbhash = "DB_HASH: " + db_hash;
			std::string pkhash = "PK_HASH: " + h_password;
			Logger::log("DATABASE", dbhash);
			Logger::log("DATABASE", pkhash);

			return BCrypt::validatePassword(std::string(s_password.data(), s_password.length()), db_hash);
		}

		return false;
	}

	static bool IsLoginCorrectEmail(std::string_view& s_email, std::string_view& s_password) {
		std::string h_password = BCrypt::generateHash(std::string(s_password.data(), s_password.length()));

		odbc::PreparedStatementRef stmt = safelyPrepareStmt(conn, "SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE email=?");
		stmt->setString(1, std::string(s_email.data(), s_email.length()));
		odbc::ResultSetRef rs = stmt->executeQuery();
		if (rs->next()) {
			std::string db_hash = *rs->getString(1);
			std::string dbhash = "DB_HASH: " + db_hash;
			std::string pkhash = "PK_HASH: " + h_password;
			Logger::log("DATABASE", dbhash);
			Logger::log("DATABASE", pkhash);

			return BCrypt::validatePassword(std::string(s_password.data(), s_password.length()), db_hash);
		}

		return false;
	}

	static bool UserExists(std::string_view& s_username, std::string_view& s_email) {

		odbc::PreparedStatementRef stmt = safelyPrepareStmt(conn, "SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE username=? OR email=?");
		stmt->setString(1, std::string(s_username.data(), s_username.length()));
		stmt->setString(2, std::string(s_email.data(), s_email.length()));

		odbc::ResultSetRef rs = stmt->executeQuery();

		return rs->next();
	}

};
#endif // !__DATABASE_HPP__
