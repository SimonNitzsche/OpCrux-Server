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
		Class: ColumnHeader
	*/
	
	uint32_t ColumnHeader::getColumnCount() {
		return *memlocation;
	}

	PointerString ColumnHeader::getTableName() {
		return PointerString(conn, memlocation + 4);
	}

	/*
		Class: TableHeader
	*/

	ColumnHeader TableHeader::getColumnHeader(uint32_t indexOfTable) {
		return ColumnHeader(conn, conn->getFileData() + *reinterpret_cast<uint32_t*>(memlocation + (8 * indexOfTable)) );
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