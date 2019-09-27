/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 27.09.2019
*/

#include <mysql.h>
#include <unistd.h>
#include <iostream>

#define HOST			"mysql"
#define USER_NAME		"Tester"
#define PASSWORD		"secret"
#define DB_NAME			"TestDb"
#define PORT			3306
#define ATTEMPTS_COUNT	50

using namespace std;

int main() {
	cout << "Start" << endl;

	MYSQL* conn = mysql_init(NULL);
	if (conn == NULL) {
		cout << "Initialization failed" << endl;
		exit(-1);
	}

	MYSQL* connect_res;
	for (int i = 0; i < ATTEMPTS_COUNT; ++i) {
		cout << "Trying to connect: attempt #" << i + 1 << endl;
		connect_res = mysql_real_connect(conn, HOST, USER_NAME, PASSWORD, DB_NAME, PORT, NULL, 0);
		if (connect_res != NULL) {
			break;
		}
		sleep(1);
	}
	if (connect_res == NULL) {
		cout << "Connection failed after " << ATTEMPTS_COUNT << " attempts. "
				<< mysql_error(connect_res) << endl;
		exit(-2);
	}

	// Writing new data
	int query_res = mysql_query(conn, "INSERT INTO test_table(Data) VALUES(\"Generated data\")");
	if (query_res != 0) {
		cout << "Query failed: " << mysql_error(conn) << endl;
		exit(-4);
	}

	// Reading last data
	query_res = mysql_query(conn, "SELECT * FROM test_table ORDER BY Id DESC;");
	if (query_res != 0) {
		cout << "Query failed: " << mysql_error(conn) << endl;
		exit(-4);
	}

	MYSQL_RES* mysql_res = mysql_use_result(conn);
	MYSQL_ROW mysql_row = mysql_fetch_row(mysql_res);

	cout << "Id: " << mysql_row[0] << ", data: " << mysql_row[1] << endl;

	mysql_free_result(mysql_res);
	mysql_close(conn);

	cout << "End" << endl;
	return 0;
}