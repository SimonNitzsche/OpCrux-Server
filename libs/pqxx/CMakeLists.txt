set(PQXX_SRC
		array
		array.cxx
		array.hxx
		basic_connection
		basic_connection.hxx
		binarystring
		binarystring.cxx
		binarystring.hxx
		compiler-internal.hxx
		compiler-internal-post.hxx
		compiler-internal-pre.hxx
		compiler-public.hxx
		config-internal-compiler.h
		config-public-compiler.h
		connection
		connection.cxx
		connection.hxx
		connection_base
		connection_base.cxx
		connection_base.hxx
		connectionpolicy
		connectionpolicy.hxx
		cursor
		cursor.cxx
		cursor.hxx
		dbtransaction
		dbtransaction.cxx
		dbtransaction.hxx
		errorhandler
		errorhandler.cxx
		errorhandler.hxx
		except
		except.cxx
		except.hxx
		field
		field.cxx
		field.hxx
		isolation
		isolation.hxx
		largeobject
		largeobject.cxx
		largeobject.hxx
		nontransaction
		nontransaction.cxx
		nontransaction.hxx
		notification
		notification.cxx
		notification.hxx
		pipeline
		pipeline.cxx
		pipeline.hxx
		pqxx
		prepared_statement
		prepared_statement.cxx
		prepared_statement.hxx
		result
		result.cxx
		result.hxx
		result_iterator.hxx
		robusttransaction
		robusttransaction.cxx
		robusttransaction.hxx
		row
		row.cxx
		row.hxx
		statement_parameters.cxx
		strconv
		strconv.cxx
		strconv.hxx
		subtransaction
		subtransaction.cxx
		subtransaction.hxx
		tablereader
		tablereader.cxx
		tablereader.hxx
		tablestream
		tablestream.cxx
		tablestream.hxx
		tablewriter
		tablewriter.cxx
		tablewriter.hxx
		transaction
		transaction.cxx
		transaction.hxx
		transaction_base
		transaction_base.cxx
		transaction_base.hxx
		transactor
		transactor.hxx
		types
		types.hxx
		util
		util.cxx
		util.hxx
		version
		version.cxx
		version.hxx
		internal/callgate.hxx
		internal/libpq-forward.hxx
		internal/statement_parameters.hxx
		internal/gates/connection-dbtransaction.hxx
		internal/gates/connection-errorhandler.hxx
		internal/gates/connection-largeobject.hxx
		internal/gates/connection-notification_receiver.hxx
		internal/gates/connection-parameterized_invocation.hxx
		internal/gates/connection-pipeline.hxx
		internal/gates/connection-prepare-invocation.hxx
		internal/gates/connection-reactivation_avoidance_exemption.hxx
		internal/gates/connection-sql_cursor.hxx
		internal/gates/connection-transaction.hxx
		internal/gates/errorhandler-connection.hxx
		internal/gates/icursor_iterator-icursorstream.hxx
		internal/gates/icursorstream-icursor_iterator.hxx
		internal/gates/result-connection.hxx
		internal/gates/result-creation.hxx
		internal/gates/result-row.hxx
		internal/gates/result-sql_cursor.hxx
		internal/gates/transaction-subtransaction.hxx
		internal/gates/transaction-tablereader.hxx
		internal/gates/transaction-tablewriter.hxx
		internal/gates/transaction-transactionfocus.hxx
)

add_library(pqxx ${PQXX_SRC})

find_package(PostgreSQL REQUIRED)

if(PostgreSQL_FOUND)
	include_directories(${PostgreSQL_INCLUDE_DIRS})
	link_directories(${PostgreSQL_LIBRARY_DIRS})
endif()

message(STATUS "---------------------------")
message(STATUS "PostgreSQL_FOUND = ${PostgreSQL_FOUND}")
message(STATUS "${PostgreSQL_INCLUDE_DIRS}")
message(STATUS "${PostgreSQL_LIBRARY_DIRS}")
message(STATUS "${PostgreSQL_LIBRARIES}")
message(STATUS "---------------------------")

target_link_libraries(pqxx ${PostgreSQL_LIBRARY})
