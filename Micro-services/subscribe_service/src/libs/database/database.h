#pragma once

#include <iostream>
#include <string>
#include "SimpleSQL.h"
#include <ctime>
#include <chrono>
#include <typeinfo>
#include <exception>

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

	SimpleSQL::Connector* dbc;

	std::string time_to_string(time_t);
	void check_for_exist(int);
	void check_for_exist(std::string);
public:
	DataBase();
	~DataBase();
	void init(const char*, const char*, const char*, const char*);
	bool check_for_sub(int);
	void update_sub(int, time_t, time_t);
	int insert_sub(std::string, time_t, time_t);
	void delete_sub(int);
};
