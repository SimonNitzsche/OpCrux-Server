#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

// TODO: Figure out Linux solution
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

#include "Encryption/sha512.hpp"

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

class Database {
public:

	static void extract_error(
		char *fn,
		SQLHANDLE handle,
		SQLSMALLINT type) {
		SQLINTEGER   i = 0;
		SQLINTEGER   native;
		SQLCHAR      state[7];
		SQLCHAR      text[256];
		SQLSMALLINT  len;
		SQLRETURN    ret;

		fprintf(stderr,
			"\n"
			"The driver reported the following diagnostics whilst running "
			"%s\n\n",
			fn);

		

		do
		{
			ret = SQLGetDiagRec(type, handle, ++i, state, &native, text,
				sizeof(text), &len);

			std::string str = "";

			switch (ret) {
			case SQL_SUCCESS:
				str = "SQL_SUCCESS"; break;
			case SQL_SUCCESS_WITH_INFO:
				str = "SQL_SUCCESS_WITH_INFO"; break;
			case SQL_ERROR:
				str = "SQL_ERROR"; break;
			case SQL_INVALID_HANDLE:
				str = "SQL_INVALID_HANDLE"; break;
			case SQL_NO_DATA:
				str = "SQL_NO_DATA"; break;
			case SQL_NEED_DATA:
				str = "SQL_NEED_DATA"; break;
			case SQL_STILL_EXECUTING:
				str = "SQL_STILL_EXECUTING"; break;
			default:
				str = "Unknkown " + ret;
			}

			printf(str.c_str());

			if (SQL_SUCCEEDED(ret)||ret==SQL_ERROR)
				printf("%s:%ld:%ld:%s\n", state, i, native, text);
		} while (ret == SQL_SUCCESS||ret==SQL_ERROR);
	}

	

	static void Test() {
		ListDrivers();
		Connect();
		Disconnect();
	}

	static void ListDrivers() {
		{
			SQLHENV env;
			SQLCHAR driver[256];
			SQLCHAR attr[256];
			SQLSMALLINT driver_ret;
			SQLSMALLINT attr_ret;
			SQLUSMALLINT direction;
			SQLRETURN ret;

			SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
			SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

			direction = SQL_FETCH_FIRST;
			while (SQL_SUCCEEDED(ret = SQLDrivers(env, direction,
				driver, sizeof(driver), &driver_ret,
				attr, sizeof(attr), &attr_ret))) {
				direction = SQL_FETCH_NEXT;
				printf("%s - %s\n", driver, attr);
				if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
			}
		}
	}
private:
	//define handles and variables
	inline static SQLHANDLE sqlConnHandle;
	inline static SQLHANDLE sqlEnvHandle;
	inline static SQLCHAR retconstring[SQL_RETURN_CODE_LEN];
	inline static SQLHANDLE sqlStmtHandle = NULL;
public:
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


		//output
		std::cout << "Attempting connection to SQL Server...";
		std::cout << "\n";

		//connect to SQL Server	
		//I am using a trusted connection and port 14808
		//it does not matter if you are using default or named instance
		//just make sure you define the server name and the port
		//You have the option to use a username/password instead of a trusted connection
		//but is more secure to use a trusted connection
		switch (SQLDriverConnect(sqlConnHandle,
			NULL,
			//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=master;UID=username;PWD=password;",
			//(SQLCHAR*)"DRIVER={SQL Server};SERVER=DESKTOP-U26DIDP, 1433;DATABASE=OPCRUX_GD;UID=dev_opcrux;PWD=dev_opcrux", ODBC Driver 13 for SQL Server
			(SQLCHAR*)"DRIVER={SQL Server};SERVER=DESKTOP-U26DIDP;UID=dev_opcrux;PWD=dev_opcrux;",
			SQL_NTS,
			retconstring,
			1024,
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
			extract_error("SQLDriverConnect", sqlConnHandle, SQL_HANDLE_DBC);
			break;

		case SQL_INVALID_HANDLE:
			std::cout << "Could not connect to SQL Server";
			std::cout << "\n";
			Disconnect();

		case SQL_ERROR:
			std::cout << "Could not connect to SQL Server";
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
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLCHAR*)"SELECT @@VERSION", SQL_NTS)) {
			std::cout << "Error querying SQL Server";
			std::cout << "\n";

			extract_error("SQLExecDirect", sqlStmtHandle, SQL_HANDLE_DBC);

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
		SetupStatementHandle();
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		SQLDisconnect(sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

		Connect();

		//pause the console window - exit when key is pressed
		//std::cout << "\nPress any key to exit...";
		//getchar();
	}

	static bool IsLoginCorrect(wchar_t * username, wchar_t * password) {
		std::wstring w_username(username);
		std::string s_username(w_username.begin(), w_username.end());
		std::wstring w_password(password);
		std::string s_password(w_password.begin(), w_password.end());

		std::string h_password = sha512(s_password);
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE username=?", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}
		
		SQLLEN __L__ = 32;
		int __1__ = SQL_C_TCHAR;
		SQLSMALLINT * pfSqlType;
		SQLULEN * pcbParamDef;
		SQLSMALLINT * pibScale;
		SQLSMALLINT * pfNullable;
		size_t len = s_username.size();
		SQLDescribeParam(sqlStmtHandle, 1, (SQLSMALLINT*)&__1__, (SQLULEN*)&__L__, 0, 0);
		int wat = SQL_NTS;
		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, s_username.size(), 0, (SQLPOINTER)s_username.c_str(), s_username.size(), (SQLLEN*)&len);
		
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}
		{
			if (SQLFetch(sqlStmtHandle) != SQL_SUCCESS) {
				std::cout << "Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return false;
			}
			{
				SQLCHAR sqlAnswer[SQL_RESULT_LEN];
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlAnswer, SQL_RESULT_LEN, &ptrSqlAnswer);

				std::string db_hash = std::string((char*)&sqlAnswer, ptrSqlAnswer);
				std::cout << "DB_HASH: " << db_hash << "\nPK_HASH: " << h_password << "\n";

				if (db_hash == h_password) {
					SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
					return true;
				}

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	static void SetupStatementHandle() {
		if (sqlStmtHandle != NULL) SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		sqlStmtHandle = NULL;
		//if there is a problem connecting then exit application
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
			Disconnect();
	}

	static unsigned long long reserveStaticObjectID(bool bPlayer=false) {
		//output
		std::cout << "\n";
		std::cout << "Reserving object ID for object group "<<(bPlayer?"PLAYER":"STATIC")<<"...";
		std::cout << "\n";

		SQLCHAR * query
			= bPlayer
			? (SQLCHAR*)"SELECT counter FROM OPCRUX_CD.dbo.ObjectIDCounter WHERE type='PLAYER';UPDATE OPCRUX_CD.dbo.ObjectIDCounter SET counter=(counter+1) WHERE type='PLAYER';"
			: (SQLCHAR*)"SELECT counter FROM OPCRUX_CD.dbo.ObjectIDCounter WHERE type='STATIC';UPDATE OPCRUX_CD.dbo.ObjectIDCounter SET counter=(counter+1) WHERE type='STATIC';";

		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, query, SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		SQLLEN rowCount=0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		{
			if (SQL_SUCCEEDED(ret=SQLFetch(sqlStmtHandle))) {
				SQLUBIGINT id;
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_C_UBIGINT, &id, 0, &ptrSqlAnswer);

				return id;
			}
			
			{
				std::cout << "Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return -1;

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return -1;
	}

	static void DebugTest() {

	}

	static void CreateNewChar() {

	}

};
#endif // !__DATABASE_HPP__