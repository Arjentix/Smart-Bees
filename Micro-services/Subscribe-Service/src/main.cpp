#include "database.h"
#include "HTTPServer.h"
#include "HTTPHandler.h"
#include "Logger.h"
#include "nlohmann/json.hpp"
#include "ConfigReader.h"

#include <unistd.h>
#include <signal.h>

using namespace std;
using json = nlohmann::json;


bool finish = false;

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
		logger << "work was ended without exceptions" << endl;
	} catch(exception& e) {
		logger << "[EXCEPTION on work] " << e.what() << endl;
		set_bad_request(answer);
	   	answer_json["exception"] = e.what();
	}
	answer.body = answer_json.dump(4);
	return answer;
}

void signal_handler(int)
{
	finish = true;
}

int main(int argc, char** argv)
{

	// Daemon mode
	if (argc > 1 && strcmp(argv[1], "-d") == 0) {
		int fork_res = fork();
		if (fork_res == -1) {	// Error
			return -1;
		}
		if (fork_res > 0) {		// Parent
			return 0;
		}
		
		// Child 
		// Closing useless fds
		close(0);
		close(1);
		close(2);
	}


	try {
		ConfigReader::reader.set_file_name("config.txt");
		ConfigReader::reader.read_config(); // Buffering all configs

		logger.open(ConfigReader::reader.read_value_by_key<string>("LOG_FILE"));
		logger << "Start" << endl;

		logger << "Initating database" << endl;
   		DataBase db;
		db.init(
				ConfigReader::reader.read_value_by_key<string>("DB_HOSTNAME"),
				ConfigReader::reader.read_value_by_key<string>("DB_NAME"),
				ConfigReader::reader.read_value_by_key<string>("DB_USERNAME"),
				ConfigReader::reader.read_value_by_key<string>("DB_PASSWORD"),
			   );

		HTTPServer server;
		logger << "Starting server..." << endl;
		server.start_server(ConfigReader::reader.read_value_by_key<int>("HTTP_SERVER_PORT"));
		server.turn_to_listen(10);

		// Capturing SIGINT signal
		signal(SIGINT, signal_handler);

		while(!finish) {
			logger << "Awaiting for connection..." << endl;
			int client_id = server.connect_client();
			logger << "Client was connected" << endl;
			HTTPHandler::Request request = server.get_request(client_id);
			HTTPHandler::Answer answer = work_with_db(db, request);
			server.send_answer(client_id, answer);
			logger << "Answer sent" << endl;
			server.close_con(client_id);
			logger << "Connection closed" << endl;
		}
    } catch (exception& e) {
        logger << "[EXCEPTION] " << e.what() << endl;
    }
	logger << "Shutting down..." << endl;
	return 0;
}
