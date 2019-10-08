#include "database.h"
#include "HTTPServer.h"


using namespace std;

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
	int user_id = std::stoi(request.uri.substr(1, request.uri.size()));
	time_t sub_start, sub_end;
	string sub_start_str, sub_end_str;
	try {
		switch(request.method) {
			case HTTPHandler::Method::GET:
				answer.body = to_string(db.check_for_sub(user_id));
				break;
			case HTTPHandler::Method::PUT:
				sub_start_str = request.headers.at("sub_start_date");
				sub_end_str = request.headers.at("sub_end_date");
				strptime(sub_start_str.c_str(), "%Y.%m.%d", &tm);
				sub_start = mktime(&tm);
				strptime(sub_end_str.c_str(), "%Y.%m.%d", &tm);
				sub_end = mktime(&tm);
				db.update_sub(user_id, sub_start, sub_end);
				answer.body = "User id: " + to_string(user_id) + " successfuly updated";
				break;
			case HTTPHandler::Method::POST:
				sub_start_str = request.headers.at("sub_start_date");
				sub_end_str = request.headers.at("sub_end_date");
				strptime(sub_start_str.c_str(), "%Y.%m.%d", &tm);
				sub_start = mktime(&tm);
				strptime(sub_end_str.c_str(), "%Y.%m.%d", &tm);
				sub_end = mktime(&tm);
				user_id = db.insert_sub(request.headers.at("username"), sub_start, sub_end);
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
			cout << "Going to handling" << endl;
			string str_request = server.get_request(client_id);
			cout << "\"" << str_request << "\"" << endl;
			HTTPHandler::Request request = server.handling_request(str_request);
			cout << "go to work" << endl;
			HTTPHandler::Answer answer = work_with_db(db, request);
			cout << answer.status_code << " " << answer.status_description << endl << answer.body << endl;
			cout << "try to send" << endl;
			server.send_answer(client_id, answer);
			cout << "closing" << endl;
			server.close_con(client_id);
			cout << "closed" << endl;
		}
	} catch(std::runtime_error& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    } catch (exception& e) {
        cout << "[EXCEPTION] " << e.what() << endl;
        return 1;
    }
}
