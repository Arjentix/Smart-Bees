/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 27.09.2019
*/

#include "SimpleSQL.h"
#include "test_runner.h"

#include <sstream>

#define HOST "db4free.net"
#define USER "arjentix"
#define PASSWD "verysecret"
#define DB "arjentix_test_db"

using namespace std;

void test_basic()
{
	SimpleSQL::Connector db;
	if (db.connect(HOST, USER, PASSWD, DB)) {
		auto query_res_ptr = db.query("SELECT * FROM sample_table");
		while (auto row = query_res_ptr->get_row()) {
			static int i = 1;
			stringstream result;
			result << row[0] << " - " << row[1];
			ASSERT_EQUAL(result.str(), to_string(i) + " - data " + to_string(i));
			++i;
		}
	}
}

void test_multiple_queries()
{
	SimpleSQL::Connector db;
	db.connect(HOST, USER, PASSWD, DB);

	if(db.is_connected()) {
		auto query_res_ptr = db.query("SELECT * FROM sample_table WHERE Id = 1");
		ASSERT_EQUAL(string(query_res_ptr->get_row()[1]), "data 1");

		query_res_ptr = db.query("SELECT * FROM sample_table WHERE Id = 2");
		ASSERT_EQUAL(string(query_res_ptr->get_row()[1]), "data 2");
	}
}

void test_non_need_result()
{
	SimpleSQL::Connector db;
	db.connect(HOST, USER, PASSWD, DB);

	if(db.is_connected()) {
		auto query_res_ptr = db.query("SELECT * FROM sample_table WHERE Id = 1");

		query_res_ptr = db.query("SELECT * FROM sample_table WHERE Id = 2");
		ASSERT_EQUAL(string(query_res_ptr->get_row()[1]), "data 2");
	}
}

void test_insert_and_delete()
{
	SimpleSQL::Connector db;
	db.connect(HOST, USER, PASSWD, DB);

	if(db.is_connected()) {
		db.query("INSERT INTO sample_table(Data) VALUES (\"my data\");");
		auto query_res_ptr = db.query("DELETE FROM sample_table WHERE Data = \"my data\"");
	}
}

void test_wrong_query()
{
	SimpleSQL::Connector db;
	db.connect(HOST, USER, PASSWD, DB);

	if(db.is_connected()) {
		try {
			auto query_res_ptr = db.query("INSERT INTO wrong_table(Data) VALUES (\"my data\");");
		}
		catch (runtime_error&) {

		}
		catch(...) {
			throw runtime_error("Expected runtime_error exception");
		}
	}
}

void test_get_row_on_released_res()
{
	SimpleSQL::Connector db;
	db.connect(HOST, USER, PASSWD, DB);

	if(db.is_connected()) {
		auto query_res_ptr = db.query("SELECT * FROM sample_table WHERE Id = 1");
		db.query("SELECT * FROM sample_table WHERE Id = 2");

		try {
			query_res_ptr->get_row();
		}
		catch (runtime_error&) {

		}
		catch(...) {
			throw runtime_error("Expected runtime_error exception");
		}
	}
}

int main()
{
	cout << "Test can take a few moments, " 
			"cause it connects to the db4free.net database..." << endl;

	TestRunner tr;
	RUN_TEST(tr, test_basic);
	RUN_TEST(tr, test_multiple_queries);
	RUN_TEST(tr, test_non_need_result);
	RUN_TEST(tr, test_insert_and_delete);
	RUN_TEST(tr, test_wrong_query);
	RUN_TEST(tr, test_get_row_on_released_res);

	return 0;
}
