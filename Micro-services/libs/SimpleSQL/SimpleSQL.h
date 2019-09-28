/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 27.09.2019
*/

/**
 * Here declared namespace SimpleSQL, which contains some tools, that can
 * help you to connect to a MySQL database and to send queries.
 * All this library is a wrapper on MySQL C API.
 * For more information read classes and methods description.
 * 
 * !!! You can find some usage examples at the test directory!!!
*/

#pragma once

#include <mysql.h>
#include <string>
#include <memory>

/**
 * namespace SimpleSQL - contains synonym Row, struct QueryResult and class
 * Connector. Provides possibility to connect to a MySQL database, send queries
 * and read answers.
 * Some of the methods can throw an exception, so be careful and read the 
 * documentation.
*/
namespace SimpleSQL {
	/**
	 * Row - synonym of MYSQL_ROW (MySQL C API structure).
	 * Most important thing is that you can access to the row's field by
	 * usin [] operator.
	 * For example:
	 *		Row row = ...;
	 *		std::cout << "Column 1: " << row[0] << std::endl;
	*/
	using Row = MYSQL_ROW;

	/**
	 * QueryResylt - contains result of a query. In fact it's just a smart
	 * wrapper on the MYQSL_RES*(MySQL C API structure pointer) with less but
	 * simpler functional.
	 * It's a non-copy class! It's possible only to move information from
	 * another object.
	*/
	struct QueryResult {
		QueryResult();
		explicit QueryResult(MYSQL_RES* res_ptr);

		// Can't copy information of the result into another object
		QueryResult(const QueryResult& other) = delete;

		// It's possible only to move other object information
		QueryResult(QueryResult&& other);

		~QueryResult();


		/**
		 * get_row() - extracts next row of the result.
		 * Throws a std::runtime_error exception if result was released.
		*/
		Row get_row();

		/**
		 * free() - free result memory.
		 * Actualy in application code you don't need this method.
		*/
		void free();


		// Can't copy information of the result into another object
		QueryResult& operator=(const QueryResult& other) = delete;

		// It's possible only to move other object information
		QueryResult& operator=(QueryResult&& other);
	private:
		MYSQL_RES* _res_ptr;
	};

	/**
	 * Connector - connects to the MySQL database and send queries to it.
	*/
	class Connector {
	public:
		/**
		 * Connector() - throws a std::runtime_error exception if can't connect
		 * to the database.
		*/
		Connector(
			const std::string& host,
			const std::string& user,
			const std::string& passwd,
			const std::string& db,
			unsigned int port = 0,
			const std::string& unix_socket = "",
			unsigned int client_flag = 0
		);

		~Connector();

		/**
		 * query() - sends given query to the connected database.
		 * Return type is pointer cause class Connector stores information about
		 * last query result and releases its memory in next query() call.
		 * Throws a std::runtime_error exception if query was failed.
		*/
		std::shared_ptr<QueryResult> query(const std::string& query_str);

	private:
		MYSQL* _conn_ptr;
		std::shared_ptr<QueryResult> _last_res_ptr;
	};
}
