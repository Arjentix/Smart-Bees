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
	bool time_compare(struct tm, struct tm);
	void check_for_exist(int);
	void check_for_exist(std::string);
public:
	DataBase();
	~DataBase();
	void init(const char*, const char*, const char*, const char*);
	bool check_for_sub(int);
	void update_sub(int, std::string,  std::string);
	void insert_sub(int, std::string, std::string, std::string);
	void delete_sub(int);

	class Time {
	public:
		Time() {}
		Time(int d, int h, int m) {
			days = d;
			hours = h;
			minutes = m;
		}

		int days;
		int hours;
		int minutes;
	};

	Time time_left(int);
private:
	Time time_left_counter(struct tm, struct tm);
};
