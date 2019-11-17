#include "HTTPHandler.h"
#include "HTTPServer.h"
#include "HTTPClient.h"
#include "Logger.h"
#include "ConfigReader.h"
#include "nlohmann/json.hpp"

#include <iostream>
#include <signal.h>

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

HTTPHandler::Request build_request(HTTPHandler::Method method, string uri, string body="") {
	HTTPHandler::Request request;
	request.method = method;
	request.uri = uri;
	request.headers = {
		{"Connection", "close"},
	 	{"Content-Length", to_string(body.size())}
	};
	request.body = body;
	return request;
}

void check_sub(json req_body) {
	return;
	HTTPClient client;
	HTTPHandler::Answer answer;
	json ans_body;
	string user_id = req_body["user_id"].get<string>();

	client.connect_to_server(
		ConfigReader::reader.read_value_by_key<string>("SUB_SERVICE_HOST"),
		ConfigReader::reader.read_value_by_key<int>("SUB_SERVICE_PORT")
	);

	if(client.is_connected()) {
		client.send_request(build_request(HTTPHandler::Method::GET, "/sub_status?user_id=" + user_id));
		answer = client.read_answer();
		ans_body = json::parse(answer.body);
		if(ans_body["sub_status"] == nullptr)
			throw runtime_error(ans_body["exception"]);
		else if(ans_body["sub_status"] == false)
			throw runtime_error("Subscribe is invalid");
	}
	else
		throw runtime_error("Subscribe sevice is not responding");
}

string get_rasp_id(json req_body) {
	return "12345";
	HTTPClient client;
	HTTPHandler::Answer answer;
	json ans_body;
	string user_id = req_body["user_id"].get<string>();

	client.connect_to_server(
		ConfigReader::reader.read_value_by_key<string>("ID_SERVICE_HOST"),
		ConfigReader::reader.read_value_by_key<int>("ID_SERVICE_PORT")
	);

	if(client.is_connected()) {
		client.send_request(build_request(HTTPHandler::Method::GET, "/get_gate?user_id=" + user_id));
		answer = client.read_answer();
		ans_body = json::parse(answer.body);
		if(ans_body["gate_id"] == nullptr)
			throw runtime_error(ans_body["exception"]);
		else
			return ans_body["gate_id"];
	}
	else
		throw runtime_error("ID sevice is not responding");
}

void ac_send(json req_body) {
	HTTPClient client;
	HTTPHandler::Answer answer;
	json ans_body;

	client.connect_to_server(
		ConfigReader::reader.read_value_by_key<string>("AC_SERVICE_HOST"),
		ConfigReader::reader.read_value_by_key<int>("AC_SERVICE_PORT")
	);
	logger << "FUCK_AC_SEND" << endl;	

	if(client.is_connected()) {
		logger << "FUCK_AC_SENDA_after_if" << endl;       
		client.send_request(build_request(HTTPHandler::Method::POST, "/", req_body.dump(4))); // Слава не отправляет хедеры
		logger << "FUCK_AC_SENDA_after_client.send_request" << endl;
		answer = client.read_answer();
		logger << "FUCK_AC_SENDA_after_answer" << endl;
		ans_body = json::parse(answer.body);
		//if(ans_body["status"].get<string>() == "fail")
		//	throw runtime_error(ans_body["exception"]);
		logger << "FUCK_AC_SENDA_as_end_if" << endl;
	}
	else
		throw runtime_error("AC sevice is not responding");

}

HTTPHandler::Answer check_all(HTTPHandler::Request request) {
	HTTPHandler::Answer answer;
	json answer_json;
	string rasp_id;
	logger << "FUCK_check_all" << endl;	

	try {
		json req_body = json::parse(request.body);
		logger << "FUCK_parse" << endl;
		check_sub(req_body);
		rasp_id = get_rasp_id(req_body);
		logger << "FUCK_After_parse" << endl;
		req_body["gate_id"] = "12345";
		logger << "FUCK_AC_SENDA_in_check" << endl;
		ac_send(req_body);
		logger << "FUCK_AC_SENDA_after_ac_send" << endl;
		answer_json["status"] = "ok";
    } catch (exception& e) {
		set_bad_request(answer);
		answer_json["status"] = "fail";
	   	answer_json["exception"] = e.what();    }
	
	answer.body = answer_json.dump(4);
	return answer;
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

		logger << "Starting manager service..." << endl;


		HTTPServer server;
		logger << "Starting server..." << endl;
		server.start_server(
			ConfigReader::reader.read_value_by_key<int>("HTTP_SERVER_PORT")
		);
		server.turn_to_listen(10);

		// Capturing SIGINT signal
		signal(SIGINT, signal_handler);

		while(!finish) {
			logger << "Awaiting for connection..." << endl;
			int client_id = server.connect_client();
			logger << "Client connected" << endl;
			logger << "Getting request..." << endl;
			HTTPHandler::Request request = server.get_request(client_id);
			logger << "Request body:    " << request.body << endl; 
			logger << "Checking..." << endl;
			HTTPHandler::Answer answer = check_all(request);
			logger << "Sending answer" << endl;
			server.send_answer(client_id, answer);
			server.close_con(client_id);
			logger << "Connection closed" << endl;

		}
    } catch (exception& e) {
        logger << "[EXCEPTION] " << e.what() << endl;
    }

}
