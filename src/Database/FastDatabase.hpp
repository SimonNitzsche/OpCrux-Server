#ifndef __FASTDATABASE_HPP_
#define __FASTDATABASE_HPP_

#include <string>

namespace FDB {

	class Connection;

	enum class DATA_TYPE : uint32_t {
		NOTHING = 0,
		INTEGER,
		UNKNOWN1,
		FLOAT,
		TEXT,
		BOOLEAN,
		BIGINT,
		UNKNOWN2,
		VARCHAR
	};

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
	public:
		ColumnData(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		DATA_TYPE getColumnDataType(uint32_t indexOfColumn);
		PointerString getColumnName(uint32_t indexOfColumn);
	};

	class ColumnHeader {
	private:
		Connection * conn;
		unsigned char* memlocation;
	public:
		ColumnHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		uint32_t getColumnCount();
		PointerString getTableName();
		ColumnData getColumnData();
	};

	class RowInfo {
	private:
		Connection * conn;
		unsigned char * memlocation;
	public:
		RowInfo(Connection * connection, unsigned char * where): conn(connection), memlocation(where) {}
		bool isRowDataHeaderValid();
		bool isLinkedRowInfoValid();
	};

	class RowHeader {
	private:
		Connection * conn;
		unsigned char * memlocation;
	public:
		RowHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		RowInfo getRowInfo(uint32_t indexOfRow);
		bool isRowInfoValid(uint32_t indexOfRow);
	};

	class RowTopHeader {
	private:
		Connection * conn;
		unsigned char * memlocation;
	public:
		RowTopHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		uint32_t getRowCount();
		RowHeader getRowHeader();
		bool isRowHeaderValid();
	};

	class TableHeader {
	private:
		Connection * conn;
		unsigned char* memlocation;
	public:
		TableHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}

		ColumnHeader getColumnHeader(uint32_t indexOfTable);
		RowTopHeader getRowTopHeader(uint32_t indexOfTable);
	};

	class Connection {
	private:
		unsigned char * fileData;
	public:
		Connection(std::string database);

		uint32_t getTableCount();

		unsigned char * getFileData();

		TableHeader getTableHeader();

		~Connection() {
			delete fileData;
		}
	};
}

#endif // !__FASTDATABASE_HPP_