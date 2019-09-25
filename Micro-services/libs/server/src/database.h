#pragma once

#include <iostream>
#include <mysql/mysql.h>
#include <string>

using namespace std;

class DataBase {

private:

	const char* MY_HOSTNAME;
    const char* MY_DATABASE;
    const char* MY_USERNAME;
    const char* MY_PASSWORD;
    const char* MY_SOCKET;
    enum {
        MY_PORT_NO = 3306,
        MY_OPT = 0
    };
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

public:
	DataBase();
	~DataBase();
	void init(const char*, const char*, const char*, const char*);
	bool exec();
};
