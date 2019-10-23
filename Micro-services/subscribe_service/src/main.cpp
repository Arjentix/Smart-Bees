#include "database.h"
#include "HTTPServer.h"
#include "HTTPHandler.h"
#include "nlohmann/json.hpp"


using namespace std;
using json = nlohmann::json;

const char* DB_HOSTNAME = "localhost";
const char* DB_NAME = "alice_subs";
const char* DB_USERNAME = "http_server";
const char* DB_PASSWORD = "12345678";


void set_bad_request(HTTPHandler::Answer& answer) {
	answer.status_code = 403;
	answer.status_description = "Bad Request";
}

void set_ok(HTTPHandler::Answer& answer) {
	answer.status_code = 200;
	answer.status_description = "OK";
}

HTTPHandler::Answer work_with_db(DataBase& db, const HTTPHandler::Request& request) {
	HTTPHandler::Answer answer;
	struct tm tm;
	json args_json;
	json answer_json;
	int user_id;
	string command;


	try {
		if(request.method == HTTPHandler::Method::POST || request.method == HTTPHandler::Method::PUT)
			args_json = json::parse(request.body);
		else {
			user_id = stoi(request.variables.at("user_id"));
			if(request.method == HTTPHandler::Method::GET)
				command = request.uri;
		}
		switch(request.method) {
			case HTTPHandler::Method::GET:
				if(command == "/sub_status")
					answer_json["sub_status"] = db.check_for_sub(user_id);
				else if(command == "/sub_left") {
					DataBase::Time t_r = db.time_left(user_id);
					answer_json["minutes"] = t_r.minutes;
					answer_json["hours"] = t_r.hours;
					answer_json["days"] = t_r.days;
				}
				else
					throw runtime_error("Unknown command");
				break;
			case HTTPHandler::Method::PUT:
				db.update_sub(args_json["user_id"], args_json["sub_start_date"], args_json["sub_end_date"]);
				break;
			case HTTPHandler::Method::POST:
				db.insert_sub(args_json["user_id"], args_json["username"], args_json["sub_start_date"], args_json["sub_end_date"]);
				break;
			case HTTPHandler::Method::DELETE:
				db.delete_sub(user_id);
				break;
		}
		set_ok(answer);
	} catch(std::runtime_error& e) {
		set_bad_request(answer);
	   	answer_json["exception"] = e.what();
	} catch(exception& e) {
		set_bad_request(answer);
	   	answer_json["exception"] = e.what();
	}
	answer.body = answer_json.dump(4);
	return answer;
}

int main()
{
	const int port = 3000;

	try {
   		DataBase db;
		db.init(DB_HOSTNAME, DB_NAME, DB_USERNAME, DB_PASSWORD);
		HTTPServer server;
		server.start_server(port);
		server.turn_to_listen(10);
		while(true) {
			cout << "Awaiting for connection..." << endl;
			int client_id = server.connect_client();
			HTTPHandler::Request request = server.get_request(client_id);
			HTTPHandler::Answer answer = work_with_db(db, request);
			server.send_answer(client_id, answer);
			server.close_con(client_id);
		}
	} catch(runtime_error& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    } catch (exception& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    }
}
