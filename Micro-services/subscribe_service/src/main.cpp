#include "database.h"
#include "HTTPServer.h"
#include "HTTPHandler.h"
#include <json.hpp>


using namespace std;
using json = nlohmann::json;

const char* DB_HOSTNAME = "localhost";
const char* DB_NAME = "alice_subs";
const char* DB_USERNAME = "http_server";
const char* DB_PASSWORD = "12345678";


void set_not_found(HTTPHandler::Answer& answer) {
	answer.status_code = 404;
	answer.status_description = "Not Found";
}

void set_ok(HTTPHandler::Answer& answer) {
	answer.status_code = 200;
	answer.status_description = "OK";
}

HTTPHandler::Answer work_with_db(DataBase& db, const HTTPHandler::Request& request) {
	HTTPHandler::Answer answer;
	struct tm tm;
	auto args_json = json::parse(request.body);
	int user_id;

	if(request.method != HTTPHandler::Method::PUT)
		user_id = args_json["user_id"];
	try {
		switch(request.method) {
			case HTTPHandler::Method::GET:
				answer.body = to_string(db.check_for_sub(user_id));
				break;
			case HTTPHandler::Method::PUT:
				db.update_sub(user_id, args_json["sub_start_date"], args_json["sub_end_date"]);
				answer.body = "User id: " + to_string(user_id) + " successfuly updated";
				break;
			case HTTPHandler::Method::POST:
				user_id = db.insert_sub(args_json["username"], args_json["sub_start_date"], args_json["sub_end_date"]);
				answer.body = to_string(user_id);
				break;
			case HTTPHandler::Method::DELETE:
				db.delete_sub(user_id);
				answer.body = "User id: " + to_string(user_id) + " successfuly deleted";
				break;
		}
		set_ok(answer);
	} catch(std::runtime_error& e) {
		set_not_found(answer);
		answer.body = "[EXCEPTION] ";
	   	answer.body += e.what();
	} catch(exception& e) {
		set_not_found(answer);
		answer.body = "[EXCEPTION] ";
	   	answer.body += e.what();
	}
	return answer;
}

int main()
{
	/*
	try {
        DataBase db;
		db.init(DB_HOSTNAME, DB_NAME, DB_USERNAME, DB_PASSWORD);
        db.delete_sub(1);
	} catch(std::runtime_error& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    } catch (exception& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    }
	*/
	int port = 3000;

	try {
   		DataBase db;
		db.init(DB_HOSTNAME, DB_NAME, DB_USERNAME, DB_PASSWORD);
		HTTPServer server;
		server.start_server(port);
		server.turn_to_listen(10);
		while(true) {
			cout << "Awaiting for connection..." << endl;
			int client_id = server.connect_client();
			string str_request = server.get_request(client_id);
			HTTPHandler::Request request = HTTPHandler::parse_request(str_request);
			HTTPHandler::Answer answer = work_with_db(db, request);
			stringstream answer_ss;
			HTTPHandler::write_answer(answer, answer_ss);
			server.send_answer(client_id, answer_ss);
			server.close_con(client_id);
		}
	} catch(std::runtime_error& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    } catch (exception& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    }
}
