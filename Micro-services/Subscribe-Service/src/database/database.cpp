#include "database.h"

DataBase::DataBase() {}

DataBase::~DataBase() {}

void DataBase::init(const char* hostname, const char* db_name, const char* gate, const char* pass) {
	HOSTNAME = hostname;
	DATABASE = db_name;
   	USERNAME = gate;
   	PASSWORD = pass;
   	SOCKET = NULL;

	dbc = new SimpleSQL::Connector();
	if(!dbc->connect(HOSTNAME, USERNAME, PASSWORD, DATABASE, PORT_NO)) {
		char ex[] = "ERROR with connecting database ";
		throw std::runtime_error(std::strcat(ex, db_name));
	}
	dbc->connect(HOSTNAME, USERNAME, PASSWORD, DATABASE, PORT_NO);
}

bool DataBase::time_compare_end(struct tm tm_n, struct tm tm_e) {
	if(tm_n.tm_year == tm_e.tm_year) {
		if(tm_n.tm_mon == tm_e.tm_mon) {
			if(tm_n.tm_mday == tm_e.tm_mday) {
				if(tm_n.tm_hour == tm_e.tm_hour) {
					if(tm_n.tm_min == tm_e.tm_min) {
						return true;
					} else return tm_n.tm_min < tm_e.tm_min;
				} else return tm_n.tm_hour < tm_e.tm_hour;
			} else return tm_n.tm_mday < tm_e.tm_mday;
		} else return tm_n.tm_mon < tm_e.tm_mon;
	} else return tm_n.tm_year < tm_e.tm_year;
}

bool DataBase::time_compare_begin(struct tm tm_n, struct tm tm_b) {
	if(tm_n.tm_year == tm_b.tm_year) {
		if(tm_n.tm_mon == tm_b.tm_mon) {
			if(tm_n.tm_mday == tm_b.tm_mday) {
				if(tm_n.tm_hour == tm_b.tm_hour) {
					if(tm_n.tm_min == tm_b.tm_min) {
						return true;
					} else return tm_n.tm_min > tm_b.tm_min;
				} else return tm_n.tm_hour > tm_b.tm_hour;
			} else return tm_n.tm_mday > tm_b.tm_mday;
		} else return tm_n.tm_mon > tm_b.tm_mon;
	} else return tm_n.tm_year > tm_b.tm_year;
}

void DataBase::ping_db() {
	dbc->ping_db();
}

bool DataBase::check_for_sub(std::string gate_id) {
	if(dbc->is_connected()) {
		check_for_exist(gate_id);
		struct tm tm_b;
		struct tm tm_e;
		std::string request = "SELECT sub_start_date, sub_end_date FROM subs_table WHERE gate_id='" + gate_id + "';";
		auto query_res_ptr = dbc->query(request);
		auto row = query_res_ptr->get_row();
		strptime(std::string(row[0]).c_str(), "%F %H:%M", &tm_b);
		strptime(std::string(row[1]).c_str(), "%F %H:%M", &tm_e);
		time_t now = time(NULL);
		struct tm *tm_n = localtime(&now);

		return time_compare_begin(*tm_n, tm_b) & time_compare_end(*tm_n, tm_e);
	}
	else
		throw std::runtime_error("Database doesn't connected");
}

DataBase::Time DataBase::time_left_counter(struct tm tm_n, struct tm tm_e) {
	long long all_seconds = difftime(mktime(&tm_e), mktime(&tm_n));
	DataBase::Time t_r;
	t_r.minutes = (all_seconds/60)%60;
	t_r.hours = (all_seconds/(60*60))%24;
	t_r.days = (all_seconds/(60*60*24));
	return t_r;
}

DataBase::Time DataBase::time_left(std::string gate_id) {
	if(dbc->is_connected()) {
		check_for_exist(gate_id);
		if(check_for_sub(gate_id) == 0)
			return Time(0, 0, 0);
		struct tm tm_e;
		std::string request = "SELECT sub_end_date FROM subs_table WHERE gate_id='" + gate_id + "';";
		auto query_res_ptr = dbc->query(request);
		auto row = query_res_ptr->get_row();
		strptime(std::string(row[0]).c_str(), "%F %H:%M", &tm_e);
		time_t now = time(NULL);
		struct tm *tm_n = localtime(&now);

		return time_left_counter(*tm_n, tm_e);
	}
	else
		throw std::runtime_error("Database doesn't connected");
}

void DataBase::update_sub(std::string gate_id, std::string s_start, std::string s_end) {
	if(dbc->is_connected()) {
		check_for_exist(gate_id);
		std::string request = "UPDATE subs_table SET sub_start_date=\'" + s_start + "\', sub_end_date=\'" + s_end + "\' WHERE gate_id=" + gate_id + ";";
		dbc->query(request);
	}
	else
		throw std::runtime_error("Database doesn't connected");
}

void DataBase::insert_sub(std::string gate_id, std::string s_start, std::string s_end) {
	if(dbc->is_connected()) {
		std::string request = "INSERT INTO subs_table(gate_id, sub_start_date, sub_end_date) VALUES (\'" + gate_id + "\', \'" + s_start + "\', \'" + s_end + "\');";
		dbc->query(request);
	}
	else
		throw std::runtime_error("Database doesn't connected");

}
void DataBase::delete_sub(std::string gate_id) {
	if(dbc->is_connected()) {
		check_for_exist(gate_id);
		std::string request = "DELETE FROM subs_table WHERE gate_id='" + gate_id + "';";
		dbc->query(request);
	}
	else {
		throw std::runtime_error("Database doesn't connected");
	}
}

std::string DataBase::time_to_string(time_t t) {
	struct tm *tm = localtime(&t);
	char c[80];
	strftime(c, 80, "%Y.%m.%d %H:%M", tm);
	std::cout << std::string(c) << std::endl;
	return std::string(c);
}

void DataBase::check_for_exist(std::string gate_id) {
	std::string request = "SELECT * FROM subs_table WHERE gate_id='" + gate_id + "';";
	auto query_res_ptr = dbc->query(request);
	auto row = query_res_ptr->get_row();
	if(!row)
		throw std::runtime_error("Gate doesn't exist in database");
}
