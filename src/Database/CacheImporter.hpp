#ifndef __DATABASE__CACHEIMPORTER_HPP
#define __DATABASE__CACHEIMPORTER_HPP



#include "GameCache/Interface/FastDatabase.hpp"

using namespace GameCache::Interface;
extern FDB::Connection Cache;

class CacheImporter {
public:
	static void Import() {
	
		std::uint32_t tableCount = Cache.getTableCount();
		auto tableHeader = Cache.getTableHeader();

		std::string sqlStmDropDB = "DROP DATABASE IF EXISTS cdclient;";
		std::string sqlStmCreateDB = "CREATE DATABASE cdclient;";

		Database::SetupStatementHandle();
		SQLHANDLE sqlStmtHandle = Database::GetSqlStmtHandle();

		/*
			DROP TABLE CDCLIENT
		*/
		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)sqlStmDropDB.c_str(), SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Database::extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw;
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			Database::extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw;
		}

		/*
			CREATE TABLE CDCLIENT
		*/
		Database::SetupStatementHandle();
		ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)sqlStmCreateDB.c_str(), SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			Database::extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw;
		}

		ret = SQLExecute(sqlStmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			std::cout << "Database Exception on Execute!\n";
			Database::extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
			SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
			throw;
		}

		/*
			GO THROUGH CDCLIENT
		*/
		for (int tableIndex = 0; tableIndex < tableCount; ++tableIndex) {
			FDB::PointerString tableName = tableHeader.getTableName(tableIndex);
			auto columnHeader = tableHeader.getColumnHeader(tableIndex);
			auto columnData = columnHeader.getColumnData();
			auto columnCount = columnHeader.getColumnCount();
			auto tableRows = Cache.getRows(tableIndex);

			/*
				CREATE TABLE
			*/

			std::string sqlStmCreateTable = "USE cdclient; CREATE TABLE \"" + tableName.operator std::string() + "\" (";

			for (int columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
				if (columnIndex > 0) {
					sqlStmCreateTable += ", ";
				}
				sqlStmCreateTable += "\"" + columnData.getColumnName(columnIndex).operator std::string() + "\" ";

				FDB::DATA_TYPE columnDataType = columnData.getColumnDataType(columnIndex);

				switch (columnDataType){
					case GameCache::Interface::FDB::DATA_TYPE::NOTHING:
						//throw;
					case GameCache::Interface::FDB::DATA_TYPE::INTEGER:
						sqlStmCreateTable += "int"; break;
					case GameCache::Interface::FDB::DATA_TYPE::UNKNOWN1:
						throw;
					case GameCache::Interface::FDB::DATA_TYPE::FLOAT:
						sqlStmCreateTable += "float"; break;
					case GameCache::Interface::FDB::DATA_TYPE::TEXT:
						sqlStmCreateTable += "varchar(max)"; break;
					case GameCache::Interface::FDB::DATA_TYPE::BOOLEAN:
						sqlStmCreateTable += "bit"; break;
					case GameCache::Interface::FDB::DATA_TYPE::BIGINT:
						sqlStmCreateTable += "bigint"; break;
					case GameCache::Interface::FDB::DATA_TYPE::UNKNOWN2:
						throw;
					case GameCache::Interface::FDB::DATA_TYPE::VARCHAR:
						sqlStmCreateTable += "nvarchar(max)"; break;
					case GameCache::Interface::FDB::DATA_TYPE::INVALID_POINTER:
						throw;
					default:
						break;
				}
			}
			sqlStmCreateTable += ");";

			Database::SetupStatementHandle();
			ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)sqlStmCreateTable.c_str(), SQL_NTS);
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				Database::extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				throw;
			}

			ret = SQLExecute(sqlStmtHandle);
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				std::cout << "Database Exception on Execute!\n";
				Database::extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				throw;
			}


			auto rowCount = tableRows.getRowCount();
			auto rows = Cache.getRows(tableIndex);

			if (rowCount == 0) continue;

			/*
				INSERT THE DATA
			*/

			Logger::log("TEST", "Inserting " + tableName.operator std::string() + "...", LogType::PASSED);
			// Start query 
			std::string tmplSqlStmInsert = "USE cdclient; INSERT INTO dbo." + tableName.operator std::string() + " (";
			for (int columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
				if (columnIndex > 0) {
					tmplSqlStmInsert += ", ";
				}
				tmplSqlStmInsert += "\"" + columnData.getColumnName(columnIndex).operator std::string() + "\"";
			}
			tmplSqlStmInsert += ") VALUES ";

			int rowsFound = 0;

			std::string sqlStmInsertFull = "";
			std::string sqlStmInsert = tmplSqlStmInsert;
			std::string sqlStmInsertVal = "";
			for (int rowIndex = 0; rowIndex < rowCount; ++rowIndex) {
				if (!rows.isValid(rowIndex)) continue;

				auto rowInfo = tableRows[rowIndex];
				while (rowInfo.isValid()) {

					if (rowsFound > 0) {
						sqlStmInsertVal += ", ";
					}
					++rowsFound;

					sqlStmInsertVal += "(";

					for (int columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
						if (columnIndex > 0) {
							sqlStmInsertVal += ", ";
						}
						if (rowInfo[columnIndex].isNull()) {
							sqlStmInsertVal += "NULL";
							continue;
						}
						if (columnData.getColumnDataType(columnIndex) == FDB::DATA_TYPE::BOOLEAN) {
							sqlStmInsertVal += (rowInfo.getRowData(columnIndex).ToString() == "True") ? "1" : "0";
						}
						else if (columnData.getColumnDataType(columnIndex) == FDB::DATA_TYPE::TEXT || columnData.getColumnDataType(columnIndex) == FDB::DATA_TYPE::VARCHAR) {
							std::string strVal = rowInfo.getRowData(columnIndex).ToString();
							StringUtils::replace(strVal, "'", "\'\'");
							sqlStmInsertVal += "\'" + strVal + "\'";
						}
						else {
							sqlStmInsertVal += rowInfo.getRowData(columnIndex).ToString();
						}
					}

					sqlStmInsertVal += ")";

					if (rowInfo.getMemoryLocation() == nullptr || !rowInfo.isLinkedRowInfoValid()) break;
					rowInfo = rowInfo.getLinkedRowInfo();

					if (rowsFound >= 50) {
						sqlStmInsert += sqlStmInsertVal;

						sqlStmInsertFull += sqlStmInsert + ";";

						rowsFound = 0;
						sqlStmInsert = tmplSqlStmInsert;
						sqlStmInsertVal = "";
					}
				}

				
			}

			sqlStmInsert += sqlStmInsertVal;
			sqlStmInsertFull += sqlStmInsertFull + ";";

			Logger::log("TEST", sqlStmInsert, LogType::INFO);
			Database::SetupStatementHandle();

			ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)sqlStmInsertFull.c_str(), SQL_NTS);
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				Database::extract_error("SQLPrepare", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				throw;
			}

			ret = SQLExecute(sqlStmtHandle);
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
				std::cout << "Database Exception on Execute!\n";
				Database::extract_error("SQLExecute", sqlStmtHandle, SQL_HANDLE_STMT);
				SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
				throw;
			}

			RakSleep(200);

			//Logger::log("TEST", sqlStmCreateTable, LogType::INFO);
		}
	}
};
#endif