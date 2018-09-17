#ifndef __FASTDATABASE_HPP_
#define __FASTDATABASE_HPP_

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

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
		VARCHAR,
		INVALID_POINTER = 0xffffffff
	};

	class RowData;

	class FieldValue {
	private:
		Connection * conn;
		DATA_TYPE dataType;
		int32_t * data;
	public:
		FieldValue(Connection * connection, DATA_TYPE type, int32_t * where) : conn(connection), dataType(type), data(where) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return reinterpret_cast<unsigned char*>(data); }

		std::string ToString();

		operator std::string() {
			return ToString();
		}
	};

	class PointerString {
	private:
		Connection * conn;
		unsigned char * memlocation;
		uint32_t length;
	public:
		PointerString();
		PointerString(Connection * connection, unsigned char * where);
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

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
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		DATA_TYPE getColumnDataType(uint32_t indexOfColumn);
		PointerString getColumnName(uint32_t indexOfColumn);
	};

	class ColumnHeader {
	private:
		Connection * conn;
		unsigned char* memlocation;
	public:
		ColumnHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		uint32_t getColumnCount();
		PointerString getTableName();
		ColumnData getColumnData();
	};

	class RowData {
	private:
		Connection * conn;
		unsigned char * memlocation;
	public:
		RowData(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		DATA_TYPE getColumnDataType(uint32_t indexOfColumn);
		FieldValue getColumnData(uint32_t indexOfColumn);
	};

	class RowDataHeader {
	private:
		Connection * conn;
		unsigned char * memlocation;
	public:
		RowDataHeader(Connection * connection, unsigned char * where): conn(connection), memlocation(where) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		bool isRowDataValid();
		int32_t getColumnCount();
		RowData getRowData();

		DATA_TYPE getColumnDataType(uint32_t indexOfColumn) { return getRowData().getColumnDataType(indexOfColumn); }
		FieldValue getColumnData(uint32_t indexOfColumn) { return getRowData().getColumnData(indexOfColumn); }
	};

	class RowInfo {
	private:
		Connection * conn;
		unsigned char * memlocation;
		unsigned char * columnHeaderAddr;
	public:
		RowInfo() : conn(nullptr), memlocation(nullptr), columnHeaderAddr(nullptr) {}
		RowInfo(Connection * connection, unsigned char * where, unsigned char * columnHeader): conn(connection), memlocation(where), columnHeaderAddr(columnHeader) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		bool isValid();
		bool isRowDataHeaderValid();
		bool isLinkedRowInfoValid();

		RowDataHeader getRowDataHeader();
		RowInfo	getLinkedRowInfo();

		RowData getRowData() { return getRowDataHeader().getRowData(); }
			DATA_TYPE getColumnDataType(uint32_t indexOfColumn) { return getRowData().getColumnDataType(indexOfColumn); }
			FieldValue getRowData(uint32_t indexOfColumn) { return getRowData().getColumnData(indexOfColumn); }
			int32_t getColumnCount() { return getRowDataHeader().getColumnCount(); }
			PointerString getColumnName(uint32_t indexOfColumn) { return ColumnHeader(conn, columnHeaderAddr).getColumnData().getColumnName(indexOfColumn); }

		FieldValue operator [] (uint32_t indexOfColumn) {
			return getRowData(indexOfColumn);
		}

		FieldValue operator [] (std::string columnName) {
			ColumnHeader columnHeader = ColumnHeader(conn, columnHeaderAddr);
			int32_t column_count = getRowDataHeader().getColumnCount();
			for (int i = 0; i < column_count; ++i) {
				if (static_cast<std::string>(columnHeader.getColumnData().getColumnName(i)) == columnName) {
					return getRowData(i);
				}
			}
			throw std::runtime_error("Field \"" + columnName + "\" can not be found.");
		}
	};

	class RowHeader {
	private:
		Connection * conn;
		unsigned char * memlocation;
		unsigned char * columnHeaderAddr;
	public:
		RowHeader(Connection * connection, unsigned char * where, unsigned char * columnHeader) : conn(connection), memlocation(where), columnHeaderAddr(columnHeader) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		RowInfo getRowInfo(uint32_t indexOfRow);
		bool isRowInfoValid(uint32_t indexOfRow);
	};

	class RowTopHeader {
	private:
		Connection * conn;
		unsigned char * memlocation;
		unsigned char * columnHeaderAddr;
	public:
		RowTopHeader(Connection * connection, unsigned char * where, unsigned char * columnHeader) : conn(connection), memlocation(where), columnHeaderAddr(columnHeader) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		uint32_t getRowCount();
		RowHeader getRowHeader();
		bool isRowHeaderValid();

		RowInfo operator [] (uint32_t indexOfRow) {
			return getRowHeader().getRowInfo(indexOfRow);
		}
	};

	class TableHeader {
	private:
		Connection * conn;
		unsigned char* memlocation;
	public:
		TableHeader(Connection * connection, unsigned char * where) : conn(connection), memlocation(where) {}
		Connection * getConnection() { return conn; }
		unsigned char * getMemoryLocation() { return memlocation; }

		ColumnHeader getColumnHeader(uint32_t indexOfTable);
		RowTopHeader getRowTopHeader(uint32_t indexOfTable);

		PointerString getTableName(uint32_t indexOfTable) { return getColumnHeader(indexOfTable).getTableName(); }
	};

	typedef std::vector<RowInfo> QueryResult;

	class Connection {
	private:
		std::unique_ptr<unsigned char[]> filePtr;
		unsigned char * fileData = nullptr;
	public:
		Connection(std::string database);
		Connection() {}
		void Connect(std::string database);
		bool isConnected() { return fileData != nullptr; }
		uint32_t getTableCount();

		unsigned char * getFileData();

		TableHeader getTableHeader();

		uint32_t getTableIndex(std::string tableName);
		ColumnHeader getColumns(uint32_t indexOfTable) { return getTableHeader().getColumnHeader(indexOfTable); }
		RowTopHeader getRows(uint32_t indexOfTable);
		RowTopHeader getRows(std::string tableName) { return getRows(getTableIndex(tableName)); }

		QueryResult Query(uint32_t indexOfTable, bool compare(std::string columnName, std::string columnVal));
		QueryResult Query(std::string tablename, bool compare(std::string columnName, std::string columnVal));

		~Connection() {}
	};
}

#endif // !__FASTDATABASE_HPP_