#pragma once
#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

// TODO: Figure out Linux solution
#include <windows.h>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <algorithm>
#include <list>
#include <bcrypt/BCrypt.hpp>

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

class DBInterface {
public:

	static void extract_error(
		const wchar_t* fn,
		SQLHANDLE handle,
		SQLSMALLINT type) {
		SQLINTEGER   i = 0;
		SQLINTEGER   native;
		SQLWCHAR      state[7];
		SQLWCHAR      text[256];
		SQLSMALLINT  len;
		SQLRETURN    ret;

		fwprintf(stderr,
			L"\n"
			L"The driver reported the following diagnostics whilst running "
			L"%s\n\n",
			fn);

		do
		{
			ret = SQLGetDiagRec(type, handle, ++i, state, &native, text,
				sizeof(text), &len);

			std::wstring str = L"";

			switch (ret) {
			case SQL_SUCCESS:
				str = L"SQL_SUCCESS"; break;
			case SQL_SUCCESS_WITH_INFO:
				str = L"SQL_SUCCESS_WITH_INFO"; break;
			case SQL_ERROR:
				str = L"SQL_ERROR"; break;
			case SQL_INVALID_HANDLE:
				str = L"SQL_INVALID_HANDLE"; break;
			case SQL_NO_DATA:
				str = L"SQL_NO_DATA"; break;
			case SQL_NEED_DATA:
				str = L"SQL_NEED_DATA"; break;
			case SQL_STILL_EXECUTING:
				str = L"SQL_STILL_EXECUTING"; break;
			default:
				str = L"Unknkown " + ret;
			}

			wprintf(str.c_str());

			if (SQL_SUCCEEDED(ret) || ret == SQL_ERROR)
				wprintf(L"%s:%ld:%ld:%s\n", state, i, native, text);
		} while (ret == SQL_SUCCESS || ret == SQL_ERROR);
	}

	static void Test() {
		ListDrivers();
		Connect();
		Disconnect();
	}

	static void ListDrivers() {
		{
			SQLHENV env;
			SQLWCHAR driver[256];
			SQLWCHAR attr[256];
			SQLSMALLINT driver_ret;
			SQLSMALLINT attr_ret;
			SQLUSMALLINT direction;
			SQLRETURN ret;

			SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
			SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

			direction = SQL_FETCH_FIRST;
			while (SQL_SUCCEEDED(ret = SQLDrivers(env, direction,
				driver, sizeof(driver), &driver_ret,
				attr, sizeof(attr), &attr_ret))) {
				direction = SQL_FETCH_NEXT;
				wprintf(L"%s - %s\n", driver, attr);
				if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
			}
		}
	}
private:
	//define handles and variables
	inline static SQLHANDLE sqlConnHandle;
	inline static SQLHANDLE sqlEnvHandle;
	inline static SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
	inline static SQLHANDLE sqlStmtHandle = NULL;
public:
	static SQLHANDLE GetSqlStmtHandle() {
		return sqlStmtHandle;
	}

