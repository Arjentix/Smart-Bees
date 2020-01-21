/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 27.09.2019
*/

#include "SimpleSQL.h"


#include <stdexcept>
#include <algorithm>
#include <iostream>

using namespace SimpleSQL;

QueryResult::QueryResult()
{
	_res_ptr = nullptr;
}

QueryResult::QueryResult(MYSQL_RES* res_ptr) : _res_ptr(res_ptr) {}

QueryResult::QueryResult(QueryResult&& other)
{
	*this = std::move(other);
}

QueryResult::~QueryResult()
{
	free();
}

Row QueryResult::get_row()
{
	if (_res_ptr != nullptr) {
		return mysql_fetch_row(_res_ptr);
	}

	throw std::runtime_error("QueryResult was released, can't fetch row");
}

void QueryResult::free()
{
	if (_res_ptr != nullptr) {
		mysql_free_result(_res_ptr);
		_res_ptr = nullptr;
	}
}

QueryResult& QueryResult::operator=(QueryResult&& other)
{
	mysql_free_result(_res_ptr);

	_res_ptr = other._res_ptr;
	other._res_ptr = nullptr;

	return *this;
}


Connector::Connector() : _last_res_ptr(nullptr), _connected(false)
{
	_conn_ptr = mysql_init(nullptr);
	if (_conn_ptr == nullptr) {
		throw std::runtime_error(
			"MySQL initialization failed: " +
			std::string(mysql_error(_conn_ptr))
		);
	}
}

bool Connector::connect(
	const std::string& host,
	const std::string& user,
	const std::string& passwd,
	const std::string& db,
	unsigned int port,
	const std::string& unix_socket,
	unsigned int client_flag
)
{
	const char* tmp_unix_socket = (unix_socket.empty() ? 
		nullptr : unix_socket.c_str()
	);

	my_bool reconnect = 1;
	mysql_options(_conn_ptr, MYSQL_OPT_RECONNECT, &reconnect);

	MYSQL* res = mysql_real_connect(
		_conn_ptr, 
		host.c_str(),
		user.c_str(),
		passwd.c_str(),
		db.c_str(),
		port,
		tmp_unix_socket,
		client_flag
	);
	if (res != nullptr) {
		_connected = true;
	}

	return _connected;
}

bool Connector::is_connected() {
	return _connected;
}

void Connector::ping_db() {
	mysql_ping(_conn_ptr);
}

Connector::~Connector()
{
	mysql_close(_conn_ptr);
}

std::shared_ptr<QueryResult> Connector::query(const std::string& query_str)
{
	if (_last_res_ptr != nullptr) {
		_last_res_ptr->free();
	}
	int res = mysql_query(_conn_ptr, query_str.c_str());

	if (res != 0) {
		throw std::runtime_error(
			"MySQL query failed: " +
			std::string(mysql_error(_conn_ptr))
		);
	}

	_last_res_ptr = std::make_shared<QueryResult>(mysql_use_result(_conn_ptr));
	return _last_res_ptr;
}
