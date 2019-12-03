#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

// TODO: Figure out Linux solution
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <algorithm>

#include "Common/CrossPlatform.hpp"
#include "Encryption/sha512.hpp"
#include "GameCache/Interface/FastDatabase.hpp"
#include "GameCache/ComponentsRegistry.hpp"
#include "DataTypes/Vector3.hpp"

#include "DB_Server.hpp"

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

using namespace GameCache::Interface;
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
			(SQLCHAR*)DB_CREDENTIALS,
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

	static void UpdateClientSession(char * key) {

	}
	static void SetupStatementHandle() {

		if (sqlStmtHandle != NULL) {
			SQLCloseCursor(sqlStmtHandle);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
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
				std::cout << "Database Exception on Fetch!\n";
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
				std::cout << "Database Exception on Fetch!\n";
				extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				return -1;

			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return false;
	}

	struct Str_DB_CharStyle {
	public:
		int headColor;
		int head;
		int chestColor;
		int chest;
		int legs;
		int hairStyle;
		int hairColor;
		int leftHand;
		int rightHand;
		int eyebrowStyle;
		int eyesStyle;
		int mouthStyle;
	};

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

		std::cout << "Database Exception on Fetch!\n";
		extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return {};
	}

	struct Str_DB_CharInfo {
	public:
		unsigned long accountID;
		unsigned long long objectID;
		unsigned char charIndex;
		std::string name="";
		std::string pendingName="";
		unsigned int styleID;
		unsigned int statsID;
		unsigned short lastWorld;
		unsigned short lastInstance;
		unsigned int lastClone;
		unsigned long long lastLog;
		DataTypes::Vector3 position;
		unsigned int shirtObjectID;
		unsigned int pantsObjectID;
		unsigned int uScore;
		unsigned int uLevel;
		unsigned int currency;
		unsigned int reputation;
		unsigned int health;
		unsigned int imagination;
		unsigned int armor;
		Str_DB_CharInfo() {
			name = "";
			pendingName = "";
		}
	};

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
			SQLINTEGER shirtObjectID;
			SQLINTEGER pantsObjectID;
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
			SQLGetData(sqlStmtHandle, 14, SQL_C_SLONG, &shirtObjectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 15, SQL_C_SLONG, &pantsObjectID, 0, &ptrSqlAnswer);
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
			std::cout << "Database Exception on Fetch!\n";
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
			SQLINTEGER shirtObjectID;
			SQLINTEGER pantsObjectID;
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
			SQLGetData(sqlStmtHandle, 14, SQL_C_SLONG, &shirtObjectID, 0, &ptrSqlAnswer);
			SQLGetData(sqlStmtHandle, 15, SQL_C_SLONG, &pantsObjectID, 0, &ptrSqlAnswer);
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

			std::cout << "Database Exception on Fetch!\n";
			extract_error("SQLFetch", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			return {};

		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		return {};
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

			int shirtObjectID = 0;
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
							shirtObjectID = CacheComponentsRegistry::FindID(componentID, 11);
							break;
						}
					}
					catch (std::runtime_error e) {
						Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
					}
				}
				if(shirtObjectID == 0) {
					Logger::log("DB-CreateNewChar", "Unable to find componentID for shirt.");
					return -1; // Fail
				}
			}

			int pantsObjectID = 0; {
				FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
				for (int i = 0; i < rth.getRowCount(); ++i) {
					if (!rth.isValid(i)) continue;
					try {
						if (
							*reinterpret_cast<uint32_t*>(rth[i]["itemType"].getMemoryLocation()) == 7 &&
							*reinterpret_cast<uint32_t*>(rth[i]["color1"].getMemoryLocation()) == pantsObjectID &&
							*reinterpret_cast<uint32_t*>(rth[i]["isBOE"].getMemoryLocation()) == 1
							) {
							int componentID = *reinterpret_cast<uint32_t*>(rth[i][0].getMemoryLocation());
							pantsObjectID = CacheComponentsRegistry::FindID(componentID, 11);
							break;
						}
					}
					catch (std::runtime_error e) {
						Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
					}
				}
				if (pantsObjectID == 0) {
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
			// TODO
			unsigned long statsID = 0;

			// Reserve objectID
			unsigned long long objectID = reserveCountedID(DBCOUNTERID::PLAYER);

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
			ret = SQLBindParameter(sqlStmtHandle, 15, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &shirtObjectID, 0, &lenZero);
			ret = SQLBindParameter(sqlStmtHandle, 16, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pantsObjectID, 0, &lenZero);
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
				std::cout << "Database Exception on Fetch!\n";
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

	static void DebugTest() {

	}

};
#endif // !__DATABASE_HPP__
