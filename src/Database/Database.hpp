#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

// TODO: Figure out Linux solution
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <algorithm>
#include <list>

#include "Common/CrossPlatform.hpp"
#include "Encryption/sha512.hpp"
#include "GameCache/Interface/FastDatabase.hpp"
#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MissionTasks.hpp"
#include "DataTypes/Vector3.hpp"
#include "DataTypes/LWOOBJID.hpp"
#include "Configuration/ConfDatabase.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "DataTypes/LDF.hpp"
#include "Utils/LDFUtils.hpp"

#include "Database/DatabaseModels.hpp"

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

using namespace GameCache::Interface;
using namespace DatabaseModels;
extern FDB::Connection Cache;
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

		fprintf(stderr, "[DATABASE] While running %s the driver reported:", fn);

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

			std::cout << " " <<str.c_str() << std::endl;

			if (SQL_SUCCEEDED(ret)||ret==SQL_ERROR)
				printf("[DATABASE] %s ->", text);
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
	static SQLHANDLE GetSqlStmtHandle() {
		return sqlStmtHandle;
	}

	static int Connect() {
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
		Logger::log("DATABASE", "Attempting connection to SQL Server...");

		//connect to SQL Server
		//I am using a trusted connection and port 14808
		//it does not matter if you are using default or named instance
		//just make sure you define the server name and the port
		//You have the option to use a username/password instead of a trusted connection
		//but is more secure to use a trusted connection

		auto dbConf = &Configuration::ConfigurationManager::dbConf;

		std::string connStrBuilder = \
			"DRIVER={"\
			+ dbConf->GetStringVal("DBConnection", "DBDRIVER")\
			+ "};SERVER="\
			+ dbConf->GetStringVal("DBConnection", "DBHOST")\
			+ ";UID="\
			+ dbConf->GetStringVal("DBConnection", "DBUSER")\
			+ ";PWD="\
			+ dbConf->GetStringVal("DBConnection", "DBPASS")\
			+ ";";
	


		switch (SQLDriverConnect(sqlConnHandle,
			NULL,
			(SQLCHAR*)connStrBuilder.c_str(),
			SQL_NTS,
			retconstring,
			1000,
			NULL,
			SQL_DRIVER_COMPLETE)) {

		case SQL_SUCCESS:
			Logger::log("DATABASE", "Successfully connected to SQL Server");
			return 1;
			break;

		case SQL_SUCCESS_WITH_INFO:
			Logger::log("DATABASE", "Successfully connected to SQL Server");
			Logger::log("DATABASE", "Driver reported the following diagnostics");
			extract_error("SQLDriverConnect", sqlConnHandle, SQL_HANDLE_DBC);
			return 1;
			break;

		case SQL_INVALID_HANDLE:
			Logger::log("DATABASE", "Could not connect to SQL Server (SQL_INVALID_HANDLE)");
			return 2;
			Disconnect();

		case SQL_ERROR:
			Logger::log("DATABASE", "Could not connect to SQL Server (SQL_ERROR)");
			return 2;
			Disconnect();

		default:
			return 0;
			break;
		}

		SetupStatementHandle();
	}
	static int DoATestQuery() {
		SetupStatementHandle();

		//output
		Logger::log("DATABASE", "Executing T-SQL query...");

		//if there is a problem executing the query then exit application
		//else display query result
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLCHAR*)"SELECT @@VERSION", SQL_NTS)) {
			Logger::log("DATABASE", "Error querying SQL Server");

			extract_error("SQLExecDirect", sqlStmtHandle, SQL_HANDLE_DBC);

			Disconnect();
			return -1;
		}
		else {

			//declare output variable and pointer
			SQLCHAR sqlVersion[SQL_RESULT_LEN];
			SQLLEN ptrSqlVersion;

			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

				//display query result
				Logger::log("DATABASE", "Query Success");
				return 1;
			}
		}
	}

	static void Disconnect() {
		Logger::log("DATABASE", "WARNING!!!! DATABASE HAS BEEN UNLOADED!", LogType::ERR);
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

	static bool IsLoginCorrect(char16_t * username, char16_t * password) {
		std::u16string w_username(username);
		std::string s_username(w_username.begin(), w_username.end());
		std::u16string w_password(password);
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
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return false;
			}
			{
				SQLCHAR sqlAnswer[SQL_RESULT_LEN];
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlAnswer, SQL_RESULT_LEN, &ptrSqlAnswer);

				std::string db_hash = std::string((char*)&sqlAnswer, ptrSqlAnswer);
				std::string dbhash = "DB_HASH: " + db_hash;
				std::string pkhash = "PK_HASH: " + h_password;
				Logger::log("DATABASE", dbhash);
				Logger::log("DATABASE", pkhash);

				if (db_hash == h_password) {
					SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
					return true;
				}

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	static bool APILoginCheck(std::string s_username, std::string s_password) {
		std::string h_password = sha512(s_password);
		SetupStatementHandle();
		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE username=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		SQLLEN __L__ = 32;
		int __1__ = SQL_C_TCHAR;
		SQLSMALLINT* pfSqlType;
		SQLULEN* pcbParamDef;
		SQLSMALLINT* pibScale;
		SQLSMALLINT* pfNullable;
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
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return false;
			}
			{
				SQLCHAR sqlAnswer[SQL_RESULT_LEN];
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlAnswer, SQL_RESULT_LEN, &ptrSqlAnswer);
				std::string db_hash = std::string((char*)&sqlAnswer, ptrSqlAnswer);
				std::string dbhash = "DB_HASH: " + db_hash;
				std::string pkhash = "PK_HASH: " + h_password;
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

		if (sqlStmtHandle != NULL) {
			SQLCloseCursor(sqlStmtHandle);
			if (SQL_NO_DATA != SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle)) {
				extract_error("SQLFreeHandle", sqlConnHandle, SQL_HANDLE_DBC);
				extract_error("SQLFreeHandle", sqlStmtHandle, SQL_HANDLE_STMT);
			}
		}
		sqlStmtHandle = NULL;
		//if there is a problem connecting then exit application
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
			extract_error("SQLAllocHandle", sqlConnHandle, SQL_HANDLE_DBC);
			extract_error("SQLAllocHandle", sqlStmtHandle, SQL_HANDLE_STMT);
			Disconnect();
		}
	}


	enum class DBCOUNTERID {
		STATIC,
		PLAYER,
		P_STYLE,
		P_STATS
	};

	static unsigned long long reserveCountedID(DBCOUNTERID dbCounterID) {

		SQLCHAR * query;
		switch (dbCounterID) {
		case DBCOUNTERID::STATIC:
			query = (SQLCHAR*)"SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='STATIC';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='STATIC';";
			break;
		case DBCOUNTERID::PLAYER:
			query = (SQLCHAR*)"SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='PLAYER';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='PLAYER';";
			break;
		case DBCOUNTERID::P_STYLE:
			query = (SQLCHAR*)"SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='P_STYLE';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='P_STYLE';";
			break;
		case DBCOUNTERID::P_STATS:
			query = (SQLCHAR*)"SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='P_STATS';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='P_STATS';";
			break;
		default:
			return -1;
		}

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
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return -1;

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return -1;
	}

	static int GetCharCount(unsigned long accountID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT COUNT(objectID) FROM OPCRUX_GD.dbo.Characters WHERE accountID=?", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_ULONG, SQL_INTEGER, 0, 0, &accountID, 0);

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

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		{
			if (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
				SQLUBIGINT count;
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_C_UBIGINT, &count, 0, &ptrSqlAnswer);

				return count;
			}

			{
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return -1;

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	static Str_DB_CharStyle GetCharStyle(unsigned long styleID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT headColor, head, chestColor, chest, legs, hairStyle, hairColor, leftHand, rightHand, eyebrowStyle, eyesStyle, mouthStyle FROM OPCRUX_GD.dbo.CharacterStyles WHERE id=?", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_ULONG, SQL_INTEGER, 0, 0, &styleID, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLINTEGER headColor;
			SQLINTEGER head;
			SQLINTEGER chestColor;
			SQLINTEGER chest;
			SQLINTEGER legs;
			SQLINTEGER hairStyle;
			SQLINTEGER hairColor;
			SQLINTEGER leftHand;
			SQLINTEGER rightHand;
			SQLINTEGER eyebrowStyle;
			SQLINTEGER eyesStyle;
			SQLINTEGER mouthStyle;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SLONG, &headColor, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &head, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &chestColor, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_SLONG, &chest, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &legs, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SLONG, &hairStyle, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &hairColor, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 8, SQL_C_SLONG, &leftHand, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 9, SQL_C_SLONG, &rightHand, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 10, SQL_C_SLONG, &eyebrowStyle, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 11, SQL_C_SLONG, &eyesStyle, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 12, SQL_C_SLONG, &mouthStyle, 0, &ptrSqlAnswer);

			Str_DB_CharStyle charStyle;
			charStyle.headColor = headColor;
			charStyle.head = head;
			charStyle.chestColor = chestColor;
			charStyle.chest = chest;
			charStyle.legs = legs;
			charStyle.hairStyle = hairStyle;
			charStyle.hairColor = hairColor;
			charStyle.leftHand = leftHand;
			charStyle.rightHand = rightHand;
			charStyle.eyebrowStyle = eyebrowStyle;
			charStyle.eyesStyle = eyesStyle;
			charStyle.mouthStyle = mouthStyle;

			return charStyle;
		}

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return {};
	}

	static std::vector<Str_DB_CharInfo> GetChars(unsigned long accountID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT objectID, charIndex, name, pendingName, styleID,statsID, lastWorld, lastInstance, lastClone, lastLog, positionX, positionY, positionZ, shirtObjectID, pantsObjectID, uScore, uLevel, currency, reputation, health, imagination, armor FROM OPCRUX_GD.dbo.Characters WHERE accountID=? ORDER BY charIndex", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_ULONG, SQL_INTEGER, 0, 0, &accountID, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		std::vector<Str_DB_CharInfo> charsInfo;
		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLUBIGINT objectID;
			int charIndex;
			SQLCHAR sqlName[SQL_RESULT_LEN];
			SQLCHAR sqlPendingName[SQL_RESULT_LEN];
			SQLINTEGER styleID;
			SQLINTEGER statsID;
			SQLINTEGER lastWorld;
			SQLINTEGER lastInstance;
			SQLINTEGER lastClone;
			SQLUBIGINT lastLog;
			float posX;
			float posY;
			float posZ;
			SQLBIGINT shirtObjectID;
			SQLBIGINT pantsObjectID;
			SQLINTEGER uScore;
			SQLINTEGER uLevel;
			SQLINTEGER currency;
			SQLINTEGER reputation;
			SQLINTEGER health;
			SQLINTEGER imagination;
			SQLINTEGER armor;

			SQLGetData(sqlStmtHandle, 1, SQL_C_UBIGINT, &objectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &charIndex, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_TCHAR, &sqlName, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string name((char*)&sqlName, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlPendingName, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string pendingName((char*)&sqlPendingName, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &styleID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SLONG, &statsID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &lastWorld, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 8, SQL_C_SLONG, &lastInstance, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 9, SQL_C_SLONG, &lastClone, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 10, SQL_C_UBIGINT, &lastLog, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 11, SQL_C_FLOAT, &posX, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 12, SQL_C_FLOAT, &posY, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 13, SQL_C_FLOAT, &posZ, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 14, SQL_C_UBIGINT, &shirtObjectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 15, SQL_C_UBIGINT, &pantsObjectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 16, SQL_C_SLONG, &uScore, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 17, SQL_C_SLONG, &uLevel, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 18, SQL_C_SLONG, &currency, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 19, SQL_C_SLONG, &reputation, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 20, SQL_C_SLONG, &health, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 21, SQL_C_SLONG, &imagination, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 22, SQL_C_SLONG, &armor, 0, &ptrSqlAnswer);

			Str_DB_CharInfo charInfo;
			charInfo.accountID = accountID;
			charInfo.objectID = objectID;
			charInfo.charIndex = charIndex;
			charInfo.name = name;
			charInfo.pendingName = pendingName;
			charInfo.styleID = styleID;
			charInfo.statsID = statsID;
			charInfo.lastWorld = lastWorld & 0xFFFF;
			charInfo.lastInstance = lastInstance & 0xFFFF;
			charInfo.lastClone = lastClone;
			charInfo.lastLog = lastLog;
			charInfo.position = DataTypes::Vector3(posX, posY, posZ);
			charInfo.shirtObjectID = shirtObjectID;
			charInfo.pantsObjectID = pantsObjectID;
			charInfo.uScore = uScore;
			charInfo.uLevel = uLevel;
			charInfo.currency = currency;
			charInfo.reputation = reputation;
			charInfo.health = health;
			charInfo.imagination = imagination;
			charInfo.armor = armor;

			charsInfo.push_back(charInfo);
		}

		if(charsInfo.size() != GetCharCount(accountID)){
			std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
			extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};

		}
		else {
			return charsInfo;
		}

		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return {};
	}

	static Str_DB_CharInfo GetChar(unsigned long long objectID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT accountID, charIndex, name, pendingName, styleID,statsID, lastWorld, lastInstance, lastClone, lastLog, positionX, positionY, positionZ, shirtObjectID, pantsObjectID, uScore, uLevel, currency, reputation, health, imagination, armor FROM OPCRUX_GD.dbo.Characters WHERE objectID=? ORDER BY charIndex", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &objectID, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		if (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLINTEGER accountID;
			int charIndex;
			SQLCHAR sqlName[SQL_RESULT_LEN];
			SQLCHAR sqlPendingName[SQL_RESULT_LEN];
			SQLINTEGER styleID;
			SQLINTEGER statsID;
			SQLINTEGER lastWorld;
			SQLINTEGER lastInstance;
			SQLINTEGER lastClone;
			SQLUBIGINT lastLog;
			float posX;
			float posY;
			float posZ;
			SQLBIGINT shirtObjectID;
			SQLBIGINT pantsObjectID;
			SQLINTEGER uScore;
			SQLINTEGER uLevel;
			SQLINTEGER currency;
			SQLINTEGER reputation;
			SQLINTEGER health;
			SQLINTEGER imagination;
			SQLINTEGER armor;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SLONG, &accountID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &charIndex, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_TCHAR, &sqlName, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string name((char*)&sqlName, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlPendingName, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string pendingName((char*)&sqlPendingName, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &styleID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SLONG, &statsID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &lastWorld, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 8, SQL_C_SLONG, &lastInstance, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 9, SQL_C_SLONG, &lastClone, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 10, SQL_C_UBIGINT, &lastLog, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 11, SQL_C_FLOAT, &posX, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 12, SQL_C_FLOAT, &posY, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 13, SQL_C_FLOAT, &posZ, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 14, SQL_C_UBIGINT, &shirtObjectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 15, SQL_C_UBIGINT, &pantsObjectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 16, SQL_C_SLONG, &uScore, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 17, SQL_C_SLONG, &uLevel, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 18, SQL_C_SLONG, &currency, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 19, SQL_C_SLONG, &reputation, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 20, SQL_C_SLONG, &health, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 21, SQL_C_SLONG, &imagination, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 22, SQL_C_SLONG, &armor, 0, &ptrSqlAnswer);

			Str_DB_CharInfo charInfo;
			charInfo.accountID = accountID;
			charInfo.objectID = objectID;
			charInfo.charIndex = charIndex;
			charInfo.name = name;
			charInfo.pendingName = pendingName;
			charInfo.styleID = styleID;
			charInfo.statsID = statsID;
			charInfo.lastWorld = lastWorld & 0xFFFF;
			charInfo.lastInstance = lastInstance & 0xFFFF;
			charInfo.lastClone = lastClone;
			charInfo.lastLog = lastLog;
			charInfo.position = DataTypes::Vector3(posX, posY, posZ);
			charInfo.shirtObjectID = shirtObjectID;
			charInfo.pantsObjectID = pantsObjectID;
			charInfo.uScore = uScore;
			charInfo.uLevel = uLevel;
			charInfo.currency = currency;
			charInfo.reputation = reputation;
			charInfo.health = health;
			charInfo.imagination = imagination;
			charInfo.armor = armor;
			return charInfo;
		}

			std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
			extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};

		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return {};
	}

	static void UpdateChar(Str_DB_CharInfo charInfo) {
		// TODO check if char exists.
		if (false) {
			throw std::exception("Player does not exist.");
		}
		SetupStatementHandle();
		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_GD.dbo.Characters SET name=?,pendingName=?,lastWorld=?,lastInstance=?,lastClone=?,lastLog=?,positionX=?,positionY=?,positionZ=?,uScore=?,uLevel=?,currency=?,reputation=?,health=?,imagination=?,armor=? WHERE objectID=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to prepare sql.");
		}

		SQLLEN lenZero = 0;
		SQLLEN NTS = SQL_NTS;
		SQLLEN lenName = charInfo.name.size();
		SQLLEN lenPendingName = charInfo.pendingName.size();


		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, std::max<SQLUINTEGER>(charInfo.name.size(), 1), 0, (SQLPOINTER)charInfo.name.c_str(), 0, &lenName);
		ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, std::max<SQLUINTEGER>(charInfo.pendingName.size(), 1), 0, (SQLPOINTER)charInfo.pendingName.c_str(), 0, &lenPendingName);
		ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &charInfo.lastWorld, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &charInfo.lastInstance, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.lastClone, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &charInfo.lastLog, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &charInfo.position.x, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &charInfo.position.y, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &charInfo.position.z, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.uScore, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 11, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.uLevel, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.currency, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.reputation, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 14, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.health, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 15, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.imagination, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 16, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &charInfo.armor, 0, &lenZero);

		ret = SQLBindParameter(sqlStmtHandle, 17, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &charInfo.objectID, 0, &lenZero);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to bind parameters.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to execute query.");
		}
	}

	static unsigned long CreateCharStyle(
		int headColor, int head, int chestColor, int chest,
		int legs, int hairStyle, int hairColor, int leftHand,
		int rightHand, int eyebrowStyle, int eyesStyle, int mouthStyle
	){
		long id = reserveCountedID(DBCOUNTERID::P_STYLE);

		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SET IDENTITY_INSERT OPCRUX_GD.dbo.CharacterStyles ON;INSERT INTO OPCRUX_GD.dbo.CharacterStyles(id,headColor,head,chestColor,chest,legs,hairStyle,hairColor,leftHand,rightHand,eyebrowStyle,eyesStyle,mouthStyle) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
		//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SLONG, SQL_INTEGER, 0, 0, &headColor, 0);
		ret = SQLBindParam(sqlStmtHandle, 3, SQL_C_SLONG, SQL_INTEGER, 0, 0, &head, 0);
		ret = SQLBindParam(sqlStmtHandle, 4, SQL_C_SLONG, SQL_INTEGER, 0, 0, &chestColor, 0);
		ret = SQLBindParam(sqlStmtHandle, 5, SQL_C_SLONG, SQL_INTEGER, 0, 0, &chest, 0);
		ret = SQLBindParam(sqlStmtHandle, 6, SQL_C_SLONG, SQL_INTEGER, 0, 0, &legs, 0);
		ret = SQLBindParam(sqlStmtHandle, 7, SQL_C_SLONG, SQL_INTEGER, 0, 0, &hairStyle, 0);
		ret = SQLBindParam(sqlStmtHandle, 8, SQL_C_SLONG, SQL_INTEGER, 0, 0, &hairColor, 0);
		ret = SQLBindParam(sqlStmtHandle, 9, SQL_C_SLONG, SQL_INTEGER, 0, 0, &leftHand, 0);
		ret = SQLBindParam(sqlStmtHandle, 10, SQL_C_SLONG, SQL_INTEGER, 0, 0, &rightHand, 0);
		ret = SQLBindParam(sqlStmtHandle, 11, SQL_C_SLONG, SQL_INTEGER, 0, 0, &eyebrowStyle, 0);
		ret = SQLBindParam(sqlStmtHandle, 12, SQL_C_SLONG, SQL_INTEGER, 0, 0, &eyesStyle, 0);
		ret = SQLBindParam(sqlStmtHandle, 13, SQL_C_SLONG, SQL_INTEGER, 0, 0, &mouthStyle, 0);

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

		return id;
	}

	static unsigned long long CreateNewChar (
		unsigned long accountID, std::string customName, std::string genname, int headColor, int head, int chestColor, int chest,
		int legs, int hairStyle, int hairColor, int leftHand, int rightHand, int eyebrowStyle, int eyesStyle, int mouthStyle
	) {
		// Get Char Count
		int charCount = GetCharCount(accountID);

		// Check if char is creatable on char count
		if (charCount < 4) {

			// Default Valuse
			int lastWorld = 0;
			int lastInstance = 0;
			int lastClone = 0;
			unsigned long long lastLog = 0;
			DataTypes::Vector3 position = DataTypes::Vector3::zero();

			int shirtObjectLOT = 0;
			{
				FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
				if (!rth.isRowHeaderValid()) throw new std::runtime_error("Invalid Row Header");
				for (int i = 0; i < rth.getRowCount(); ++i) {
					if (!rth.isValid(i)) continue;
					try {
						if (
							*reinterpret_cast<uint32_t*>(rth[i]["itemType"].getMemoryLocation()) == 15 &&
							*reinterpret_cast<uint32_t*>(rth[i]["color1"].getMemoryLocation()) == chestColor &&
							*reinterpret_cast<uint32_t*>(rth[i]["decal"].getMemoryLocation()) == chest &&
							*reinterpret_cast<uint32_t*>(rth[i]["isBOE"].getMemoryLocation()) == 1
							) {

							int componentID = *reinterpret_cast<uint32_t*>(rth[i][0].getMemoryLocation());
							shirtObjectLOT = CacheComponentsRegistry::FindID(componentID, 11);
							break;
						}
					}
					catch (std::runtime_error e) {
						Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
					}
				}
				if(shirtObjectLOT == 0) {
					Logger::log("DB-CreateNewChar", "Unable to find componentID for shirt.");
					return -1; // Fail
				}
			}

			int pantsObjectLOT = 0; {
				FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
				for (int i = 0; i < rth.getRowCount(); ++i) {
					if (!rth.isValid(i)) continue;
					try {
						if (
							*reinterpret_cast<uint32_t*>(rth[i]["itemType"].getMemoryLocation()) == 7 &&
							*reinterpret_cast<uint32_t*>(rth[i]["color1"].getMemoryLocation()) == legs &&
							*reinterpret_cast<uint32_t*>(rth[i]["isBOE"].getMemoryLocation()) == 1
							) {
							int componentID = *reinterpret_cast<uint32_t*>(rth[i][0].getMemoryLocation());
							pantsObjectLOT = CacheComponentsRegistry::FindID(componentID, 11);
							break;
						}
					}
					catch (std::runtime_error e) {
						Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
					}
				}
				if (pantsObjectLOT == 0) {
					Logger::log("DB-CreateNewChar", "Unable to find componentID for pants.");
					return -1; // Fail
				}
			}
			int uScore = 0;
			int uLevel = 0;
			int currency = 0;
			int reputation = 0;
			int health = 4;
			int imagination = 0;
			int armor = 0;

			// Create style
			unsigned long styleID = CreateCharStyle(headColor, head, chestColor, chest, legs, hairStyle, hairColor, leftHand, rightHand, eyebrowStyle, eyesStyle, mouthStyle);

			// Create statistics
			unsigned long statsID = CreateCharStats(reserveCountedID(DBCOUNTERID::P_STATS));

			// Reserve objectID
			unsigned long long objectID = reserveCountedID(DBCOUNTERID::PLAYER);

			// Create default shirt/pants objects
			DatabaseModels::ItemModel shirtObject;
			shirtObject.attributes.SetBound(true);
			shirtObject.attributes.SetEquipped(true);
			shirtObject.templateID = shirtObjectLOT;
			shirtObject.slot = 0;
			shirtObject.tab = 0;
			shirtObject.count = 1;
			shirtObject.subkey = 0;
			shirtObject.ownerID = objectID;
			shirtObject.objectID = (1ULL << 60) + reserveCountedID(DBCOUNTERID::PLAYER);

			DatabaseModels::ItemModel pantsObject = shirtObject;
			pantsObject.templateID = pantsObjectLOT;
			pantsObject.objectID = (1ULL << 60) + reserveCountedID(DBCOUNTERID::PLAYER);
			pantsObject.slot = 1;

			AddItemToInventory(shirtObject);
			AddItemToInventory(pantsObject);

			// Create player

			SetupStatementHandle();

			SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SET IDENTITY_INSERT OPCRUX_GD.dbo.Characters ON;INSERT INTO OPCRUX_GD.dbo.Characters(objectID,accountID,charIndex,name,pendingName,styleID,statsID,lastWorld,lastInstance,lastClone,lastLog,positionX,positionY,positionZ,shirtObjectID,pantsObjectID,uScore,uLevel,currency,reputation,health,imagination,armor) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", SQL_NTS);
			//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_AD.dbo.Accounts SET username = ? WHERE id = 0", SQL_NTS);
			//ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"PRINT '?'", SQL_NTS);
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return false;
			}

			SQLLEN lenZero = 0;
			SQLLEN NTS = SQL_NTS;
			SQLLEN lenGenName = genname.size();
			SQLLEN lenCustomName = customName.size();

			ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &objectID, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &accountID, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_TINYINT, 0, 0, &charCount, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, genname.size(), 0, (SQLPOINTER)genname.c_str(), 0, &lenGenName);
			ret = SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, std::max<SQLUINTEGER>(customName.size(),1), 0, (SQLPOINTER)customName.c_str(), 0, &lenCustomName);
			ret = SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &styleID, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &statsID, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_SMALLINT, 0, 0, &lastWorld, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_SMALLINT, 0, 0, &lastInstance, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &lastClone, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 11, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &lastLog, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 12, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(position.x), 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 13, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(position.y), 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 14, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(position.z), 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 15, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &shirtObject.objectID, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 16, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pantsObject.objectID, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 17, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &uScore, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 18, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &uLevel, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 19, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &currency, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 20, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &reputation, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 21, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &health, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 22, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &imagination, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 23, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &armor, 0, &lenZero);

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

			return objectID;
		}

		return -1;
	}

	static unsigned long GetAccountIDByClientName(std::string clientName) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT id FROM OPCRUX_AD.dbo.Accounts WHERE username=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return -1;
		}

		SQLLEN lenClientName = clientName.size();
		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, clientName.size(), 0, (SQLPOINTER)clientName.c_str(), 0, &lenClientName);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return -1;
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return -1;
		}
		{
			if (SQLFetch(sqlStmtHandle) != SQL_SUCCESS) {
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return -1;
			}
			{
				SQLINTEGER accountID;
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &accountID, 0, &ptrSqlAnswer);

				return accountID;

			}
		}
	}

	static std::uint64_t getStatByID(std::int64_t statsID, std::uint32_t statsIndex) {
		const std::string lookup[27] = {
			"currencyCollected",
			"bricksCollected",
			"objectsSmashed",
			"quickbuildsCompleted",
			"enemiesSmashed",
			"rocketsUsed",
			"missionsCompleted",
			"petsTamed",
			"imaginationCollected",
			"healthCollected",
			"armorCollected",
			"distanceTraveled",
			"smashed",
			"damageTaken",
			"damageHealed",
			"armorRepaired",
			"imaginationRestored",
			"imaginationUsed",
			"distanceDriven",
			"airborneTime",
			"racingImaginationCollected",
			"racingImaginationCratesSmashed",
			"racingBoostsActivated",
			"racingWrecks",
			"racingSmashablesSmashed",
			"racesFinished",
			"racesWon"
		};
		if (statsID >= 27) throw new std::runtime_error("Index out of range.");
		std::string statsName = lookup[statsID];
		return 0;
	}

	static bool HasMission(std::int64_t charID, std::int32_t missionID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT COUNT(missionID) FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to query DB");
		}

		SQLLEN lenZero = 0;

		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &charID, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &missionID, 0, &lenZero);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to query DB");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to query DB");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		{
			if (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
				SQLUBIGINT count;
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_C_UBIGINT, &count, 0, &ptrSqlAnswer);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return count!=0;
			}

			{
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				throw std::exception("Unable to query DB");

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to query DB");
	}

	/*
		Note: check with NOT Database::HasMission(...) before executing.
	*/
	static MissionModel AddMission(std::int64_t charID, std::int32_t missionID) {
		MissionModel model;
		model.charID = charID;
		model.missionID = missionID;
		model.state = 2;
		model.repeatCount = 0;
		model.time = time(0);
		model.chosenReward = -1;

		std::list<std::int32_t> initialProgress = {};
		auto mtr = CacheMissionTasks::getRow(missionID);
		while (mtr.isValid()) {
			initialProgress.push_back(0);
			if (!mtr.isLinkedRowInfoValid()) break;
			mtr = mtr.getLinkedRowInfo();
		}
		model.progress = StringUtils::IntListToString(initialProgress, '|');
		
		if (HasMission(charID, missionID)) {
			throw std::exception("Mission already exists!");
		}
		SetupStatementHandle();
		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"INSERT INTO OPCRUX_GD.dbo.Missions(charID,missionID,state,progress,repeatCount,time,chosenReward) VALUES(?,?,?,?,?,?,?)", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to add mission.");
		}

		SQLLEN lenZero = 0;
		SQLLEN NTS = SQL_NTS;
		SQLLEN lenProgress = model.progress.size();

		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &model.charID, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &model.missionID, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &model.state, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, std::max<SQLUINTEGER>(model.progress.size(), 1), 0, (SQLPOINTER)model.progress.c_str(), 0, &lenProgress);
		ret = SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &model.repeatCount, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &model.time, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &model.chosenReward, 0, &lenZero);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to add mission.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to add mission.");
		}

		return model;
	}

	/*
		Note: check with Database::HasMission(...) before executing.
	*/
	static MissionModel GetMission(std::int64_t charID, std::int32_t missionID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID=?", SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SLONG, SQL_INTEGER, 0, 0, &missionID, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		if (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT charID;
			SQLINTEGER missionID;
			SQLINTEGER state;
			SQLCHAR sqlProgress[SQL_RESULT_LEN];
			SQLINTEGER repeatCount;
			SQLBIGINT time;
			SQLINTEGER chosenReward;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &charID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &missionID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &state, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlProgress, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string progress((char*)&sqlProgress, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &repeatCount, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &time, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &chosenReward, 0, &ptrSqlAnswer);
			
			MissionModel model;
			model.charID = charID;
			model.missionID = missionID;
			model.state = state;
			model.progress = progress;
			model.repeatCount = repeatCount;
			model.time = time;
			model.chosenReward = chosenReward;

			return model;
			
		}

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static std::list<MissionModel> GetAllMissions(std::int64_t charID) {
		SetupStatementHandle();

		std::list<MissionModel> retVal={};

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=?", SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT charID;
			SQLINTEGER missionID;
			SQLINTEGER state;
			SQLCHAR sqlProgress[SQL_RESULT_LEN];
			SQLINTEGER repeatCount;
			SQLBIGINT time;
			SQLINTEGER chosenReward;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &charID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &missionID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &state, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlProgress, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string progress((char*)&sqlProgress, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &repeatCount, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &time, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &chosenReward, 0, &ptrSqlAnswer);

			MissionModel model;
			model.charID = charID;
			model.missionID = missionID;
			model.state = state;
			model.progress = progress;
			model.repeatCount = repeatCount;
			model.time = time;
			model.chosenReward = chosenReward;

			retVal.push_back(model);

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static std::list<MissionModel> GetAllMissionsByState(std::int64_t charID, std::int32_t state) {
		SetupStatementHandle();

		std::list<MissionModel> retVal = {};

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? and state=?", SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SLONG, SQL_INTEGER, 0, 0, &state, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT charID;
			SQLINTEGER missionID;
			SQLINTEGER state;
			SQLCHAR sqlProgress[SQL_RESULT_LEN];
			SQLINTEGER repeatCount;
			SQLBIGINT time;
			SQLINTEGER chosenReward;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &charID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &missionID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &state, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlProgress, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string progress((char*)&sqlProgress, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &repeatCount, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &time, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &chosenReward, 0, &ptrSqlAnswer);

			MissionModel model;
			model.charID = charID;
			model.missionID = missionID;
			model.state = state;
			model.progress = progress;
			model.repeatCount = repeatCount;
			model.time = time;
			model.chosenReward = chosenReward;

			retVal.push_back(model);

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static std::list<MissionModel> GetAllMissionsByStates(std::int64_t charID, std::list<std::int32_t> state) {
		SetupStatementHandle();

		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND state IN (";

		if (state.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < state.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";
		
		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)stmBuilder.c_str(), SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);
		
		SQLSMALLINT qI = 2;
		for (auto it = state.begin(); it != state.end(); ++it, ++qI) {
			std::int32_t &e = *it;
			ret = SQLBindParam(sqlStmtHandle, qI, SQL_C_SLONG, SQL_INTEGER, 0, 0, &e, 0);
		}

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT charID;
			SQLINTEGER missionID;
			SQLINTEGER state;
			SQLCHAR sqlProgress[SQL_RESULT_LEN];
			SQLINTEGER repeatCount;
			SQLBIGINT time;
			SQLINTEGER chosenReward;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &charID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &missionID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &state, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlProgress, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string progress((char*)&sqlProgress, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &repeatCount, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &time, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &chosenReward, 0, &ptrSqlAnswer);

			MissionModel model;
			model.charID = charID;
			model.missionID = missionID;
			model.state = state;
			model.progress = progress;
			model.repeatCount = repeatCount;
			model.time = time;
			model.chosenReward = chosenReward;

			retVal.push_back(model);

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static std::list<MissionModel> GetAllMissionsByIDs(std::int64_t charID, std::list<std::int32_t> missions) {
		SetupStatementHandle();

		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID IN (";

		if (missions.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < missions.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)stmBuilder.c_str(), SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);

		SQLSMALLINT qI = 2;
		for (auto it = missions.begin(); it != missions.end(); ++it, ++qI) {
			std::int32_t &e = *it;
			ret = SQLBindParam(sqlStmtHandle, qI, SQL_C_SLONG, SQL_INTEGER, 0, 0, &e, 0);
		}

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT charID;
			SQLINTEGER missionID;
			SQLINTEGER state;
			SQLCHAR sqlProgress[SQL_RESULT_LEN];
			SQLINTEGER repeatCount;
			SQLBIGINT time;
			SQLINTEGER chosenReward;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &charID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &missionID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &state, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlProgress, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string progress((char*)&sqlProgress, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &repeatCount, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &time, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &chosenReward, 0, &ptrSqlAnswer);

			MissionModel model;
			model.charID = charID;
			model.missionID = missionID;
			model.state = state;
			model.progress = progress;
			model.repeatCount = repeatCount;
			model.time = time;
			model.chosenReward = chosenReward;

			retVal.push_back(model);

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static std::list<MissionModel> GetAllMissionsByIDsAndState(std::int64_t charID, std::list<std::int32_t> missions, std::int32_t state) {
		SetupStatementHandle();

		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND state=? AND missionID IN (";

		if (missions.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < missions.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)stmBuilder.c_str(), SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SLONG, SQL_INTEGER, 0, 0, &state, 0);

		SQLSMALLINT qI = 3;
		for (auto it = missions.begin(); it != missions.end(); ++it, ++qI) {
			std::int32_t &e = *it;
			ret = SQLBindParam(sqlStmtHandle, qI, SQL_C_SLONG, SQL_INTEGER, 0, 0, &e, 0);
		}

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT charID;
			SQLINTEGER missionID;
			SQLINTEGER state;
			SQLCHAR sqlProgress[SQL_RESULT_LEN];
			SQLINTEGER repeatCount;
			SQLBIGINT time;
			SQLINTEGER chosenReward;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &charID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &missionID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &state, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlProgress, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string progress((char*)&sqlProgress, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &repeatCount, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &time, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &chosenReward, 0, &ptrSqlAnswer);

			MissionModel model;
			model.charID = charID;
			model.missionID = missionID;
			model.state = state;
			model.progress = progress;
			model.repeatCount = repeatCount;
			model.time = time;
			model.chosenReward = chosenReward;

			retVal.push_back(model);

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static std::list<MissionModel> GetAllMissionsByIDsAndStates(std::int64_t charID, std::list<std::int32_t> missions, std::list<std::int32_t> states) {
		SetupStatementHandle();

		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID IN (";

		if (missions.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < missions.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ") AND state IN (";

		if (states.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < states.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";


		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)stmBuilder.c_str(), SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);

		SQLSMALLINT qI = 2;
		for (auto it = missions.begin(); it != missions.end(); ++it, ++qI) {
			std::int32_t &e = *it;
			ret = SQLBindParam(sqlStmtHandle, qI, SQL_C_SLONG, SQL_INTEGER, 0, 0, &e, 0);
		}
		for (auto it = states.begin(); it != states.end(); ++it, ++qI) {
			std::int32_t &e = *it;
			ret = SQLBindParam(sqlStmtHandle, qI, SQL_C_SLONG, SQL_INTEGER, 0, 0, &e, 0);
		}

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT charID;
			SQLINTEGER missionID;
			SQLINTEGER state;
			SQLCHAR sqlProgress[SQL_RESULT_LEN];
			SQLINTEGER repeatCount;
			SQLBIGINT time;
			SQLINTEGER chosenReward;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &charID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &missionID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &state, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_TCHAR, &sqlProgress, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string progress((char*)&sqlProgress, ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &repeatCount, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &time, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &chosenReward, 0, &ptrSqlAnswer);

			MissionModel model;
			model.charID = charID;
			model.missionID = missionID;
			model.state = state;
			model.progress = progress;
			model.repeatCount = repeatCount;
			model.time = time;
			model.chosenReward = chosenReward;

			retVal.push_back(model);

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	/*
		Note: check with Database::HasMission(...) before executing.
	*/
	static void UpdateMission(MissionModel mission) {
		if (!HasMission(mission.charID, mission.missionID)) {
			throw std::exception("Mission does not exists on player!");
		}
		SetupStatementHandle();
		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_GD.dbo.Missions SET state=?,progress=?,repeatCount=?,time=?,chosenReward=? WHERE charID=? AND missionID=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to add mission.");
		}

		SQLLEN lenZero = 0;
		SQLLEN NTS = SQL_NTS;
		SQLLEN lenProgress = mission.progress.size();

		mission.time = time(0);

		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &mission.state, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, std::max<SQLUINTEGER>(mission.progress.size(), 1), 0, (SQLPOINTER)mission.progress.c_str(), 0, &lenProgress);
		ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &mission.repeatCount, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &mission.time, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &mission.chosenReward, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &mission.charID, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &mission.missionID, 0, &lenZero);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to add mission.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to add mission.");
		}
	}


	// UPDATE OPCRUX_GD.dbo.FlagChunks SET playerID=0, chunkID=0, chunkData=0; IF @@ROWCOUNT=0 INSERT INTO OPCRUX_GD.dbo.FlagChunks VALUES(0,0,1);
	static void SetFlag(DataTypes::LWOOBJID playerID, std::uint32_t chunkID, std::uint64_t chunkData) {

		SetupStatementHandle();
		// Try to update, if not exist insert
		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_GD.dbo.FlagChunks SET chunkData=? WHERE playerID=? AND chunkID=?; IF @@ROWCOUNT=0 INSERT INTO OPCRUX_GD.dbo.FlagChunks(playerID,chunkID,chunkData) VALUES(?,?,?);", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("sql error.");
		}

		SQLLEN lenZero = 0;
		SQLLEN NTS = SQL_NTS;

		std::uint64_t ppid = playerID.getPureID();


		ret = SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &chunkData, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &ppid, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &chunkID, 0, &lenZero);

		ret = SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &ppid, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &chunkID, 0, &lenZero);
		ret = SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &chunkData, 0, &lenZero);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("error.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("error.");
		}
	}

	static std::map<std::uint32_t, std::uint64_t> GetFlagChunks(std::int64_t charID) {
		SetupStatementHandle();

		std::map<std::uint32_t, std::uint64_t> retVal = {};

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT chunkID, chunkData FROM OPCRUX_GD.dbo.FlagChunks WHERE playerID=?", SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLINTEGER chunkID;
			SQLBIGINT chunkData;

			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &chunkID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SBIGINT, &chunkData, 0, &ptrSqlAnswer);

			retVal.insert({chunkID, chunkData});

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static std::map<std::int32_t, std::list<ItemModel>> GetFullInventory(std::int64_t charID) {
		const std::list<std::int32_t> tabs = { 0,1,2,4,5,6,7,8,12,14 };
		std::map<std::int32_t, std::list<ItemModel>> result;
		for (auto it = tabs.begin(); it != tabs.end(); ++it) {
			result.insert({ *it, GetInventoryItemsOfTab(charID, *it) });
		}
		return result;
	}

	static std::list<ItemModel> GetInventoryItemsOfTab(std::int64_t charID, std::int32_t _tab) {
		SetupStatementHandle();

		std::list<ItemModel> retVal = {};

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT objectID, ownerID, subkey, tab, slot, template, count, attributes, metadata FROM OPCRUX_GD.dbo.Inventory WHERE ownerID=? AND tab=?", SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &charID, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SLONG, SQL_INTEGER, 0, 0, &_tab, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		while (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLBIGINT objectID;
			SQLBIGINT ownerID;
			SQLBIGINT subkey;
			SQLINTEGER tab;
			SQLINTEGER slot;
			SQLINTEGER templateID;
			SQLINTEGER count;
			SQLSMALLINT attributes;
			SQLCHAR sqlMetaData[SQL_RESULT_LEN];

			SQLGetData(sqlStmtHandle, 1, SQL_C_SBIGINT, &objectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SBIGINT, &ownerID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SBIGINT, &subkey, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_SLONG, &tab, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &slot, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SLONG, &templateID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &count, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 8, SQL_C_SHORT, &attributes, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 9, SQL_C_TCHAR, &sqlMetaData, SQL_RESULT_LEN, &ptrSqlAnswer);
			std::string metadata((char*)&sqlMetaData, ptrSqlAnswer);

			ItemModel model;
			model.objectID = objectID;
			model.ownerID = ownerID;
			model.subkey = subkey;
			model.tab = tab;
			model.slot = slot;
			model.templateID = templateID;
			model.count = count;
			model.attributes.SetAttributes(attributes);
			model.metadata = LDFUtils::ParseCollectionFromWString(std::u16string(metadata.begin(), metadata.end()));

			retVal.push_back(model);

		}

		return retVal;

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		throw std::exception("Unable to fetch DB.");
	}

	static void AddItemToInventory(ItemModel item) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"INSERT INTO OPCRUX_GD.dbo.Inventory (objectID, ownerID, subkey, tab, slot, template, count, attributes, metadata) VALUES (?,?,?,?,?,?,?,?,?)", SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &item.objectID, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &item.ownerID, 0);
		ret = SQLBindParam(sqlStmtHandle, 3, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &item.subkey, 0);
		ret = SQLBindParam(sqlStmtHandle, 4, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.tab, 0);
		ret = SQLBindParam(sqlStmtHandle, 5, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.slot, 0);
		ret = SQLBindParam(sqlStmtHandle, 6, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.templateID, 0);
		ret = SQLBindParam(sqlStmtHandle, 7, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.count, 0);
		ret = SQLBindParam(sqlStmtHandle, 8, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &item.attributes, 0);

		std::u16string u16metadata = LDFUtils::PackCollectionToWString(item.metadata);
		std::string metadata = std::string(u16metadata.begin(), u16metadata.end());
		SQLLEN lenMetadata = metadata.size();

		ret = SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, std::max<SQLUINTEGER>(item.metadata.size(), 1), 0, (SQLPOINTER)metadata.c_str(), 0, &lenMetadata);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}
	}

	static void UpdateItemFromInventory(ItemModel item) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"UPDATE OPCRUX_GD.dbo.Inventory SET objectID = ?, ownerID = ?, subkey = ?, tab = ?, slot = ?, template = ?, count = ?, attributes = ?, metadata = ? WHERE objectID = ?", SQL_NTS);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &item.objectID, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &item.ownerID, 0);
		ret = SQLBindParam(sqlStmtHandle, 3, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &item.subkey, 0);
		ret = SQLBindParam(sqlStmtHandle, 4, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.tab, 0);
		ret = SQLBindParam(sqlStmtHandle, 5, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.slot, 0);
		ret = SQLBindParam(sqlStmtHandle, 6, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.templateID, 0);
		ret = SQLBindParam(sqlStmtHandle, 7, SQL_C_SLONG, SQL_INTEGER, 0, 0, &item.count, 0);
		ret = SQLBindParam(sqlStmtHandle, 8, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &item.attributes, 0);

		std::u16string u16metadata = LDFUtils::PackCollectionToWString(item.metadata);
		std::string metadata = std::string(u16metadata.begin(), u16metadata.end());
		SQLLEN lenMetadata = metadata.size();

		ret = SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, std::max<SQLUINTEGER>(item.metadata.size(), 1), 0, (SQLPOINTER)metadata.c_str(), 0, &lenMetadata);

		ret = SQLBindParam(sqlStmtHandle, 10, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &item.objectID, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw std::exception("Unable to fetch DB.");
		}
	}

	static int GetAccountIDFromMinifigOBJID(unsigned long objid) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT accountID FROM OPCRUX_GD.dbo.Characters WHERE objectID=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_ULONG, SQL_INTEGER, 0, 0, &objid, 0);

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

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		{
			if (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
				SQLUBIGINT accountID;
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_C_UBIGINT, &accountID, 0, &ptrSqlAnswer);
				return accountID;
			}

			{
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return -1;

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	static int GetAccountGMLevel(unsigned long accountID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT rank FROM OPCRUX_AD.dbo.Accounts WHERE id=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_ULONG, SQL_INTEGER, 0, 0, &accountID, 0);

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

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		{
			if (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
				SQLUBIGINT rank;
				SQLLEN ptrSqlAnswer;
				SQLGetData(sqlStmtHandle, 1, SQL_C_UBIGINT, &rank, 0, &ptrSqlAnswer);

				return rank;
			}

			{
				std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return -1;

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	static Str_DB_CharStats GetCharStats(long charIndex) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SELECT statsID, TotalCurrencyCollected, TotalBricksCollected, TotalSmashablesSmashed, TotalQuickBuildsCompleted, TotalEnemiesSmashed, TotalRocketsUsed, TotalPetsTamed, TotalImaginationPowerUpsCollected, TotalLifePowerUpsCollected, TotalArmorPowerUpsCollected, TotalDistanceTraveled, TotalSuicides, TotalDamageTaken, TotalDamageHealed, TotalArmorRepaired, TotalImaginationRestored, TotalImaginationUsed, TotalDistanceDriven, TotalTimeAirborne, TotalRacingImaginationPowerUpsCollected, TotalRacecarBoostsActivated, TotalRacecarWrecks, TotalRacingSmashablesSmashed, TotalRacesFinished, TotalFirstPlaceFinishes FROM OPCRUX_GD.dbo.CharacterStats WHERE charIndex=?", SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SLONG, SQL_INTEGER, 0, 0, &charIndex, 0);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLBindParameter", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};
		}

		SQLLEN rowCount = 0;
		SQLRowCount(sqlStmtHandle, &rowCount);

		if (SQL_SUCCEEDED(ret = SQLFetch(sqlStmtHandle))) {
			SQLLEN ptrSqlAnswer;

			SQLINTEGER statsID;
			SQLINTEGER TotalCurrencyCollected;
			SQLINTEGER TotalBricksCollected;
			SQLINTEGER TotalSmashablesSmashed;
			SQLINTEGER TotalQuickBuildsCompleted;
			SQLINTEGER TotalEnemiesSmashed;
			SQLINTEGER TotalRocketsUsed;
			SQLINTEGER TotalMissionsCompleted;
			SQLINTEGER TotalPetsTamed;
			SQLINTEGER TotalImaginationPowerUpsCollected;
			SQLINTEGER TotalLifePowerUpsCollected;
			SQLINTEGER TotalArmorPowerUpsCollected;
			SQLINTEGER TotalDistanceTraveled;
			SQLINTEGER TotalSuicides;
			SQLINTEGER TotalDamageTaken;
			SQLINTEGER TotalDamageHealed;
			SQLINTEGER TotalArmorRepaired;
			SQLINTEGER TotalImaginationRestored;
			SQLINTEGER TotalImaginationUsed;
			SQLINTEGER TotalDistanceDriven;
			SQLINTEGER TotalTimeAirborne;
			SQLINTEGER TotalRacingImaginationPowerUpsCollected;
			SQLINTEGER TotalRacingImaginationCratesSmashed;
			SQLINTEGER TotalRacecarBoostsActivated;
			SQLINTEGER TotalRacecarWrecks;
			SQLINTEGER TotalRacingSmashablesSmashed;
			SQLINTEGER TotalRacesFinished;
			SQLINTEGER TotalFirstPlaceFinishes;

			SQLGetData(sqlStmtHandle, 1, SQL_C_SLONG, &statsID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 2, SQL_C_SLONG, &TotalCurrencyCollected, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 3, SQL_C_SLONG, &TotalBricksCollected, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 4, SQL_C_SLONG, &TotalSmashablesSmashed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 5, SQL_C_SLONG, &TotalQuickBuildsCompleted, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 6, SQL_C_SLONG, &TotalEnemiesSmashed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 7, SQL_C_SLONG, &TotalRocketsUsed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 8, SQL_C_SLONG, &TotalMissionsCompleted, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 9, SQL_C_SLONG, &TotalPetsTamed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 10, SQL_C_SLONG, &TotalImaginationPowerUpsCollected, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 11, SQL_C_SLONG, &TotalLifePowerUpsCollected, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 12, SQL_C_SLONG, &TotalArmorPowerUpsCollected, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 13, SQL_C_SLONG, &TotalDistanceTraveled, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 14, SQL_C_SLONG, &TotalSuicides, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 15, SQL_C_SLONG, &TotalDamageTaken, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 16, SQL_C_SLONG, &TotalDamageHealed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 17, SQL_C_SLONG, &TotalArmorRepaired, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 18, SQL_C_SLONG, &TotalImaginationRestored, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 19, SQL_C_SLONG, &TotalImaginationUsed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 20, SQL_C_SLONG, &TotalDistanceDriven, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 21, SQL_C_SLONG, &TotalTimeAirborne, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 22, SQL_C_SLONG, &TotalRacingImaginationPowerUpsCollected, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 23, SQL_C_SLONG, &TotalRacingImaginationCratesSmashed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 24, SQL_C_SLONG, &TotalRacecarBoostsActivated, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 25, SQL_C_SLONG, &TotalRacecarWrecks, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 26, SQL_C_SLONG, &TotalRacingSmashablesSmashed, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 27, SQL_C_SLONG, &TotalRacesFinished, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 28, SQL_C_SLONG, &TotalFirstPlaceFinishes, 0, &ptrSqlAnswer);

			Str_DB_CharStats charStats;

			charStats.TotalCurrencyCollected = TotalCurrencyCollected;
			charStats.TotalBricksCollected = TotalBricksCollected;
			charStats.TotalSmashablesSmashed = TotalSmashablesSmashed;
			charStats.TotalQuickBuildsCompleted = TotalQuickBuildsCompleted;
			charStats.TotalEnemiesSmashed = TotalEnemiesSmashed;
			charStats.TotalRocketsUsed = TotalRocketsUsed;
			charStats.TotalPetsTamed = TotalPetsTamed;
			charStats.TotalImaginationPowerUpsCollected = TotalImaginationPowerUpsCollected;
			charStats.TotalLifePowerUpsCollected = TotalLifePowerUpsCollected;
			charStats.TotalArmorPowerUpsCollected = TotalArmorPowerUpsCollected;
			charStats.TotalDistanceTraveled = TotalDistanceTraveled;
			charStats.TotalSuicides = TotalSuicides;
			charStats.TotalDamageTaken = TotalDamageTaken;
			charStats.TotalDamageHealed = TotalDamageHealed;
			charStats.TotalArmorRepaired = TotalArmorRepaired;
			charStats.TotalImaginationRestored = TotalImaginationRestored;
			charStats.TotalImaginationUsed = TotalImaginationUsed;
			charStats.TotalDistanceDriven = TotalDistanceDriven;
			charStats.TotalTimeAirborne = TotalTimeAirborne;
			charStats.TotalRacingImaginationPowerUpsCollected = TotalRacingImaginationPowerUpsCollected;
			charStats.TotalRacecarBoostsActivated = TotalRacecarBoostsActivated;
			charStats.TotalRacecarWrecks = TotalRacecarWrecks;
			charStats.TotalRacingSmashablesSmashed = TotalRacingSmashablesSmashed;
			charStats.TotalRacesFinished = TotalRacesFinished;
			charStats.TotalFirstPlaceFinishes = TotalFirstPlaceFinishes;

			return charStats;
		}

		std::cout << __FILE__ << " :: " << __LINE__ << " Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return {};

		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return {};
	}

	static unsigned long CreateCharStats(long statsID) {
		SetupStatementHandle();

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)"SET IDENTITY_INSERT OPCRUX_GD.dbo.CharacterStats ON;INSERT INTO OPCRUX_GD.dbo.CharacterStats (statsID, TotalCurrencyCollected, TotalBricksCollected, TotalSmashablesSmashed, TotalQuickBuildsCompleted, TotalEnemiesSmashed, TotalRocketsUsed, TotalPetsTamed, TotalImaginationPowerUpsCollected, TotalLifePowerUpsCollected, TotalArmorPowerUpsCollected, TotalDistanceTraveled, TotalSuicides, TotalDamageTaken, TotalDamageHealed, TotalArmorRepaired, TotalImaginationRestored, TotalImaginationUsed, TotalDistanceDriven, TotalTimeAirborne, TotalRacingImaginationPowerUpsCollected, TotalRacecarBoostsActivated, TotalRacecarWrecks, TotalRacingSmashablesSmashed, TotalRacesFinished, TotalFirstPlaceFinishes) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", SQL_NTS);
		
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return false;
		}

		ret = SQLBindParam(sqlStmtHandle, 1, SQL_C_SLONG, SQL_BIGINT, 0, 0, &statsID, 0);
		ret = SQLBindParam(sqlStmtHandle, 2, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 3, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 4, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 5, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 6, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 7, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 8, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 9, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 10, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 11, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 12, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 13, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 14, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 15, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 16, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 17, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 18, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 19, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 20, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 21, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 22, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 23, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 24, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 25, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 26, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 27, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);
		ret = SQLBindParam(sqlStmtHandle, 28, SQL_C_SLONG, SQL_BIGINT, 0, 0, 0, 0);

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
	}
};
#endif // !__DATABASE_HPP__
