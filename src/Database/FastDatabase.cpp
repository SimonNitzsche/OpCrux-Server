#include "FastDatabase.hpp"
#include "Utils/FileUtils.hpp"
namespace FDB {
	
	/*
		Class: PointerString
	*/

	PointerString::PointerString(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {
		memlocation = conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation);
		length = strlen(reinterpret_cast<const char*>(memlocation));
	}

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
		return *memlocation;
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

	/*
		Class: RowDataHeader
	*/
	
	/*
		Class: RowInfo
	*/
	
	bool RowInfo::isRowDataHeaderValid() {
		return *reinterpret_cast<uint32_t*>(memlocation);
	}

	bool RowInfo::isLinkedRowInfoValid() {
		return *reinterpret_cast<uint32_t*>(memlocation + 4);
	}

	/*
		Class: RowHeader
	*/

	bool RowHeader::isRowInfoValid(uint32_t indexOfRow) {
		return *reinterpret_cast<uint32_t*>(memlocation + (4*indexOfRow));
	}

	/*
		Class: RowTopHeader
	*/

	uint32_t RowTopHeader::getRowCount() {
		return *reinterpret_cast<uint32_t*>(memlocation);
	}

	RowHeader RowTopHeader::getRowHeader() {
		int32_t offset = *reinterpret_cast<int32_t*>(memlocation + 4);
		if (offset < 0)
			throw std::runtime_error("RowHeader is invalid.");
		RowHeader(conn, conn->getFileData() + offset);
	}

	bool RowTopHeader::isRowHeaderValid() {
		return *reinterpret_cast<int32_t*>(memlocation + 4) > -1;
	}

	/*
		Class: TableHeader
	*/

	ColumnHeader TableHeader::getColumnHeader(uint32_t indexOfTable) {
		return ColumnHeader(conn, conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation + (8 * indexOfTable)) );
	}

	RowTopHeader TableHeader::getRowTopHeader(uint32_t indexOfTable) {
		return RowTopHeader(conn, conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation + (8 * indexOfTable) + 4) );
	}

	/*
		Class: Connection
	*/

	Connection::Connection(std::string database) {
		fileData = FileUtils::ReadFileCompletely(database);
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
}