#include "database.h"

DataBase::DataBase() {}

DataBase::~DataBase() {}

void DataBase::init(const char* hostname, const char* db_name, const char* user, const char* pass) {
	try {
		HOSTNAME = hostname;
		DATABASE = db_name;
    	USERNAME = user;
    	PASSWORD = pass;
    	SOCKET = NULL;

		dbc = new SimpleSQL::Connector();
		if(!dbc->connect(HOSTNAME, USERNAME, PASSWORD, DATABASE, PORT_NO))
			std::cout << "ERROR with connecting database " << db_name << std::endl;
	/*
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, HOSTNAME, USERNAME, PASSWORD, 
					DATABASE, PORT_NO, SOCKET, OPT)) {
			std::cerr << mysql_error(conn) << std::endl;
        }
		else {
			connected = true;
			std::cout << "DataBase " << db_name << " successfully connected" << std::endl;
		}
		*/

    } catch (char *e) {
		std::cerr << "[EXCEPTION] " << e << std::endl;
    }
}

bool DataBase::check_for_sub(int id) {
	try {
		struct tm tm;
		if(dbc->is_connected()) {
			std::string request = "SELECT sub_end_date FROM subs_table WHERE user_id=" + std::to_string(id) + ";";
			auto query_res_ptr = dbc->query(request);
			auto row = query_res_ptr->get_row();
			strptime(std::string(row[0]).c_str(), "%F %T", &tm);
			time_t end_sub = mktime(&tm);
			time_t now = time(NULL);
			return end_sub > now;
		}
    } catch (char *e) {
		std::cerr << "[EXCEPTION] " << e << std::endl;
    }
}

void DataBase::update_sub(int id, time_t s_start, time_t s_end) {
	try {
		if(dbc->is_connected()) {
			std::string request = "UPDATE subs_table SET sub_start_date=" + time_to_string(s_start) + ", sub_end_date=" + time_to_string(s_end) + " WHERE user_id=" + std::to_string(id) + ";";
			dbc->query(request);
		}
    } catch (char *e) {
		std::cerr << "[EXCEPTION] " << e << std::endl;
    }
}
void DataBase::insert_sub(std::string username, int id, time_t s_start, time_t s_end) {
	try {
		if(dbc->is_connected()) {
			std::string request = "INSERT INTO subs_table(user_name, user_id, sub_start_date, sub_end_date) VALUES (" + username + ", " + std::to_string(id) + ", " + time_to_string(s_start) + ", " + time_to_string(s_end) + ");";
			dbc->query(request);
		}
    } catch (char *e) {
		std::cerr << "[EXCEPTION] " << e << std::endl;
    }

}
void DataBase::delete_sub(int id) {
	try {
		if(dbc->is_connected()) {
			std::string request = "DELETE FROM subs_table WHERE user_id=" + std::to_string(id) + ";";
			auto query_res_ptr = dbc->query(request);
		}
    } catch (char *e) {
		std::cerr << "[EXCEPTION] " << e << std::endl;
    }
}

std::string DataBase::time_to_string(time_t t) {

	struct tm *tm = localtime(&t);
	char c[80];
	strftime(c, 80, "%F %T", tm);
	return std::string(c);

}


/*
void DataBase::exec() {
	try {
        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(
                conn,
                MY_HOSTNAME, MY_USERNAME,
                MY_PASSWORD, MY_DATABASE,
                MY_PORT_NO, MY_SOCKET, MY_OPT)) {
            cerr << mysql_error(conn) << endl;
            return false;
        }

        // Execute a sql statement
        if (mysql_query(conn, "SHOW TABLES")) {
            cerr << mysql_error(conn) << endl;
            return false;
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        cout << "* MySQL - SHOW TABLES in `"
             << MY_DATABASE << "`" << endl;
        while ((row = mysql_fetch_row(res)) != NULL)
            cout << row[0] << endl;

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);
    } catch (char *e) {
        cerr << "[EXCEPTION] " << e << endl;
        return false;
    }
    return true;
}
*/
