#include "HTTPHandler.h"
#include "HTTPServer.h"
#include "HTTPClient.h"
#include "Logger.h"
#include "ConfigReader.h"
#include "nlohmann/json.hpp"

#include <unistd.h>
#include <signal.h>
#include <vector>
#include <future>
#include <thread>
#include <iostream>

using namespace std;
using json = nlohmann::json;


bool finish = false;

void signal_handler(int)
{
	finish = true;
}

void set_bad_request(HTTPHandler::Answer& answer) {
	answer.status_code = 403;
	answer.status_description = "Bad Request";
}

void set_ok(HTTPHandler::Answer& answer) {
	answer.status_code = 200;
	answer.status_description = "OK";
}

HTTPHandler::Request build_request(
		HTTPHandler::Method const& method, 
		string const& uri, 
		string const& api_key, 
		string const& body=""
		) {
	HTTPHandler::Request request;
	request = {
		method, uri,
		{},
		{
			{"Content-Type", "application/json"},
			{"Content-Length", to_string(body.size())},
			{"Connecton", "close"},
			{"Api-Key", api_key}
		},
		body
	};
	return request;
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

void check_sub(HTTPHandler::Request const& request, int const& send_code) {
	HTTPClient client;
	HTTPHandler::Answer answer;
	json ans_body;
	string user_id;
	if(send_code == 0)
		user_id = req_body["user_id"].get<string>();
	else if(send_code == 1)
		user_id = request.variables.at("user_id");

	client.connect_to_server(
		ConfigReader::reader.read_value_by_key<string>("SUB_SERVICE_HOST"),
		ConfigReader::reader.read_value_by_key<int>("SUB_SERVICE_PORT")
	);

	if(client.is_connected()) {
		logger << "Successfully connected to Subscribe service" << endl;

		HTTPHandler::Request request = 
			build_request(
					HTTPHandler::Method::GET, 
					"/sub_status?user_id=" + user_id,
					ConfigReader::reader.read_value_by_key<string>("SUB_API_KEY")
			);
		
		logger << "Subscribe service request:\n" << request_to_str(request) << endl;
		client.send_request(request);

		answer = client.read_answer();
		logger << "Subscribe service answer:\n" << answer_to_str(answer) << endl;

		ans_body = json::parse(answer.body);

		if(answer.status_code != 200 && !answer.body.empty()) {
			ans_body = json::parse(answer.body);
			if(ans_body["error_message"] != nullptr)
				throw runtime_error(ans_body["error_message"].get<string>());
			else
				throw runtime_error("Unknown error");
		}
		else if(ans_body["sub_status"].get<bool>() == false)
			throw runtime_error("Subscribe is invalid");
	}
	else
		throw runtime_error("Subscribe sevice is not responding");
}

string get_gate_id(HTTPHandler::Request const& request, int const& send_code) {
	HTTPClient client;
	HTTPHandler::Answer answer;
	json ans_body;
	json req_body = json::parse(request.body);
	string user_id;
	if(send_code == 0)
		user_id = req_body["user_id"].get<string>();
	else if(send-code == 1)
		user_id = request.variables.at("user_id"); 

	client.connect_to_server(
		ConfigReader::reader.read_value_by_key<string>("ID_SERVICE_HOST"),
		ConfigReader::reader.read_value_by_key<int>("ID_SERVICE_PORT")
	);

	if(client.is_connected()) {
		logger << "Successfully connected to ID service" << endl;

		HTTPHandler::Request request = 
			build_request(
					HTTPHandler::Method::GET, 
					"/get_gate?user_id=" + user_id,
					ConfigReader::reader.read_value_by_key<string>("ID_API_KEY")
			);

		logger << "ID service request:\n" << request_to_str(request) << endl;
		client.send_request(request);

		answer = client.read_answer();
		logger << "ID service answer:\n" << answer_to_str(answer) << endl;

		ans_body = json::parse(answer.body);

		if(answer.status_code != 200 && !answer.body.empty()) {
			ans_body = json::parse(answer.body);
			if(ans_body["error_message"] != nullptr)
				throw runtime_error(ans_body["error_message"].get<string>());
			else
				throw runtime_error("Unknown error");
		}
		else
			return ans_body["gate_id"].get<string>();
	}
	else
		throw runtime_error("ID sevice is not responding");
}

// send_code = 0 for send command
// send_code = 1 for check gate
void ac_send(json const& req_body, int const& send_code) {
	HTTPClient client;
	HTTPHandler::Answer answer;
	HTTPHandler::Request request;
	json ans_body;

	client.connect_to_server(
		ConfigReader::reader.read_value_by_key<string>("AC_SERVICE_HOST"),
		ConfigReader::reader.read_value_by_key<int>("AC_SERVICE_PORT")
	);
	
	if(client.is_connected()) {
		logger << "Successfully connected to AC service" << endl;
		if(send_code == 0) {
			request = 
				build_request(
						HTTPHandler::Method::POST, 
						"/", 
						ConfigReader::reader.read_value_by_key<string>("AC_API_KEY"),
						req_body.dump(4)
				);
		}
		else if(send_code == 1) {
			request = 
				build_request(
						HTTPHandler::Method::GET, 
						"/connection?gate_id=" + req_body["gate_id"].get<string>(), 
						ConfigReader::reader.read_value_by_key<string>("AC_API_KEY")
				);
		}

		logger << "AC service request:\n" << request_to_str(request) << endl;
		client.send_request(request);

		answer = client.read_answer();
		logger << "AC service answer:\n" << answer_to_str(answer) << endl;

		if(answer.status_code != 200 && !answer.body.empty()) {
			ans_body = json::parse(answer.body);
			if(ans_body["error_message"] != nullptr)
				throw runtime_error(ans_body["error_message"].get<string>());
			else
				throw runtime_error("Unknown error");
		}
	}
	else
		throw runtime_error("AC sevice is not responding");

}

void check_for_api_key(string const& api_key) {
	if(
	   api_key !=
	   ConfigReader::reader.read_value_by_key<string>("API_KEY")
	  )
		throw runtime_error("API key is incorrect");
}

json check_all(HTTPHandler::Request request) {
	json answer_json;
	string gate_id;
	
	json req_body = json::parse(request.body);

	check_sub(req_body);

	gate_id = get_gate_id(req_body);

	req_body["gate_id"] = gate_id;
	ac_send(req_body, 0);

	return answer_json;
	
}


HTTPHandler::Answer choose_way(HTTPHandler::Request request) {
	HTTPHandler::Answer answer;
	json answer_body;
	json req_body;
	string gate_id;

	try {
		check_for_api_key(request.headers.at("Api-Key"));


		switch(request.method) {
			case HTTPHandler::Method::POST:
				answer_body = check_all(request);
				break;
			case HTTPHandler::Method::GET:
				if(request.uri == "/sub_check") {
						check_sub(request, 1);
				} else if(request.uri == "/gate_id") {
						gate_id = get_gate_id(request, 1);
						answer_body["gate_id"] = gate_id;
				} else if(request.uri == "/gate_check") {
						gate_id = get_gate_id(request, 1);
						req_body["gate_id"] = gate_id;
						ac_send(req_body, 1);
				}
				break;
		}
		set_ok(answer);
    } catch (exception& e) {
		set_bad_request(answer);
	   	answer_body["error_message"] = e.what();
		logger << "[EXCEPTION] " << e.what() << endl;
    }
	answer.body = answer_body.dump(4);
	return answer;
}

void handle_client(int client_sock)
{
	HTTPHandler::Answer answer;

	try {
		HTTPHandler::Request request = HTTPServer::get_request(client_sock);
		logger << "Voice Assistant request:\n" << request_to_str(request) << endl;

		logger << "Starting to send requests to all services..." << endl;
		answer = choose_way(request);
		logger << "Answer to Voice Assistant:\n" << answer_to_str(answer) << endl;

		logger << "Sending answer to Voice Assistant..." << endl;
		HTTPServer::send_answer(client_sock, answer);

	} catch (HTTPServer::ClientDisconnected& ex) {
		logger << ex.what() << endl;
		HTTPServer::close_con(client_sock);
		logger << "Connection closed with client id:" << client_sock << endl;
		return;
	}

	HTTPServer::close_con(client_sock);
	logger << "Connection closed with client id:" << client_sock << endl;
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
		// Capturing SIGINT signal
		signal(SIGINT, signal_handler);

		ConfigReader::reader.set_file_name("config.txt");
		ConfigReader::reader.read_config(); // Buffering all configs

		logger.open(ConfigReader::reader.read_value_by_key<string>("LOG_FILE"));

		logger << "Starting manager service..." << endl;


		HTTPServer server;
		logger << "Starting server..." << endl;
		server.start_server(
			ConfigReader::reader.read_value_by_key<int>("HTTP_SERVER_PORT")
		);
		server.turn_to_listen(10);


		vector<future<void>> futures;
		while(!server.is_interrupted()) {
			logger << "Awaiting for connection..." << endl;
			int client_sock = server.connect_client();
			logger << "Client on socket " << client_sock << " was connected" << endl;

			futures.push_back(async(&handle_client, client_sock));
		}
    } catch (exception& e) {
        logger << "[EXCEPTION] " << e.what() << endl;
    }

}