	static void Connect() {
		//initializations
		sqlConnHandle = NULL;

		//allocations
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
			Disconnect();

		if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			Disconnect();

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
			Disconnect();

		if (SQL_SUCCESS != SQLSetConnectAttr(sqlConnHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
			Disconnect();

		//output
		std::cout << "Attempting connection to SQL Server...";
		std::cout << "\n";

		//connect to SQL Server
		//I am using a trusted connection and port 14808
		//it does not matter if you are using default or named instance
		//just make sure you define the server name and the port
		//You have the option to use a username/password instead of a trusted connection
		//but is more secure to use a trusted connection

		std::string _dbDriver = "SQL Server";
		std::string _dbHost = "LAPTOP-E4UQM49G\\SQLEXPRESS";
		std::string _dbUser = "dev_opcrux";
		std::string _dbPass = "dev_test";

		std::string connStrBuilder = \
			"DRIVER={"\
			+ _dbDriver\
			+ "};SERVER="\
			+ _dbHost\
			+ ";UID="\
			+ _dbUser\
			+ ";PWD="\
			+ _dbPass\
			+ ";";



		switch (SQLDriverConnect(sqlConnHandle,
			NULL,
			(SQLWCHAR*)std::wstring(connStrBuilder.begin(), connStrBuilder.end()).c_str(),
			SQL_NTS,
			retconstring,
			1000,
			NULL,
			SQL_DRIVER_COMPLETE)) {

		case SQL_SUCCESS:
			std::cout << "Successfully connected to SQL Server";
			std::cout << "\n";
			break;

		case SQL_SUCCESS_WITH_INFO:
			std::cout << "Successfully connected to SQL Server";
			std::cout << "\n";
			printf("Driver reported the following diagnostics\n");
			extract_error(L"SQLDriverConnect", sqlConnHandle, SQL_HANDLE_DBC);
			break;

		case SQL_INVALID_HANDLE:
			std::cout << "Could not connect to SQL Server (SQL_INVALID_HANDLE)";
			std::cout << "\n";
			Disconnect();

		case SQL_ERROR:
			std::cout << "Could not connect to SQL Server (SQL_ERROR)";
			std::cout << "\n";
			Disconnect();

		default:
			break;
		}

		SetupStatementHandle();
	}
	static void DoATestQuery() {
		SetupStatementHandle();

		//output
		std::cout << "\n";
		std::cout << "Executing T-SQL query...";
		std::cout << "\n";

		//if there is a problem executing the query then exit application
		//else display query result
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT @@VERSION", SQL_NTS)) {
			std::cout << "Error querying SQL Server";
			std::cout << "\n";

			extract_error(L"SQLExecDirect", sqlStmtHandle, SQL_HANDLE_DBC);

			Disconnect();
		}
		else {

			//declare output variable and pointer
			SQLCHAR sqlVersion[SQL_RESULT_LEN];
			SQLLEN ptrSqlVersion;

			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

				//display query result
				std::cout << "\nQuery Result:\n\n";
				std::cout << sqlVersion << std::endl;
			}
		}
	}
	static void Disconnect() {
		std::cout << "\nWARNING!!!! DATABASE HAS BEEN UNLOADED!\n";

		//close connection and free resources
		//SetupStatementHandle();
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		SQLDisconnect(sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

		//Connect();

		//pause the console window - exit when key is pressed
		//std::cout << "\nPress any key to exit...";
		//getchar();
	}

	static bool IsLoginCorrect(std::string_view& s_username, std::string_view& s_password) {

		//std::string h_password = BCrypt::generateHash(std::string(s_password.data(), s_password.length()));
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepareA(sqlStmtHandle, (SQLCHAR*)"SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE username=?", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error(L"SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		SQLLEN __L__ = 32;
		int __1__ = SQL_C_CHAR;
		SQLSMALLINT* pfSqlType;
		SQLULEN* pcbParamDef;
		SQLSMALLINT* pibScale;
		SQLSMALLINT* pfNullable;
		size_t len = s_username.size();
		SQLDescribeParam(sqlStmtHandle, 1, (SQLSMALLINT*)&__1__, (SQLULEN*)&__L__, 0, 0);
		int wat = SQL_NTS;
		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, s_username.size(), 0, (SQLPOINTER)s_username.data(), s_username.size(), (SQLLEN*)&len);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error(L"SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error(L"SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}
		{
			SQLLEN rowCount;
			auto hm = SQLRowCount(sqlStmtHandle, &rowCount);
			if (SQLFetch(sqlStmtHandle) != SQL_SUCCESS) {
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error(L"SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return false;
			}
			{
				SQLCHAR sqlAnswer[SQL_RESULT_LEN];
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlAnswer, SQL_RESULT_LEN, &ptrSqlAnswer);

				std::string db_hash = std::string((char*)&sqlAnswer, strlen((const char*)sqlAnswer));
				//std::cout << "DB_HASH: " << db_hash << "\nPK_HASH: " << h_password << "\n";

				if (BCrypt::validatePassword(std::string(s_password.data(), s_password.size()), db_hash)) {
					SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
					return true;
				}
			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	static bool IsLoginCorrectEmail(std::string_view& s_email, std::string_view& s_password) {

		//std::string h_password = BCrypt::generateHash(std::string(s_password.data(), s_password.length()));
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepareA(sqlStmtHandle, (SQLCHAR*)"SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE email=?", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error(L"SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		SQLLEN __L__ = 32;
		int __1__ = SQL_C_CHAR;
		SQLSMALLINT* pfSqlType;
		SQLULEN* pcbParamDef;
		SQLSMALLINT* pibScale;
		SQLSMALLINT* pfNullable;
		size_t len = s_email.size();
		SQLDescribeParam(sqlStmtHandle, 1, (SQLSMALLINT*)&__1__, (SQLULEN*)&__L__, 0, 0);
		int wat = SQL_NTS;
		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, s_email.size(), 0, (SQLPOINTER)s_email.data(), s_email.size(), (SQLLEN*)&len);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error(L"SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error(L"SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}
		{
			SQLLEN rowCount;
			auto hm = SQLRowCount(sqlStmtHandle, &rowCount);
			if (SQLFetch(sqlStmtHandle) != SQL_SUCCESS) {
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error(L"SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return false;
			}
			{
				SQLCHAR sqlAnswer[SQL_RESULT_LEN];
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlAnswer, SQL_RESULT_LEN, &ptrSqlAnswer);

				std::string db_hash = std::string((char*)&sqlAnswer, strlen((const char*)sqlAnswer));
				//std::cout << "DB_HASH: " << db_hash << "\nPK_HASH: " << h_password << "\n";

				if (BCrypt::validatePassword(std::string(s_password.data(), s_password.size()), db_hash)) {
					SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
					return true;
				}
			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	static void SetupStatementHandle() {

		if (sqlStmtHandle != NULL) {
			SQLCloseCursor(sqlStmtHandle);
			if (SQL_NO_DATA != SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle)) {
				extract_error(L"SQLFreeHandle", sqlConnHandle, SQL_HANDLE_DBC);
				extract_error(L"SQLFreeHandle", sqlStmtHandle, SQL_HANDLE_STMT);
			}
		}
		sqlStmtHandle = NULL;
		//if there is a problem connecting then exit application
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
			extract_error(L"SQLAllocHandle", sqlConnHandle, SQL_HANDLE_DBC);
			extract_error(L"SQLAllocHandle", sqlStmtHandle, SQL_HANDLE_STMT);
			Disconnect();
		}
	}

};
#endif // !__DATABASE_HPP__
