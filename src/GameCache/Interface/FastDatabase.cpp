#include "FastDatabase.hpp"

#include "Utils/FileUtils.hpp"
#include "Utils/Logger.hpp"

#include <cstring>

namespace GameCache::Interface::FDB {

	/*
		Class: FieldValue
	*/
	
	unsigned char* FieldValue::getMemoryLocation() {
		auto returnData = reinterpret_cast<unsigned char*>(data);
		if (returnData != nullptr || dataType != DATA_TYPE::NOTHING)
			return returnData;
		return const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&NULL_DATA));
	}

	std::string FieldValue::ToString() {
		switch (dataType) {
			case DATA_TYPE::NOTHING: {
				return "NULL";
			} break;

			case DATA_TYPE::INVALID_POINTER: {
				return "<Invalid Pointer>";
			}

			case DATA_TYPE::INTEGER: {
				return std::to_string(*data);
			} break;
			case DATA_TYPE::FLOAT: {
				return std::to_string(*reinterpret_cast<float*>(data));
			} break;

			case DATA_TYPE::TEXT:
			case DATA_TYPE::VARCHAR: {
				return PointerString(conn, getMemoryLocation());
			} break;

			case DATA_TYPE::BIGINT: {
				return std::to_string(*reinterpret_cast<uint64_t*>(conn->getFileData() + *data));
			} break;

			case DATA_TYPE::BOOLEAN: {
				return (*data == 1) ? "True" : "False";
			} break;

			case DATA_TYPE::UNKNOWN1:
			case DATA_TYPE::UNKNOWN2: {
				throw std::runtime_error("Unexpected: Found unused DATA_TYPE.");
			} break;

			default: {
				throw std::runtime_error("Unexpected: Found unimplemented DATA_TYPE.");
			}
		}
	}

	/*
		Class: PointerString
	*/

	PointerString::PointerString(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {
		if (memlocation == nullptr) {
			length = 0;
			return;
		}
		if (*reinterpret_cast<uint32_t*>(memlocation) == 0) {
			length = 0;
			return;
		}
		memlocation = conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation);
		length = std::strlen(reinterpret_cast<const char*>(memlocation));
	}

	PointerString::PointerString(FieldValue * fieldValue) : PointerString(fieldValue->getConnection(), fieldValue->getMemoryLocation()) {}

	/*
		Class: ColumnData
	*/

	DATA_TYPE ColumnData::getColumnDataType(uint32_t indexOfColumn) {
		return *reinterpret_cast<DATA_TYPE*>(memlocation + (8 * indexOfColumn));
	}

	PointerString ColumnData::getColumnName(uint32_t indexOfColumn) {
		return PointerString(conn, memlocation + (8 * indexOfColumn) + 4);
	}

	/*
		Class: ColumnHeader
	*/
	
	uint32_t ColumnHeader::getColumnCount() {
		return *reinterpret_cast<uint32_t*>(memlocation);
	}

	PointerString ColumnHeader::getTableName() {
		return PointerString(conn, memlocation + 4);
	}

	ColumnData ColumnHeader::getColumnData() {
		return ColumnData(conn, conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation + 8));
	}

	/*
		Class: RowData
	*/

	DATA_TYPE RowData::getColumnDataType(uint32_t indexOfColumn) {
		return static_cast<DATA_TYPE>(*reinterpret_cast<int32_t*>(memlocation + (8 * indexOfColumn)));
	}

	FieldValue RowData::getColumnData(uint32_t indexOfColumn) {
		if (memlocation == nullptr)
			return FieldValue(conn, FDB::DATA_TYPE::NOTHING, nullptr);
		return FieldValue(conn, getColumnDataType(indexOfColumn), reinterpret_cast<int32_t*>(memlocation + (8 * indexOfColumn) + 4));
	}

	/*
		Class: RowDataHeader
	*/
	
	bool RowDataHeader::isRowDataValid() {
		return *reinterpret_cast<int32_t*>(memlocation + 4) != EOF;
	}

	int32_t RowDataHeader::getColumnCount() {
		return *reinterpret_cast<int32_t*>(memlocation);
	}

	RowData RowDataHeader::getRowData() {
		if (memlocation == nullptr)
			return RowData(conn, nullptr);
		int32_t offset = *reinterpret_cast<int32_t*>(memlocation + 4);
		if (offset == EOF)
			throw std::runtime_error("RowData is invalid.");
		return RowData(conn, conn->getFileData() + offset);
	}

	/*
		Class: RowInfo
	*/

	bool RowInfo::isValid() {
		if (this->memlocation != nullptr) {
			int32_t offsetA = *reinterpret_cast<int32_t*>(memlocation);
			if (offsetA != EOF)
				if (*reinterpret_cast<int32_t*>(conn->getFileData() + offsetA) != EOF)
					return true;
		}
		return false;
	}

	bool RowInfo::isRowDataHeaderValid() {
		return *reinterpret_cast<uint32_t*>(memlocation) != EOF;
	}

	bool RowInfo::isLinkedRowInfoValid() {
		return *reinterpret_cast<uint32_t*>(memlocation + 4) != EOF;
	}

	RowDataHeader RowInfo::getRowDataHeader() {
		if (memlocation == nullptr) {
			return RowDataHeader(conn, nullptr);
		}
		int32_t offset = *reinterpret_cast<int32_t*>(memlocation);
		if (offset == EOF)
			throw std::runtime_error("RowDataHeader is invalid.");
		return RowDataHeader(conn, conn->getFileData() + offset);
	}

	RowInfo RowInfo::getLinkedRowInfo() {
		int32_t offset = *reinterpret_cast<int32_t*>(memlocation + 4);
		if (offset == EOF)
			throw std::runtime_error("RowInfo is invalid.");
		return RowInfo(conn, conn->getFileData() + offset, columnHeaderAddr);
	}

	/*
		Class: RowHeader
	*/

	RowInfo RowHeader::getRowInfo(uint32_t indexOfRow) {
		int32_t offset = *reinterpret_cast<uint32_t*>(memlocation + (4 * indexOfRow));
		if (offset == EOF)
			throw std::runtime_error("RowInfo is invalid.");
		return RowInfo(conn, conn->getFileData() + offset, columnHeaderAddr);
	}

	bool RowHeader::isRowInfoValid(uint32_t indexOfRow) {
		return *reinterpret_cast<uint32_t*>(memlocation + (4*indexOfRow)) != EOF;
	}

	/*
		Class: RowTopHeader
	*/

	uint32_t RowTopHeader::getRowCount() {
		return *reinterpret_cast<uint32_t*>(memlocation);
	}

	RowHeader RowTopHeader::getRowHeader() {
		int32_t offset = *reinterpret_cast<int32_t*>(memlocation + 4);
		if (offset == EOF)
			throw std::runtime_error("RowHeader is invalid.");
		return RowHeader(conn, conn->getFileData() + offset, columnHeaderAddr);
	}

	bool RowTopHeader::isRowHeaderValid() {
		return *reinterpret_cast<int32_t*>(memlocation + 4) != EOF;
	}

	/*
		Class: TableHeader
	*/

	ColumnHeader TableHeader::getColumnHeader(uint32_t indexOfTable) {
		return ColumnHeader(conn, conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation + (8 * indexOfTable)) );
	}

	RowTopHeader TableHeader::getRowTopHeader(uint32_t indexOfTable) {
		return RowTopHeader(conn, conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation + (8 * indexOfTable) + 4), conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation + (8 * indexOfTable)));
	}

	/*
		Class: Connection
	*/

	Connection::Connection(std::string database) {
		Connect(database);
	}

	void Connection::Connect(std::string database) {
		filePtr = FileUtils::ReadFileCompletely(database);
		fileData = filePtr.get();
	}

	uint32_t Connection::getTableCount() {
		return *fileData;
	}

	unsigned char * Connection::getFileData() {
		return fileData;
	}

	TableHeader Connection::getTableHeader() {
		return TableHeader(this, fileData + *(reinterpret_cast<uint32_t*>(fileData+4)));
	}

	uint32_t Connection::getTableIndex(std::string tableName) {
		for (int i = 0; i < getTableCount(); ++i) {
			if (static_cast<std::string>(getTableHeader().getTableName(i)) == tableName) {
				return i;
			}
		}
		return EOF;
	}

	RowTopHeader Connection::getRows(uint32_t indexOfTable) {
		return getTableHeader().getRowTopHeader(indexOfTable);
	}

	//Concept
	QueryResult Connection::Query(uint32_t indexOfTable, bool compare(std::string columnName, std::string columnVal)) {
		QueryResult result;
		try {
			RowTopHeader rowTopHeader = getRows(indexOfTable);
			uint32_t max_row_count = rowTopHeader.getRowCount();
			if (rowTopHeader.isRowHeaderValid()) {
				for (int i = 0; i < max_row_count; ++i) {
					if (rowTopHeader.getRowHeader().isRowInfoValid(i)) {
						RowInfo rowInfo = rowTopHeader[i];
						int32_t column_count = rowInfo.getColumnCount();
						for (std::ptrdiff_t j = 0; j < column_count; ++j) {
							if (compare(rowInfo.getColumnName(j), static_cast<FieldValue>(rowInfo[j]))) {
								result.push_back(rowInfo);
							}
						}
					}
				}
			}
			else {
				Logger::log("FDB", "Couldn't find table on the FDB.", LogType::ERR);
			}
		}
		catch (std::exception e) {
			Logger::log("FDB", "Error on FDB Query (" + std::string(e.what()) + ")", LogType::ERR);
		}
		return result;
	}

	QueryResult Connection::Query(std::string tablename, bool compare(std::string columnName, std::string columnVal)) {
		return Query(getTableIndex(tablename), compare);
	}

	/*void TestQuery() {
		Query("test", ([](std::string a, std::string b)-> bool {
			return a == "id"&&std::stoi(b) > 50;
		}));
	}*/
}
