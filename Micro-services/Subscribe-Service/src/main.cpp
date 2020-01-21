#include "database.h"
#include "HTTPServer.h"
#include "HTTPHandler.h"
#include "Logger.h"
#include "nlohmann/json.hpp"
#include "ConfigReader.h"

#include <unistd.h>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <future>
#include <thread>
#include <iostream>
#include <functional>


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

void check_for_api_key(string const& api_key) {
	if(
	   api_key !=
	   ConfigReader::reader.read_value_by_key<string>("API_KEY")
	  )
		throw runtime_error("API key is incorrect");
}

HTTPHandler::Answer work_with_db(DataBase& db, HTTPHandler::Request const& request) {
	HTTPHandler::Answer answer;
	struct tm tm;
	json args_json;
	json answer_json;


	try {
		check_for_api_key(request.headers.at("Api-Key"));

		db.ping_db();

		switch(request.method) {
			case HTTPHandler::Method::GET:
				if(request.uri == "/sub_status")
					answer_json["sub_status"] = db.check_for_sub(request.variables.at("gate_id"));
				else if(request.uri == "/sub_left") {
					DataBase::Time t_r = db.time_left(request.variables.at("gate_id"));
					answer_json["minutes"] = t_r.minutes;
					answer_json["hours"] = t_r.hours;
					answer_json["days"] = t_r.days;
				}
				else
					throw runtime_error("Unknown command");
				break;
			case HTTPHandler::Method::PUT:
				args_json = json::parse(request.body);
				db.update_sub(args_json["gate_id"].get<string>(), 
							  args_json["sub_start_date"].get<string>(), 
							  args_json["sub_end_date"].get<string>()
							 );
				break;
			case HTTPHandler::Method::POST:
				args_json = json::parse(request.body);
				db.insert_sub(args_json["gate_id"].get<string>(), 
							  args_json["sub_start_date"].get<string>(), 
							  args_json["sub_end_date"].get<string>()
							 );
				break;
			case HTTPHandler::Method::DELETE:
				db.delete_sub(request.variables.at("gate_id"));
				break;
			default:
				throw runtime_error("Unknown HTTP Method");
				break;
		}
		set_ok(answer);
		logger << "Work was ended without exceptions" << endl;
	} catch(exception& e) {
		logger << "[EXCEPTION on work] " << e.what() << endl;
		set_bad_request(answer);
	   	answer_json["error_message"] = e.what();
	}
	
	answer.body = answer_json.dump(4);
	answer.headers.insert(pair<string, string>("Content-Type", "application/json"));
	answer.headers.insert(pair<string, string>("Content-Length", to_string(answer.body.size())));
	answer.headers.insert(pair<string, string>("Connecton", "close"));
	return answer;
}

string request_to_str(HTTPHandler::Request const& request) {
	stringstream result_ss;
	HTTPHandler::write_request(request, result_ss);
	return result_ss.str();
}

string answer_to_str(HTTPHandler::Answer const& answer) {
	stringstream result_ss;
	HTTPHandler::write_answer(answer, result_ss);
	return result_ss.str();
}


void signal_handler(int) {
	finish = true;
}

void handle_client(int client_sock, DataBase &db)
{
	HTTPHandler::Answer answer;

	try {
		HTTPHandler::Request request = HTTPServer::get_request(client_sock);
		logger << "Request:\n" << request_to_str(request) << endl;

		answer = work_with_db(db, request);

		logger << "Answer:\n" << answer_to_str(answer) << endl;
		HTTPServer::send_answer(client_sock, answer);
		logger << "Answer was sent" << endl;

	} catch (HTTPServer::ClientDisconnected& ex) {
		logger << ex.what() << endl;
		HTTPServer::close_con(client_sock);
		logger << "Connection closed with client id:" << client_sock << endl;
		return;
	}

	HTTPServer::close_con(client_sock);
	logger << "Connection closed with client id:" << client_sock << endl;
}



int main(int argc, char** argv) {

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

		// Capturing SIGINT signal
		signal(SIGINT, signal_handler);
		ConfigReader::reader.set_file_name("config.txt");
		ConfigReader::reader.read_config(); // Buffering all configs

		logger.open(ConfigReader::reader.read_value_by_key<string>("LOG_FILE"));

		logger << "SubService starting..." << endl;

		logger << "Initating database..." << endl;
		DataBase db;
		db.init(
				ConfigReader::reader.read_value_by_key<string>("DB_HOSTNAME").c_str(),
				ConfigReader::reader.read_value_by_key<string>("DB_NAME").c_str(),
				ConfigReader::reader.read_value_by_key<string>("DB_USERNAME").c_str(),
				ConfigReader::reader.read_value_by_key<string>("DB_PASSWORD").c_str()
			   );
		logger << "Database initiated successfully" << endl;

		HTTPServer server;
		logger << "Starting server..." << endl;
		server.start_server(ConfigReader::reader.read_value_by_key<int>("HTTP_SERVER_PORT"));
		server.turn_to_listen(10);


		vector<future<void>> futures;

		while(!server.is_interrupted()) {
			logger << "Awaiting for connection..." << endl;
			int client_sock = server.connect_client();
			logger << "Client with id: " << client_sock << " was connected" << endl;
			futures.push_back(async(handle_client, client_sock, std::ref(db)));
		}

    } catch (exception& e) {
        logger << "[EXCEPTION] " << e.what() << endl;
    }

	logger << "Shutting down..." << endl;
	return 0;
}
