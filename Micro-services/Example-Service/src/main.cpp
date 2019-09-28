/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 27.09.2019
*/

#include "SimpleSQL.h"

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
	try {
		cout << "Start" << endl;

		SimpleSQL::Connector db;

		for (int i = 0; i < ATTEMPTS_COUNT; ++i) {
			cout << "Trying to connect: attempt #" << i + 1 << endl;
			db.connect(HOST, USER_NAME, PASSWORD, DB_NAME, PORT);
			if (db.is_connected()) {
				break;
			}

			sleep(1);
		}
		if (!db.is_connected()) {
			cout << "Connection failed after " << ATTEMPTS_COUNT << " attempts."
				<< endl;
			exit(-2);
		}

		// Writing new data
		db.query("INSERT INTO test_table(Data) VALUES(\"Generated data\")");

		// Reading last data
		auto query_res_ptr = db.query("SELECT * FROM test_table ORDER BY Id DESC;");
		SimpleSQL::Row row = query_res_ptr->get_row();

		cout << "Id: " << row[0] << ", data: " << row[1] << endl;

		cout << "End" << endl;
		return 0;
	}
	catch(exception& ex) {
		cout << ex.what() << endl;
	}
}