#pragma once

#include <iostream>
#include <string>
#include "SimpleSQL.h"
#include <ctime>
#include <chrono>
#include <typeinfo>

class DataBase {

private:

	const char* HOSTNAME;
    const char* DATABASE;
    const char* USERNAME;
    const char* PASSWORD;
    const char* SOCKET;
	bool connected;
    enum {
        PORT_NO = 3306,
        OPT = 0
    };
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

	SimpleSQL::Connector* dbc;

public:
	DataBase();
	~DataBase();
	void init(const char*, const char*, const char*, const char*);
	bool check_for_sub(int);
	void update_sub(int, time_t, time_t);
	void insert_sub(std::string, int, time_t, time_t);
	void delete_sub(int);
	std::string time_to_string(time_t);
};
