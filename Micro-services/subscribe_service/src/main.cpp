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
/*
HTTPHandler::Answer work_with_db(const DataBase& db, const HTTPHandler::Request& request) {
	HTTPHandler::Answer answer;
	int user_id = std::stoi(request.uti);
	try {
		switch(request.method) {
			case GET:
				db.check_for_sub(user_id);
				break;
			case POST:
				break;
			case PUT:
				break;
			case DELETE:
				db.delete_sub(user_id)
				break;
			case default:
				set_not_found(answer);
				break;
		set_ok(answer);
		}
	} catch {
		set_not_found(answer);
	}
	return answer;
}
*/
int main()
{
	/*
	try {
        DataBase db;
		db.init(DB_HOSTNAME, DB_NAME, DB_USERNAME, DB_PASSWORD);
        bool check_res = db.check_for_sub(1);
		cout << check_res << endl;
    } catch (char *e) {
        cerr << "[EXCEPTION] " << e << endl;
        return 1;
    }
	*/
	int port = 3000;

	HTTPServer server;
	server.start_server(port);
	server.turn_to_listen(10);
	while(true) {
		int client_id = server.connect_client();
		std::string str_request = server.get_request(client_id);
		HTTPHandler::Request request = server.handling_request(str_request);


		// SMTH with request and preparing HTTPHandler::Answer
		// server.send_answer(client_id, answer);
		server.close_con(client_id);
	}
}
