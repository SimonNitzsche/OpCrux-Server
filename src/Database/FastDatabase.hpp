#ifndef __FASTDATABASE_HPP_
#define __FASTDATABASE_HPP_

#include <string>

namespace FDB {

	class Connection;

	class PointerString {
	private:
		Connection * conn;
		unsigned char * memlocation;
		uint32_t length;
	public:
		PointerString();
		PointerString(Connection * connection, unsigned char * where);

		operator std::string() const {
			return std::string(reinterpret_cast<const char*>(memlocation), length);
		}
	};

	class ColumnData {
	private:
		Connection * conn;
		unsigned char * memlocation;
		ColumnData(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
	};

	class ColumnHeader {
	private:
		Connection * conn;
		unsigned char* memlocation;
	public:
		ColumnHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		uint32_t getColumnCount();
		PointerString getTableName();
		//ColumnData getColumnData();
	};

	class TableHeader {
	private:
		Connection * conn;
		unsigned char* memlocation;
	public:
		TableHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}

		ColumnHeader getColumnHeader(uint32_t indexOfTable);

	};

	class Connection {
	private:
		unsigned char * fileData;
	public:
		Connection(std::string database);

		uint32_t getTableCount();

		unsigned char * getFileData();

		TableHeader getTableHeader();

		Connection() {
			delete fileData;
		}
	};
}

#endif // !__FASTDATABASE_HPP_