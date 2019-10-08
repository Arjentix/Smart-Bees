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
	//	if(!dbc->connect(HOSTNAME, USERNAME, PASSWORD, DATABASE, PORT_NO))
	//		std::cout << "ERROR with connecting database " << db_name << std::endl;
		dbc->connect(HOSTNAME, USERNAME, PASSWORD, DATABASE, PORT_NO);
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

    } catch (std::exception& e) {
		throw e;
    }
}

bool DataBase::time_compare(struct tm tm_n, struct tm tm_e) {
	if(tm_n.tm_year == tm_e.tm_year) {
		if(tm_n.tm_mon == tm_e.tm_mon) {
			if(tm_n.tm_mday == tm_e.tm_mday) {
				return true;
			} else return tm_n.tm_mday < tm_e.tm_mday;
		} else return tm_n.tm_mon < tm_e.tm_mon;
	} else return tm_n.tm_year < tm_e.tm_year;
}

bool DataBase::check_for_sub(int id) {
	try {
		if(dbc->is_connected()) {
			check_for_exist(id);
			struct tm tm_e;
			std::string request = "SELECT sub_end_date FROM subs_table WHERE user_id=" + std::to_string(id) + ";";
			auto query_res_ptr = dbc->query(request);
			auto row = query_res_ptr->get_row();
			strptime(std::string(row[0]).c_str(), "%F", &tm_e);
			time_t now = time(NULL);
			struct tm *tm_n = localtime(&now);

			return time_compare(*tm_n, tm_e);
		}
		else
			throw std::runtime_error("Database doesn't connected");
	} catch(std::runtime_error& e) {
		throw e;
    } catch (std::exception& e) {
		throw e;
    }
}

void DataBase::update_sub(int id, std::string s_start, std::string s_end) {
	try {
		if(dbc->is_connected()) {
			check_for_exist(id);
			std::string request = "UPDATE subs_table SET sub_start_date=\'" + s_start + "\', sub_end_date=\'" + s_end + "\' WHERE user_id=" + std::to_string(id) + ";";
			dbc->query(request);
		}
		else
			throw std::runtime_error("Database doesn't connected");
	} catch(std::runtime_error& e) {
		throw e;
    } catch (std::exception& e) {
		throw e;
    }
}
int DataBase::insert_sub(std::string username, std::string s_start, std::string s_end) {
	try {
		if(dbc->is_connected()) {
			check_for_exist(username);
			std::string request = "INSERT INTO subs_table(username, sub_start_date, sub_end_date) VALUES (\'" + username + "\', \'" + s_start + "\', \'" + s_end + "\');";
			dbc->query(request);
			request = "SELECT user_id FROM subs_table WHERE username=\'" + username + "\';";
			auto query_res_ptr = dbc->query(request);
			auto row = query_res_ptr->get_row();
			return std::stoi(std::string(row[0]));
		}
		else
			throw std::runtime_error("Database doesn't connected");
	} catch(std::runtime_error& e) {
		throw e;
    } catch (std::exception& e) {
		throw e;
    }

}
void DataBase::delete_sub(int id) {
	try {
		if(dbc->is_connected()) {
			
			check_for_exist(id);
			std::string request = "DELETE FROM subs_table WHERE user_id=" + std::to_string(id) + ";";
			dbc->query(request);
		}
		else {
			throw std::runtime_error("Database doesn't connected");
		}
	} catch(std::runtime_error& e) {
		throw e;
    } catch (std::exception& e) {
		throw e;
    }
}

std::string DataBase::time_to_string(time_t t) {

	struct tm *tm = localtime(&t);
	char c[80];
	strftime(c, 80, "%Y.%m.%d", tm);
	std::cout << std::string(c) << std::endl;
	return std::string(c);

}

void DataBase::check_for_exist(int id) {
	try {
		std::string request = "SELECT * FROM subs_table WHERE user_id=" + std::to_string(id) + ";";
		auto query_res_ptr = dbc->query(request);
		auto row = query_res_ptr->get_row();
		if(!row)
			throw std::runtime_error("User doesn't exist");
	} catch(std::runtime_error& e) {
		throw e;
	} catch(std::exception& e) {
		throw e;
	}
}

void DataBase::check_for_exist(std::string name) {
	try {
		std::string request = "SELECT * FROM subs_table WHERE username=\'" + name + "\';";
		auto query_res_ptr = dbc->query(request);
		auto row = query_res_ptr->get_row();
		if(row)
			throw std::runtime_error("User has already exist");
	} catch(std::runtime_error& e) {
		throw e;
	} catch(std::exception& e) {
		throw e;
	}
}
